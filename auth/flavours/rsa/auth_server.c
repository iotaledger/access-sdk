/*
 * This file is part of the IOTA Access distribution
 * (https://github.com/iotaledger/access)
 *
 * Copyright (c) 2020 IOTA Stiftung
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/****************************************************************************
 * \project IOTA Access
 * \file auth_client.c
 * \brief
 * Server side implemntation for RSA based authentication module
 *
 * @Author Djordje Golubovic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 05.05.2020. Initial version.
 * 01.08.2020. Renaming.
 ****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "auth.h"
#include "auth_debug.h"
#include "auth_internal.h"
#include "auth_utils.h"

/* AUTH_STAGES */
#define AUTH_ERROR 0
#define AUTH_INIT 1
#define AUTH_COMPUTE 2
#define AUTH_VERIFY 3
#define AUTH_FINISH 4
#define AUTH_DONE 5

/*
 * Stage 0 - Server initialization
 *
 */

int auth_server_init(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;
  BIGNUM *e = BN_new();
  debug("auth_server_init START");

  /* Generate RSA key */
  AUTH_GET_INTERNAL_RSA_S(session) = RSA_new();
  BN_set_word(e, RSA_F4);
  RSA_generate_key_ex(AUTH_GET_INTERNAL_RSA_S(session), AUTH_RSA_KEY_LEN, e, NULL);

  AUTH_GET_INTERNAL_DH(session) = DH_new();

  /* Server generates vs, and y. */
  auth_utils_randmem(AUTH_GET_INTERNAL_VS(session), AUTH_V_STRING_LEN);

  AUTH_GET_INTERNAL_OUT_PACKET_COUNT(session) = 1;
  AUTH_GET_INTERNAL_IN_PACKET_COUNT(session) = 1;

  memset(AUTH_GET_INTERNAL_H(session), 0, RSA_DIGEST_LENGTH);
  memset(AUTH_GET_INTERNAL_HC(session), 0, RSA_DIGEST_LENGTH);

  next_stage = AUTH_COMPUTE;

  debug("auth_server_init END");

  return next_stage;
}

/* Stage 1 - Compute DH Client parameters
 *
 * Server receives ( p || g || e || vc ) from Client.
 * Server generates vs, and y.
 * Server calculates f = gy mod p.
 * Server computes k = ey mod p
 * Server computes h  = hash( vc || vs || ks || e || f || k )
 * Server computes signature s = sign( sks, h )
 * Server sends ( f || s || ks || vs )
 *
 */

static inline int auth_server_compute_receive(auth_ctx_t *session, unsigned char **p, unsigned short *p_len,
                                             unsigned char **g, unsigned short *g_len, unsigned char **e,
                                             unsigned short *e_len, unsigned char **vc, unsigned short *vc_len) {
  int ret = AUTH_ERROR;

  ret = auth_utils_receive_message_part(session, p, p_len);
  // TODO: Check return

  ret = auth_utils_receive_message_part(session, g, g_len);
  // TODO: Check return

  ret = auth_utils_receive_message_part(session, e, e_len);
  // TODO: Check return

  ret = auth_utils_receive_message_part(session, vc, vc_len);
  // TODO: Check return

  return ret;
}

static inline int auth_server_compute_send(auth_ctx_t *session, const BIGNUM *f, unsigned char *s, unsigned short s_len,
                                           unsigned char *ks, unsigned short ks_len) {
  int ret = AUTH_ERROR;

  debug("send f");
  ret = auth_utils_send_message_part_bignum(session, f);
  // TODO: Check return

  debug("send s");
  ret = auth_utils_send_message_part(session, s, s_len);
  // TODO: Check return

  debug("send ks");
  ret = auth_utils_send_message_part(session, ks, ks_len);
  // TODO: Check return

  debug("send vs");
  ret = auth_utils_send_message_part(session, AUTH_GET_INTERNAL_VS(session), AUTH_V_STRING_LEN);
  // TODO: Check return

  return ret;
}

static inline int auth_server_compute_populate_server_keys(auth_ctx_t *session, BIGNUM *p, BIGNUM *g, BIGNUM *pub_key) {
  int ret = AUTH_ERROR;
  int ret_ossl;

  if (NULL != AUTH_GET_INTERNAL_DH(session)) /* DH parameters generated */
  {
    /* Set parameters from Client */
    ret_ossl = DH_set0_pqg(AUTH_GET_INTERNAL_DH(session), p, NULL, g);

    if (0 != ret_ossl) {
      /* Server calculates f = gy mod p. */
      ret_ossl = DH_generate_key(AUTH_GET_INTERNAL_DH(session));

      if (0 != ret_ossl) /* Keys generated */
      {
        int len = DH_size(AUTH_GET_INTERNAL_DH(session));
        AUTH_GET_INTERNAL_K(session) = malloc(len);

        if (NULL != AUTH_GET_INTERNAL_K(session)) {
          /* Server computes k = ey mod p */
          AUTH_GET_INTERNAL_K_LEN(session) =
              DH_compute_key(AUTH_GET_INTERNAL_K(session), pub_key, AUTH_GET_INTERNAL_DH(session));

          ret = AUTH_OK;
        }
      }
    }
  }

  return ret;
}

int auth_server_compute(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;
  int ret = AUTH_ERROR;

  unsigned char *sp = NULL, *sg = NULL, *se = NULL, *vc = NULL, *ks = NULL;
  unsigned short sp_len = 0, sg_len = 0, se_len = 0, vc_len = 0, ks_len = 0;
  BIGNUM *p = NULL, *g = NULL;
  const BIGNUM *spub_key = NULL;
  unsigned int s_len = 0;
  unsigned char s[AUTH_RSA_SIGN_LEN] = {
      0,
  };

  debug("auth_server_compute START");

  // Receive ( p || g || e || vc )
  ret = auth_server_compute_receive(session, &sp, &sp_len, &sg, &sg_len, &se, &se_len, &vc, &vc_len);
  // TODO: Check return

  memcpy(AUTH_GET_INTERNAL_VC(session), vc, vc_len);

  auth_utils_debug_binary("p", sp, sp_len);

  p = BN_bin2bn(sp, sp_len, NULL);

  auth_utils_debug_binary("g", sg, sg_len);

  g = BN_bin2bn(sg, sg_len, NULL);

  auth_utils_debug_binary("e", se, se_len);

  AUTH_GET_INTERNAL_PUB_K(session) = BN_bin2bn(se, se_len, NULL);

  ret = auth_server_compute_populate_server_keys(session, p, g, AUTH_GET_INTERNAL_PUB_K(session));
  // TODO: Check return

  DH_get0_key(AUTH_GET_INTERNAL_DH(session), &spub_key, NULL);

  /* Server computes h  = hash( vc || vs || ks || e || f || k ) */
  ks_len = i2d_RSAPublicKey(AUTH_GET_INTERNAL_RSA_S(session), &ks);
  ret = auth_utils_compute_hash(session, AUTH_GET_INTERNAL_H(session), ks, ks_len, AUTH_GET_INTERNAL_PUB_K(session),
                              spub_key);

  /* Server computes signature s = sign( sks, h ) */
  /*NID_sha256*/
  RSA_sign(/*NID_sha256*/ 0, AUTH_GET_INTERNAL_H(session), RSA_DIGEST_LENGTH, s, &s_len,
           AUTH_GET_INTERNAL_RSA_S(session));
  auth_utils_debug_binary("RSA_Server_sign", s, s_len);

  /* Server sends ( f || s || ks || vs ) */
  ret = auth_server_compute_send(session, spub_key, s, s_len, ks, ks_len);
  // TODO: Check return

  free(sp);
  free(sg);
  free(se);
  free(vc);
  free(ks);

  next_stage = AUTH_VERIFY;

  debug("auth_server_compute END");

  return next_stage;
}

/* Stage 2 - Verify Client parameters
 *
 * Server receives ( kc || sc )
 * Server computes hc = hash( vc || vs || kc || e || f || k )
 * Server verifies the signature sc on hc
 *
 */

static inline int auth_server_verify_receive(auth_ctx_t *session, unsigned char **kc, unsigned short *kc_len,
                                             unsigned char **sc, unsigned short *sc_len) {
  int ret = AUTH_ERROR;

  ret = auth_utils_receive_message_part(session, kc, kc_len);
  // TODO: Check return

  ret = auth_utils_receive_message_part(session, sc, sc_len);
  // TODO: Check return

  return ret;
}

int auth_server_verify(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;
  int ret = AUTH_ERROR;

  unsigned char *kc = NULL, *s = NULL;
  unsigned short kc_len = 0, s_len = 0;
  const BIGNUM *spub_key = NULL;

  debug("auth_server_verify START");

  // Receive ( kc || sc )
  ret = auth_server_verify_receive(session, &kc, &kc_len, &s, &s_len);
  // TODO: Check return

  /* Server checks Client key */
  ret = session->f_verify(kc, kc_len);
  // TODO: Check return

  DH_get0_key(AUTH_GET_INTERNAL_DH(session), &spub_key, NULL);
  /* Server computes hc = hash( vc || vs || kc || e || f || k ) */
  ret = auth_utils_compute_hash(session, AUTH_GET_INTERNAL_HC(session), kc, kc_len, AUTH_GET_INTERNAL_PUB_K(session),
                              spub_key);

  /* Server verifies the signature sc on hc */
  AUTH_GET_INTERNAL_RSA_C(session) = d2i_RSAPublicKey(NULL, (const unsigned char **)&kc, kc_len);
  ret = RSA_verify(/*NID_sha256*/ 0, AUTH_GET_INTERNAL_HC(session), RSA_DIGEST_LENGTH, s, s_len,
                   AUTH_GET_INTERNAL_RSA_C(session));
  debug("!!!!!!!!!!!!!!!!!!!!!!!!!!!RSA_verify (%d)", ret);
  auth_utils_debug_binary("RSA_Client_sign", s, s_len);

  free(s);

  next_stage = AUTH_FINISH;

  debug("auth_server_verify END");

  return next_stage;
}

/* Stage 3 - Finish authentication
 *
 * Server generates AES keys.
 * Waits for encryption / decryption tasks :)
 *
 */

int auth_server_finish(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;

  debug("auth_server_finish START");

  auth_utils_debug_binary("k", AUTH_GET_INTERNAL_K(session), AUTH_GET_INTERNAL_K_LEN(session));
  auth_utils_debug_binary("h", AUTH_GET_INTERNAL_H(session), RSA_DIGEST_LENGTH);
  auth_utils_debug_binary("hc", AUTH_GET_INTERNAL_HC(session), RSA_DIGEST_LENGTH);

  // Generate AES, HMAC keys
  auth_utils_generate_keys(session);

  AUTH_GET_INTERNAL_KEY_CS(session) = malloc(sizeof(AES_KEY));
  AUTH_GET_INTERNAL_KEY_SC(session) = malloc(sizeof(AES_KEY));

  memset(AUTH_GET_INTERNAL_KEY_CS(session), 0, sizeof(AES_KEY));
  memset(AUTH_GET_INTERNAL_KEY_SC(session), 0, sizeof(AES_KEY));

  // Initialize Server -> Client Encrypt Key
  AES_set_encrypt_key(AUTH_GET_INTERNAL_EKEY_S(session), AUTH_AES_KEY_LEN, AUTH_GET_INTERNAL_KEY_SC(session));

  // Initialize Client -> Server Decrypt Key
  AES_set_decrypt_key(AUTH_GET_INTERNAL_EKEY_C(session), AUTH_AES_KEY_LEN, AUTH_GET_INTERNAL_KEY_CS(session));

  {
    const BIGNUM *priv_key = NULL;
    unsigned char *ks = NULL;
    int ks_len = 0;
    int len = 0;

    DH_get0_key(AUTH_GET_INTERNAL_DH(session), NULL, &priv_key);

    len = BN_num_bytes(priv_key);

    unsigned char *data = malloc(len);

    len = BN_bn2bin(priv_key, data);

    auth_utils_debug_binary("y", data, len);

    ks_len = i2d_RSAPrivateKey(AUTH_GET_INTERNAL_RSA_S(session), &ks);

    auth_utils_debug_binary("ks", ks, ks_len);
  }

  next_stage = AUTH_DONE;

  debug("auth_server_finish END");

  return next_stage;
}

/*
 *
 */

int auth_internal_server_authenticate(auth_ctx_t *session) {
  int ret = AUTH_ERROR;

  int auth_stage = AUTH_INIT;

  debug("auth_internal_server_authenticate START");

  while ((AUTH_DONE != auth_stage) && (AUTH_ERROR != auth_stage)) {
    switch (auth_stage) {
      case AUTH_INIT:
        auth_stage = auth_server_init(session);
        break;
      case AUTH_COMPUTE:
        auth_stage = auth_server_compute(session);
        break;
      case AUTH_VERIFY:
        auth_stage = auth_server_verify(session);
        break;
      case AUTH_FINISH:
        auth_stage = auth_server_finish(session);
        break;
      default:
        break;
    }
  }

  if (AUTH_DONE == auth_stage) {
    ret = AUTH_OK;
  }

  debug("auth_internal_server_authenticate END");

  return ret;
}

int auth_internal_server_send(auth_ctx_t *session, const unsigned char *data, unsigned short data_len) {
  return auth_utils_send(session, AUTH_GET_INTERNAL_IKEY_S(session), AUTH_GET_INTERNAL_KEY_SC(session),
                       AUTH_GET_INTERNAL_IV_S(session), data, data_len);
}

int auth_internal_server_receive(auth_ctx_t *session, unsigned char **data, unsigned short *data_len) {
  return auth_utils_receive(session, AUTH_GET_INTERNAL_IKEY_C(session), AUTH_GET_INTERNAL_KEY_CS(session),
                          AUTH_GET_INTERNAL_IV_C(session), data, data_len);
}

void auth_internal_release_server(auth_ctx_t *session) {}

int auth_internal_server_set_option(auth_ctx_t *session, const char *key, unsigned char *value) { return 0; }
