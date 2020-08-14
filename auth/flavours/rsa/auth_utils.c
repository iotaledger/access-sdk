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
 * \file auth_utils.c
 * \brief
 * Implementation of RSA key exchange and encryption key computation
 *
 * @Author Djordje Golubovic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 05.05.2020. Initial version.
 * 01.08.2020. Renaming.
 ****************************************************************************/

#include "auth_utils.h"
#include "auth_internal.h"

#if ((__BYTE_ORDER__) == (__ORDER_BIG_ENDIAN__))

#define htons(A) (A)
#define htonl(A) (A)
#define ntohs(A) (A)
#define ntohl(A) (A)

#elif ((__BYTE_ORDER__) == (__ORDER_LITTLE_ENDIAN__))

#define htons(A) ((((unsigned short)(A)&0xff00) >> 8) | (((unsigned short)(A)&0x00ff) << 8))
#define htonl(A)                                                                    \
  ((((unsigned int)(A)&0xff000000) >> 24) | (((unsigned int)(A)&0x00ff0000) >> 8) | \
   (((unsigned int)(A)&0x0000ff00) << 8) | (((unsigned int)(A)&0x000000ff) << 24))
#define ntohs htons
#define ntohl htohl

#else

#error "Either BIG_ENDIAN or LITTLE_ENDIAN must be #defined, but not both."

#endif

#if ((__BYTE_ORDER__) == (__ORDER_LITTLE_ENDIAN__))

#endif

void auth_utils_debug_binary(char *name, unsigned char *data, int len) {
#ifndef NDEBUG
  int i;
  printf("\n%s (%d) = ", name, len);
  for (i = 0; i < len; i++) {
    printf("%02X", data[i]);
  }
  printf("\n\n");
#endif
}

static unsigned char count_one(unsigned char number) { return (number + 1) ? (number + 1) : 1; }

void auth_utils_randmem(unsigned char *random_mem, int length) {
  if (NULL != random_mem) {
    if (0 < length) {
      int n = 0;
      for (; n < length; n++) {
        random_mem[n] = (unsigned char)rand();
      }
    }
  }
}

static int auth_utils_write(auth_ctx_t *session, void *data, unsigned short data_len) {
  int ret = AUTH_ERROR;
  int i = 0;

  while (i < data_len) {
    int len = session->f_write(session->ext, data + i, data_len - i);
    // TODO: Check return
    if (0 >= len) {
      break;
    }

    i += len;
  }

  return ret;
}

static int auth_utils_read(auth_ctx_t *session, void *data, unsigned short data_len) {
  int ret = AUTH_ERROR;
  int i = 0;

  while (i < data_len) {
    int len = session->f_read(session->ext, data + i, data_len - i);
    // TODO: Check return
    if (0 >= len) {
      break;
    }

    i += len;
  }

  return ret;
}

int auth_utils_send_message_part(auth_ctx_t *session, void *data, unsigned short data_len) {
  int ret = AUTH_ERROR;

  if (NULL != data) {
    unsigned short len = htons(data_len);

    ret = auth_utils_write(session, &len, sizeof(unsigned short));
    // TODO: Check return

    ret = auth_utils_write(session, data, data_len);
    // TODO: Check return
  }

  return ret;
}

int auth_utils_send_message_part_bignum(auth_ctx_t *session, const BIGNUM *bn) {
  int ret = AUTH_ERROR;
  int len = BN_num_bytes(bn);

  unsigned char *data = malloc(len);
  len = BN_bn2bin(bn, data);

  auth_utils_debug_binary("BN", data, len);

  ret = auth_utils_send_message_part(session, data, len);
  // TODO: Check return

  free(data);

  return ret;
}

int auth_utils_receive_message_part(auth_ctx_t *session, unsigned char **data, unsigned short *data_len) {
  int ret = AUTH_ERROR;

  if ((NULL != data) && (NULL != data_len)) {
    ret = auth_utils_read(session, data_len, sizeof(unsigned short));
    // TODO: Check return

    *data_len = ntohs(*data_len);

    *data = malloc(*data_len);

    ret = auth_utils_read(session, *data, *data_len);
    // TODO: Check return
  }

  return ret;
}

int auth_utils_compute_hash(auth_ctx_t *session, unsigned char *md, unsigned char *pkey, int pkey_len, const BIGNUM *e,
                          const BIGNUM *pub_key) {
  SHA256_CTX c;
  int ret = AUTH_ERROR;
  int len_bn = 0;
  unsigned char *data = NULL;

  SHA256_Init(&c);

  /* computes h  = hash( vc || vs || ks or kc || e || f || k ) */

  SHA256_Update(&c, AUTH_GET_INTERNAL_VC(session), AUTH_V_STRING_LEN);

  SHA256_Update(&c, AUTH_GET_INTERNAL_VS(session), AUTH_V_STRING_LEN);

  SHA256_Update(&c, pkey, pkey_len);

  len_bn = BN_num_bytes(e);
  data = malloc(len_bn);
  len_bn = BN_bn2bin(e, data);
  SHA256_Update(&c, data, len_bn);
  free(data);

  len_bn = BN_num_bytes(pub_key);
  data = malloc(len_bn);
  len_bn = BN_bn2bin(pub_key, data);
  SHA256_Update(&c, data, len_bn);
  free(data);

  SHA256_Update(&c, AUTH_GET_INTERNAL_K(session), AUTH_GET_INTERNAL_K_LEN(session));

  SHA256_Final(md, &c);

  OPENSSL_cleanse(&c, sizeof(c));

  return ret;
}

static void compute_hash_key(auth_ctx_t *session, unsigned char *md, char *end) {
  SHA256_CTX c;

  SHA256_Init(&c);

  /* computes h  = hash( vc || vs || ks or kc || e || f || k ) */

  SHA256_Update(&c, AUTH_GET_INTERNAL_K(session), AUTH_GET_INTERNAL_K_LEN(session));

  SHA256_Update(&c, AUTH_GET_INTERNAL_H(session), SHA256_DIGEST_LENGTH);

  SHA256_Update(&c, end, strlen(end));

  SHA256_Final(md, &c);

  OPENSSL_cleanse(&c, sizeof(c));
}

void auth_utils_generate_keys(auth_ctx_t *session) {
  compute_hash_key(session, AUTH_GET_INTERNAL_IV_S(session), AUTH_HASH_A);
  auth_utils_debug_binary("IV_S", AUTH_GET_INTERNAL_IV_S(session), SHA256_DIGEST_LENGTH);

  compute_hash_key(session, AUTH_GET_INTERNAL_IV_C(session), AUTH_HASH_B);
  auth_utils_debug_binary("IV_C", AUTH_GET_INTERNAL_IV_C(session), SHA256_DIGEST_LENGTH);

  compute_hash_key(session, AUTH_GET_INTERNAL_EKEY_S(session), AUTH_HASH_C);
  auth_utils_debug_binary("EKey_S", AUTH_GET_INTERNAL_EKEY_S(session), SHA256_DIGEST_LENGTH);

  compute_hash_key(session, AUTH_GET_INTERNAL_EKEY_C(session), AUTH_HASH_D);
  auth_utils_debug_binary("EKey_C", AUTH_GET_INTERNAL_EKEY_C(session), SHA256_DIGEST_LENGTH);

  compute_hash_key(session, AUTH_GET_INTERNAL_IKEY_S(session), AUTH_HASH_E);
  auth_utils_debug_binary("IKey_S", AUTH_GET_INTERNAL_IKEY_S(session), SHA256_DIGEST_LENGTH);

  compute_hash_key(session, AUTH_GET_INTERNAL_IKEY_C(session), AUTH_HASH_F);
  auth_utils_debug_binary("IKey_C", AUTH_GET_INTERNAL_IKEY_C(session), SHA256_DIGEST_LENGTH);
}

/* Setup packet
 *    bytes       2         N          X        32
 *          ( pack_len || payload || var_pad || mac )
 */
static unsigned char *auth_utils_setup_out_packet(const void *data, unsigned short data_length, unsigned short *out_len,
                                                int *padding_len) {
  unsigned char *ret_data = NULL;
  unsigned short data_len = htons(data_length);
  *padding_len = (AUTH_AES_DIGEST_LEN - ((data_len + AUTH_MESSAGE_NUMBER_LEN) % AUTH_AES_DIGEST_LEN)) % AUTH_AES_DIGEST_LEN;

  *out_len = AUTH_MESSAGE_NUMBER_LEN + data_len + *padding_len + HMAC_DIGEST_LENGTH;

  ret_data = malloc(*out_len);

  memcpy(ret_data, &data_len, AUTH_MESSAGE_NUMBER_LEN);

  memcpy(ret_data + AUTH_MESSAGE_NUMBER_LEN, data, data_len);

  auth_utils_randmem(ret_data + (AUTH_MESSAGE_NUMBER_LEN + data_len), *padding_len);

  return ret_data;
}

static inline int auth_utils_hmac_update(HMAC_CTX *hmac_ctx, unsigned char **hmac_data, int *hmac_done, int hmac_todo) {
  int ret = 0;
  ret = HMAC_Update(hmac_ctx, *hmac_data, hmac_todo);
  *hmac_done += hmac_todo;
  *hmac_data += hmac_todo;

  return ret;
}

int auth_utils_send(auth_ctx_t *session, const void *hmacKey, AES_KEY *aes_key, unsigned char *iv,
                  const unsigned char *data, unsigned short data_len) {
  int ret = AUTH_ERROR;
  unsigned short setup_len = 0, send_len = 0;
  int padding_len = 0;
  unsigned int hmac_len = 0;
  HMAC_CTX *hmac_ctx = HMAC_CTX_new();

  unsigned char *out_data = auth_utils_setup_out_packet(data, data_len, &setup_len, &padding_len);
  unsigned char *hmac_data = out_data;
  int hmac_done = 0;

  debug("data_len (%d), setup_len (%d), padding_len (%d)", data_len, setup_len, padding_len);

  HMAC_Init_ex(hmac_ctx, hmacKey, SHA256_DIGEST_LENGTH, EVP_sha256(), NULL);

  HMAC_Update(hmac_ctx, &AUTH_GET_INTERNAL_OUT_PACKET_COUNT(session),
              sizeof(AUTH_GET_INTERNAL_OUT_PACKET_COUNT(session)));

  send_len = htons(setup_len);

  HMAC_Update(hmac_ctx, (unsigned char *)&send_len, sizeof(unsigned short));

  auth_utils_debug_binary("outP", out_data, setup_len - HMAC_DIGEST_LENGTH);

  CRYPTO_cbc128_encrypt(out_data, out_data, setup_len - HMAC_DIGEST_LENGTH, aes_key, iv, (block128_f)AES_encrypt);

  auth_utils_debug_binary("outPenc", out_data, setup_len - HMAC_DIGEST_LENGTH);

  auth_utils_hmac_update(hmac_ctx, &hmac_data, &hmac_done, AUTH_AES_DIGEST_LEN);

  if (0 != padding_len) {
    auth_utils_hmac_update(hmac_ctx, &hmac_data, &hmac_done,
                         setup_len - HMAC_DIGEST_LENGTH - hmac_done - AUTH_AES_DIGEST_LEN);

    auth_utils_hmac_update(hmac_ctx, &hmac_data, &hmac_done, AUTH_AES_DIGEST_LEN);
  } else {
    auth_utils_hmac_update(hmac_ctx, &hmac_data, &hmac_done, setup_len - HMAC_DIGEST_LENGTH - hmac_done);
  }

  HMAC_Final(hmac_ctx, hmac_data, &hmac_len);

  debug("setup_len (%d) hmac_len (%d)", setup_len, hmac_len);

  auth_utils_debug_binary("hmac_data", hmac_data, hmac_len);

  ret = auth_utils_write(session, &send_len, sizeof(unsigned short));
  // TODO: Check return

  ret = auth_utils_write(session, out_data, setup_len);
  // TODO: Check return

  free(out_data);

  AUTH_GET_INTERNAL_OUT_PACKET_COUNT(session) = count_one(AUTH_GET_INTERNAL_OUT_PACKET_COUNT(session));

  return ret;
}

unsigned char *auth_utils_allocate_packet_len(unsigned char *digest, unsigned short send_len, unsigned short *payload_len,
                                            int *data_full, int *padding_len) {
  unsigned char *ret_data = NULL;

  *data_full = AUTH_AES_DIGEST_LEN - AUTH_MESSAGE_NUMBER_LEN;
  *payload_len = ntohs(*((unsigned short *)digest));

  if (send_len > *payload_len) {
    ret_data = malloc(*payload_len);

    memcpy(ret_data, digest + AUTH_MESSAGE_NUMBER_LEN, *data_full);

    *padding_len =
        (AUTH_AES_DIGEST_LEN - ((*payload_len + AUTH_MESSAGE_NUMBER_LEN) % AUTH_AES_DIGEST_LEN)) % AUTH_AES_DIGEST_LEN;
  }

  return ret_data;
}

int auth_utils_receive(auth_ctx_t *session, const void *hmacKey, AES_KEY *aes_key, unsigned char *iv, unsigned char **data,
                     unsigned short *data_len) {
  int ret = AUTH_ERROR;
  int read_len = 0;
  unsigned int hmac_len = 0;
  unsigned short send_len = 0;
  unsigned char digest_buffer[HMAC_DIGEST_LENGTH] = {
      0,
  };
  unsigned char hmac_buffer[HMAC_DIGEST_LENGTH] = {
      0,
  };
  int padding_len = 0, data_full = 0, payload_rest = 0;
  HMAC_CTX *hmac_ctx = HMAC_CTX_new();
  unsigned char *out_data = NULL;

  HMAC_Init_ex(hmac_ctx, hmacKey, SHA256_DIGEST_LENGTH, EVP_sha256(), NULL);

  HMAC_Update(hmac_ctx, &AUTH_GET_INTERNAL_IN_PACKET_COUNT(session), sizeof(AUTH_GET_INTERNAL_IN_PACKET_COUNT(session)));

  ret = auth_utils_read(session, &send_len, sizeof(unsigned short));
  // TODO: Check return

  HMAC_Update(hmac_ctx, (unsigned char *)&send_len, sizeof(unsigned short));

  send_len = ntohs(send_len);

  auth_utils_read(session, digest_buffer, AUTH_AES_DIGEST_LEN);

  HMAC_Update(hmac_ctx, digest_buffer, AUTH_AES_DIGEST_LEN);

  auth_utils_debug_binary("digest_buffer", digest_buffer, AUTH_AES_DIGEST_LEN);

  CRYPTO_cbc128_decrypt(digest_buffer, digest_buffer, AUTH_AES_DIGEST_LEN, aes_key, iv, (block128_f)AES_decrypt);

  auth_utils_debug_binary("digest_buffer", digest_buffer, AUTH_AES_DIGEST_LEN);

  out_data = auth_utils_allocate_packet_len(digest_buffer, send_len, data_len, &data_full, &padding_len);

  debug("data_len (%d), padding_len (%d)", *data_len, padding_len);

  if (0 != padding_len) {
    payload_rest = AUTH_AES_DIGEST_LEN - padding_len;
  }

  read_len = *data_len - data_full - payload_rest;

  debug("read_len (%d), payload_rest (%d)", read_len, payload_rest);

  auth_utils_read(session, out_data + data_full, read_len);

  HMAC_Update(hmac_ctx, out_data + data_full, read_len);

  auth_utils_debug_binary("out_data + data_full", out_data + data_full, read_len);

  CRYPTO_cbc128_decrypt(out_data + data_full, out_data + data_full, read_len, aes_key, iv, (block128_f)AES_decrypt);

  auth_utils_debug_binary("out_data + data_full", out_data + data_full, read_len);

  if (0 != padding_len) {
    auth_utils_read(session, digest_buffer, AUTH_AES_DIGEST_LEN);

    HMAC_Update(hmac_ctx, digest_buffer, AUTH_AES_DIGEST_LEN);

    auth_utils_debug_binary("digest_buffer", digest_buffer, AUTH_AES_DIGEST_LEN);

    CRYPTO_cbc128_decrypt(digest_buffer, digest_buffer, AUTH_AES_DIGEST_LEN, aes_key, iv, (block128_f)AES_decrypt);

    auth_utils_debug_binary("digest_buffer", digest_buffer, AUTH_AES_DIGEST_LEN);

    memcpy(out_data + *data_len - payload_rest, digest_buffer, payload_rest);

    debug("1 (%c), 2 (%c)", out_data[*data_len - 2], out_data[*data_len - 1]);
  }

  HMAC_Final(hmac_ctx, digest_buffer, &hmac_len);

  auth_utils_read(session, hmac_buffer, HMAC_DIGEST_LENGTH);

  ret = memcmp(hmac_buffer, digest_buffer, HMAC_DIGEST_LENGTH);

  auth_utils_debug_binary("hmac_data", digest_buffer, hmac_len);

  auth_utils_debug_binary("hmac_buffer", hmac_buffer, hmac_len);

  debug("memcmp (%d)", ret);

  AUTH_GET_INTERNAL_IN_PACKET_COUNT(session) = count_one(AUTH_GET_INTERNAL_IN_PACKET_COUNT(session));

  *data = out_data;

  return ret;
}
