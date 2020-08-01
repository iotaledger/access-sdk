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
 * Client side implemntation for ssl based authentication module
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

//////////////////////////////////////////
// Macros and defines
//////////////////////////////////////////

/* AUTH_STAGES */
#define AUTH_ERROR 0
#define AUTH_INIT 1
#define AUTH_GENERATE 2
#define AUTH_VERIFY 3
#define AUTH_FINISH 4
#define AUTH_DONE 5

//////////////////////////////////////////
// Function declarations and definitions
//////////////////////////////////////////

/*
 * Stage 0 - Client initialization
 *
 */

static int auth_client_init(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;
  BIGNUM *e = BN_new();
  debug("auth_client_init START");

  /* Generate RSA key */
  AUTH_GET_INTERNAL_RSA_C(session) = RSA_new();
  BN_set_word(e, RSA_F4);
  RSA_generate_key_ex(AUTH_GET_INTERNAL_RSA_C(session), AUTH_RSA_KEY_LEN, e, NULL);

  AUTH_GET_INTERNAL_DH(session) = DH_new();

  auth_utils_randmem(AUTH_GET_INTERNAL_VC(session), AUTH_V_STRING_LEN);

  AUTH_GET_INTERNAL_OUT_PACKET_COUNT(session) = 1;
  AUTH_GET_INTERNAL_IN_PACKET_COUNT(session) = 1;

  memset(AUTH_GET_INTERNAL_H(session), 0, RSA_DIGEST_LENGTH);
  memset(AUTH_GET_INTERNAL_HC(session), 0, RSA_DIGEST_LENGTH);

  next_stage = AUTH_GENERATE;

  debug("auth_client_init END");

  return next_stage;
}

/*
 * Stage 1 - DH parameter generation
 *
 * Client generates p, g, vc and x.
 * Client calculates e = gx mod p.
 * Client sends ( p || g || e || vc ) to Server.
 *
 */

static inline int auth_client_generate_send(auth_ctx_t *session) {
  int ret = AUTH_ERROR;
  const BIGNUM *p = NULL, *g = NULL, *pub_key = NULL;

  DH_get0_pqg(AUTH_GET_INTERNAL_DH(session), &p, NULL, &g);
  DH_get0_key(AUTH_GET_INTERNAL_DH(session), &pub_key, NULL);

  debug("send p");
  ret = auth_utils_send_message_part_bignum(session, p);
  // TODO: Check return

  debug("send g");
  ret = auth_utils_send_message_part_bignum(session, g);
  // TODO: Check return

  debug("send e");
  ret = auth_utils_send_message_part_bignum(session, pub_key);
  // TODO: Check return

  debug("send vc");
  ret = auth_utils_send_message_part(session, AUTH_GET_INTERNAL_VC(session), AUTH_V_STRING_LEN);
  // TODO: Check return

  return ret;
}

static int auth_client_generate(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;
  BIGNUM *pp, *gg;
  int ret = AUTH_ERROR;

  debug("auth_client_generate START");

  if (NULL != AUTH_GET_INTERNAL_DH(session)) /* DH parameters generated */
  {
    /* Client generates p, g, vc and x. */
    gg = BN_new();
    if (gg != NULL) {
      BN_set_word(gg, 5);
    }

    pp = BN_get_rfc3526_prime_2048(NULL);
    if (pp == NULL || gg == NULL || !DH_set0_pqg(AUTH_GET_INTERNAL_DH(session), pp, NULL, gg)) {
      DH_free(AUTH_GET_INTERNAL_DH(session));
      BN_free(pp);
      BN_free(gg);
    } else {
      /* Client calculates e = gx mod p. */
      int ret_ossl = DH_generate_key(AUTH_GET_INTERNAL_DH(session));

      if (0 != ret_ossl) /* Keys generated */
      {
        int len = DH_size(AUTH_GET_INTERNAL_DH(session));
        AUTH_GET_INTERNAL_K(session) = malloc(len);

        if (NULL != AUTH_GET_INTERNAL_K(session)) {
          /* Send ( p || g || e || vc ) */
          ret = auth_client_generate_send(session);
          // TODO: Check return

          if (AUTH_ERROR != ret) {
            next_stage = AUTH_VERIFY;
          }

          next_stage = AUTH_VERIFY;
        }
      }
    }
  }

  debug("auth_client_generate END");

  return next_stage;
}

/*
 * Stage 2 - Compute and verify Server parameters
 *
 * Client receives ( f || s || ks || vs )
 * Client computes k = fx mod p
 * Client computes h  = hash( vc || vs || ks || e || f || k )
 * Client verifies the signature s on h
 * Client computes hc = hash( vc || vs || kc || e || f || k )
 * Client computes signature sc = sign( skc, hc )
 * Client sends ( kc || sc ) to Server
 *
 */

static inline int auth_client_verify_receive(auth_ctx_t *session, unsigned char **f, unsigned short *cf_len,
                                             unsigned char **s, unsigned short *cs_len, unsigned char **ks,
                                             unsigned short *ks_len, unsigned char **vs, unsigned short *Vs_len) {
  int ret = AUTH_ERROR;

  ret = auth_utils_receive_message_part(session, f, cf_len);
  // TODO: Check return

  ret = auth_utils_receive_message_part(session, s, cs_len);
  // TODO: Check return

  ret = auth_utils_receive_message_part(session, ks, ks_len);
  // TODO: Check return

  ret = auth_utils_receive_message_part(session, vs, Vs_len);
  // TODO: Check return

  return ret;
}

static inline int auth_client_verify_send(auth_ctx_t *session, unsigned char *kc, unsigned short kc_len,
                                          unsigned char *sc, unsigned short len) {
  int ret = AUTH_ERROR;

  debug("send kc");
  ret = auth_utils_send_message_part(session, kc, kc_len);
  // TODO: Check return

  debug("send sc");
  ret = auth_utils_send_message_part(session, sc, len);
  // TODO: Check return

  return ret;
}

static int auth_client_verify(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;
  int ret;

  unsigned char *kc = NULL, *cKs = NULL, *s = NULL, *cVs = NULL, *cf = NULL;
  unsigned short f_len = 0, cVs_len = 0, s_len = 0, cKs_len = 0;
  const BIGNUM *cpub_key = NULL;
  unsigned int sc_len = 0;
  unsigned char sc[AUTH_RSA_SIGN_LEN] = {
      0,
  };
  int kc_len = 0;

  debug("auth_client_verify START");

  // Receive ( f || s || ks || vs )
  ret = auth_client_verify_receive(session, &cf, &f_len, &s, &s_len, &cKs, &cKs_len, &cVs, &cVs_len);
  // TODO: Check return

  memcpy(AUTH_GET_INTERNAL_VS(session), cVs, cVs_len);

  AUTH_GET_INTERNAL_PUB_K(session) = BN_bin2bn(cf, f_len, NULL);

  /* Server computes k = fx mod p */
  AUTH_GET_INTERNAL_K_LEN(session) =
      DH_compute_key(AUTH_GET_INTERNAL_K(session), AUTH_GET_INTERNAL_PUB_K(session), AUTH_GET_INTERNAL_DH(session));

  /* Client checks Server key */
  ret = session->f_verify(cKs, cKs_len);
  // TODO: Check return

  DH_get0_key(AUTH_GET_INTERNAL_DH(session), &cpub_key, NULL);
  /* Client computes h  = hash( vc || vs || ks || e || f || k ) */
  ret = auth_utils_compute_hash(session, AUTH_GET_INTERNAL_H(session), cKs, cKs_len, cpub_key,
                              AUTH_GET_INTERNAL_PUB_K(session));

  /* Client verifies the signature s on h */
  AUTH_GET_INTERNAL_RSA_S(session) = d2i_RSAPublicKey(NULL, (const unsigned char **)&cKs, cKs_len);
  ret = RSA_verify(/*NID_sha256*/ 0, AUTH_GET_INTERNAL_H(session), RSA_DIGEST_LENGTH, s, s_len,
                   AUTH_GET_INTERNAL_RSA_S(session));
  debug("!!!!!!!!!!!!!!!!!!!!!!!!!!!RSA_verify (%d)", ret);

  /* Client computes hc = hash( vc || vs || kc || e || f || k ) */
  kc_len = i2d_RSAPublicKey(AUTH_GET_INTERNAL_RSA_C(session), &kc);
  ret = auth_utils_compute_hash(session, AUTH_GET_INTERNAL_HC(session), kc, kc_len, cpub_key,
                              AUTH_GET_INTERNAL_PUB_K(session));

  /* Client computes signature sc = sign( skc, hc ) */
  /*NID_sha256*/
  RSA_sign(/*NID_sha256*/ 0, AUTH_GET_INTERNAL_HC(session), RSA_DIGEST_LENGTH, sc, &sc_len,
           AUTH_GET_INTERNAL_RSA_C(session));
  auth_utils_debug_binary("RSA_Client_sign", sc, sc_len);

  // Send ( kc || sc )
  ret = auth_client_verify_send(session, kc, kc_len, sc, sc_len);
  // TODO: Check return

  free(kc);
  free(s);
  free(cVs);
  free(cf);

  next_stage = AUTH_FINISH;

  debug("auth_client_verify END");

  return next_stage;
}

/*
 * Stage 3 - Finish authentication
 *
 * Client generates AES keys.
 * Waits for encryption / decryption tasks :)
 *
 */

static int auth_client_finish(auth_ctx_t *session) {
  int next_stage = AUTH_ERROR;

  debug("auth_client_finish START");

  auth_utils_debug_binary("k", AUTH_GET_INTERNAL_K(session), AUTH_GET_INTERNAL_K_LEN(session));
  auth_utils_debug_binary("h", AUTH_GET_INTERNAL_H(session), RSA_DIGEST_LENGTH);
  auth_utils_debug_binary("hc", AUTH_GET_INTERNAL_HC(session), RSA_DIGEST_LENGTH);

  // Generate AES, HMAC keys
  auth_utils_generate_keys(session);

  AUTH_GET_INTERNAL_KEY_CS(session) = malloc(sizeof(AES_KEY));
  AUTH_GET_INTERNAL_KEY_SC(session) = malloc(sizeof(AES_KEY));

  memset(AUTH_GET_INTERNAL_KEY_CS(session), 0, sizeof(AES_KEY));
  memset(AUTH_GET_INTERNAL_KEY_SC(session), 0, sizeof(AES_KEY));

  // Initialize Client -> Server Encrypt Key
  AES_set_encrypt_key(AUTH_GET_INTERNAL_EKEY_C(session), AUTH_AES_KEY_LEN, AUTH_GET_INTERNAL_KEY_CS(session));

  // Initialize Server -> Client Decrypt Key
  AES_set_decrypt_key(AUTH_GET_INTERNAL_EKEY_S(session), AUTH_AES_KEY_LEN, AUTH_GET_INTERNAL_KEY_SC(session));

  {
    const BIGNUM *priv_key = NULL;
    unsigned char *kc = NULL;
    int kc_len = 0;
    int len = 0;

    DH_get0_key(AUTH_GET_INTERNAL_DH(session), NULL, &priv_key);

    len = BN_num_bytes(priv_key);

    unsigned char *data = malloc(len);

    len = BN_bn2bin(priv_key, data);

    auth_utils_debug_binary("x", data, len);

    kc_len = i2d_RSAPrivateKey(AUTH_GET_INTERNAL_RSA_C(session), &kc);

    auth_utils_debug_binary("kc", kc, kc_len);
  }

  next_stage = AUTH_DONE;

  debug("auth_client_finish END");

  return next_stage;
}

/*
 *
 */

int auth_internal_client_authenticate(auth_ctx_t *session) {
  int ret = AUTH_ERROR;

  int auth_stage = AUTH_INIT;

  debug("auth_internal_client_authenticate START");

  while ((AUTH_DONE != auth_stage) && (AUTH_ERROR != auth_stage)) {
    switch (auth_stage) {
      case AUTH_INIT:
        auth_stage = auth_client_init(session);
        break;
      case AUTH_GENERATE:
        auth_stage = auth_client_generate(session);
        break;
      case AUTH_VERIFY:
        auth_stage = auth_client_verify(session);
        break;
      case AUTH_FINISH:
        auth_stage = auth_client_finish(session);
        break;
      default:
        break;
    }
  }

  if (AUTH_DONE == auth_stage) {
    ret = AUTH_OK;
  }

  debug("auth_internal_client_authenticate END");

  return ret;
}

int auth_internal_client_send(auth_ctx_t *session, const unsigned char *data, unsigned short data_len) {
  return auth_utils_send(session, AUTH_GET_INTERNAL_IKEY_C(session), AUTH_GET_INTERNAL_KEY_CS(session),
                       AUTH_GET_INTERNAL_IV_C(session), data, data_len);
}

int auth_internal_client_receive(auth_ctx_t *session, unsigned char **data, unsigned short *data_len) {
  return auth_utils_receive(session, AUTH_GET_INTERNAL_IKEY_S(session), AUTH_GET_INTERNAL_KEY_SC(session),
                          AUTH_GET_INTERNAL_IV_S(session), data, data_len);
}

void auth_internal_release_client(auth_ctx_t *session) {}

int auth_internal_client_set_option(auth_ctx_t *session, const char *key, unsigned char *value) { return 0; }
