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
 * \file auth_internal.h
 * \brief
 * Header file for ssl based authentication module, internal functions
 *
 * @Author Djordje Golubovic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 05.05.2020. Initial version.
 * 01.08.2020. Renaming.
 ****************************************************************************/

#ifndef AUTH_INTERNAL_H_
#define AUTH_INTERNAL_H_

#include <openssl/aes.h>
#include <openssl/bn.h>
#include <openssl/dh.h>
#include <openssl/hmac.h>
#include <openssl/modes.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

#include "auth.h"

//////////////////////////////////////////
// Macros and defines
//////////////////////////////////////////

/* AUTH_ERRORS */
#define AUTH_TYPE_CLIENT (0)
#define AUTH_TYPE_SERVER (1)

#define AUTH_MESSAGE_NUMBER_LEN (sizeof(unsigned short))
#define AUTH_MESSAGE_BASE_10 (10)

#define AUTH_HASH_A ("A")
#define AUTH_HASH_B ("B")
#define AUTH_HASH_C ("C")
#define AUTH_HASH_D ("D")
#define AUTH_HASH_E ("E")
#define AUTH_HASH_F ("F")

#define AUTH_GET_INTERNAL(s) ((s)->internal)
#define AUTH_GET_INTERNAL_TYPE(s) (AUTH_GET_INTERNAL(s)->type)
#define AUTH_GET_INTERNAL_DH(s) (AUTH_GET_INTERNAL(s)->auth_dh)
#define AUTH_GET_INTERNAL_PUB_K(s) (AUTH_GET_INTERNAL(s)->pub_key)
#define AUTH_GET_INTERNAL_VC(s) (AUTH_GET_INTERNAL(s)->vc)
#define AUTH_GET_INTERNAL_VS(s) (AUTH_GET_INTERNAL(s)->vs)
#define AUTH_GET_INTERNAL_K(s) (AUTH_GET_INTERNAL(s)->k)
#define AUTH_GET_INTERNAL_K_LEN(s) (AUTH_GET_INTERNAL(s)->k_len)
#define AUTH_GET_INTERNAL_H(s) (AUTH_GET_INTERNAL(s)->h)
#define AUTH_GET_INTERNAL_HC(s) (AUTH_GET_INTERNAL(s)->hc)
#define AUTH_GET_INTERNAL_IV_S(s) (AUTH_GET_INTERNAL(s)->iv_s)
#define AUTH_GET_INTERNAL_IV_C(s) (AUTH_GET_INTERNAL(s)->iv_c)
#define AUTH_GET_INTERNAL_EKEY_S(s) (AUTH_GET_INTERNAL(s)->ekey_s)
#define AUTH_GET_INTERNAL_EKEY_C(s) (AUTH_GET_INTERNAL(s)->ekey_c)
#define AUTH_GET_INTERNAL_IKEY_S(s) (AUTH_GET_INTERNAL(s)->i_key_s)
#define AUTH_GET_INTERNAL_IKEY_C(s) (AUTH_GET_INTERNAL(s)->i_key_c)
#define AUTH_GET_INTERNAL_KEY_CS(s) (AUTH_GET_INTERNAL(s)->key_client_server)
#define AUTH_GET_INTERNAL_KEY_SC(s) (AUTH_GET_INTERNAL(s)->key_server_client)
#define AUTH_GET_INTERNAL_RSA_S(s) (AUTH_GET_INTERNAL(s)->rsa_server)
#define AUTH_GET_INTERNAL_RSA_C(s) (AUTH_GET_INTERNAL(s)->rsa_client)
#define AUTH_GET_INTERNAL_IN_PACKET_COUNT(s) (AUTH_GET_INTERNAL(s)->out_packet_count)
#define AUTH_GET_INTERNAL_OUT_PACKET_COUNT(s) (AUTH_GET_INTERNAL(s)->in_packet_count)

#define AUTH_V_STRING_LEN (16)
#define AUTH_AES_DIGEST_LEN (16)
#define AUTH_AES_KEY_LEN (256)

#define AUTH_RSA_KEY_LEN (1024 * 2)
#define AUTH_RSA_SIGN_LEN (AUTH_RSA_KEY_LEN / 8)

#define HMAC_DIGEST_LENGTH (SHA256_DIGEST_LENGTH)

#define RSA_DIGEST_LENGTH (SHA256_DIGEST_LENGTH + 4)

struct auth_struct {
  int type; /*AUTH_TYPE_X*/

  unsigned char vc[AUTH_V_STRING_LEN];
  unsigned char vs[AUTH_V_STRING_LEN];

  /* DH parameters */
  DH *auth_dh;
  BIGNUM *pub_key;
  int k_len;
  unsigned char *k;

  /* SHA256 parameters */
  unsigned char h[RSA_DIGEST_LENGTH];
  unsigned char hc[RSA_DIGEST_LENGTH];

  unsigned char iv_s[SHA256_DIGEST_LENGTH];
  unsigned char iv_c[SHA256_DIGEST_LENGTH];

  unsigned char ekey_s[SHA256_DIGEST_LENGTH];
  unsigned char ekey_c[SHA256_DIGEST_LENGTH];

  unsigned char i_key_s[SHA256_DIGEST_LENGTH];
  unsigned char i_key_c[SHA256_DIGEST_LENGTH];

  /* RSA parameters */
  RSA *rsa_client;
  RSA *rsa_server;

  /* AES parameters */
  AES_KEY *key_client_server;
  AES_KEY *key_server_client;

  /* Packet counters */
  unsigned char out_packet_count;
  unsigned char in_packet_count;
};

int auth_internal_server_authenticate(auth_ctx_t *);

int auth_internal_client_authenticate(auth_ctx_t *);

int auth_internal_server_send(auth_ctx_t *, const unsigned char *, unsigned short);

int auth_internal_client_send(auth_ctx_t *, const unsigned char *, unsigned short);

int auth_internal_server_receive(auth_ctx_t *, unsigned char **, unsigned short *);

int auth_internal_client_receive(auth_ctx_t *, unsigned char **, unsigned short *);

void auth_internal_release_server(auth_ctx_t *);

void auth_internal_release_client(auth_ctx_t *);

int auth_internal_server_set_option(auth_ctx_t *, const char *, unsigned char *);

int auth_internal_client_set_option(auth_ctx_t *, const char *, unsigned char *);

#endif /* AUTH_INTERNAL_H_ */
