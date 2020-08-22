/*
 * This file is part of the Frost distribution
 * (https://github.com/xainag/frost)
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
 * Header file for EdDSA based authentication module, internal functions
 *
 * @Author Nikola Kuzmanovic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 14.08.2018. Initial version.
 * 06.12.2018. Implemented ed25519 signature algorithm
 * 01.08.2020. Renaming.
 ****************************************************************************/
#ifndef AUTH_INTERNAL_H_
#define AUTH_INTERNAL_H_

#include <string.h>

#include "aes.h"
#include "curve25519-donna.h"

#include "sha256.h"

#include "auth.h"

#include "apiorig.h"

//////////////////////////////////////////
// Macros and defines
//////////////////////////////////////////

/* AUTH_ERRORS */
#define AUTH_TYPE_CLIENT (0)
#define AUTH_TYPE_SERVER (1)

#define DH_PUBLIC_L 32
#define DH_PRIVATE_L 32
#define DH_SHARED_SECRET_L 32
#define EXCHANGE_HASH_L 32
#define IDENTIFICATION_STRING_L 6
#define PUBLIC_KEY_L 32
#define PRIVATE_KEY_L 64
#define IV_KEY_L 16
#define ENCRYPTION_KEY_L 32
#define INTEGRITY_KEY_L 32
#define SIGNED_MESSAGE_L 96
#define MAC_HASH_L 32
#define HASH_OUTPUT_L 32

#define I_PAD (0x36)
#define O_PAD (0x5C)
#define SHA256_BLOCK_BYTES 64

#define AUTH_GET_INTERNAL(s) ((s)->internal)
#define AUTH_GET_INTERNAL_TYPE(s) (AUTH_GET_INTERNAL(s)->type)
#define AUTH_GET_INTERNAL_DH_PUBLIC(s) (AUTH_GET_INTERNAL(s)->dh_public)
#define AUTH_GET_INTERNAL_DH_PRIVATE(s) (AUTH_GET_INTERNAL(s)->dh_private)
#define AUTH_GET_INTERNAL_PUBLIC_KEY(s) (AUTH_GET_INTERNAL(s)->public_key)
#define AUTH_GET_INTERNAL_PRIVATE_KEY(s) (AUTH_GET_INTERNAL(s)->private_key)
#define AUTH_GET_INTERNAL_EXCHANGE_HASH(s) (AUTH_GET_INTERNAL(s)->exchange_hash)
#define AUTH_GET_INTERNAL_EXCHANGE_HASH2(s) (AUTH_GET_INTERNAL(s)->exchange_hash2)
#define AUTH_GET_INTERNAL_SECREY_K(s) (AUTH_GET_INTERNAL(s)->secret_k)
#define AUTH_GET_INTERNAL_ID_V(s) (AUTH_GET_INTERNAL(s)->identification_v)
#define AUTH_GET_INTERNAL_SEQ_NUM_DECRYPT(s) (AUTH_GET_INTERNAL(s)->sequence_number_encrypt)
#define AUTH_GET_INTERNAL_SEQ_NUM_ENCRYPT(s) (AUTH_GET_INTERNAL(s)->sequence_number_decrypt)
#define AUTH_GET_INTERNAL_CTX_ENCRYPT(s) (AUTH_GET_INTERNAL(s)->ctx_encrypt)
#define AUTH_GET_INTERNAL_CTX_DECRYPT(s) (AUTH_GET_INTERNAL(s)->ctx_decrypt)

#define AUTH_GET_INTERNAL_IV_ENCRYPTION(s) (AUTH_GET_INTERNAL(s)->keys.iv_encryption)
#define AUTH_GET_INTERNAL_IV_DECRYPTION(s) (AUTH_GET_INTERNAL(s)->keys.iv_decryption)
#define AUTH_GET_INTERNAL_ENCRYPTION_KEY(s) (AUTH_GET_INTERNAL(s)->keys.encryption_key)
#define AUTH_GET_INTERNAL_DECRYPTION_KEY(s) (AUTH_GET_INTERNAL(s)->keys.decryption_key)
#define AUTH_GET_INTERNAL_INTEGRITY_KEY_ENCRYPTION(s) (AUTH_GET_INTERNAL(s)->keys.integrity_key_encryption)
#define AUTH_GET_INTERNAL_INTEGRITY_KEY_DECRYPTION(s) (AUTH_GET_INTERNAL(s)->keys.integrity_key_decryption)

//////////////////////////////////////////
// Structure definitions
//////////////////////////////////////////

typedef struct ea_keys {
  unsigned char iv_encryption[IV_KEY_L];
  unsigned char iv_decryption[IV_KEY_L];
  unsigned char encryption_key[ENCRYPTION_KEY_L];
  unsigned char decryption_key[ENCRYPTION_KEY_L];
  unsigned char integrity_key_encryption[INTEGRITY_KEY_L];
  unsigned char integrity_key_decryption[INTEGRITY_KEY_L];
} ea_keys_t;

struct auth_struct {
  int type; /*AUTH_TYPE_X*/

  unsigned char dh_public[DH_PUBLIC_L];
  unsigned char dh_private[DH_PRIVATE_L];

  unsigned char secret_k[DH_SHARED_SECRET_L];

  unsigned char exchange_hash[EXCHANGE_HASH_L];
  unsigned char exchange_hash2[EXCHANGE_HASH_L];

  unsigned char identification_v[IDENTIFICATION_STRING_L];

  unsigned char public_key[PUBLIC_KEY_L];
  unsigned char private_key[PRIVATE_KEY_L];

  AES_ctx_t ctx_encrypt;
  AES_ctx_t ctx_decrypt;

  ea_keys_t keys;

  unsigned char sequence_number_encrypt;
  unsigned char sequence_number_decrypt;
};

//////////////////////////////////////////
// Function declarations and definitions
//////////////////////////////////////////
/**
 * @fn      int auth_internal_server_authenticate(auth_ctx_t *session)
 *
 * @brief   Function that executes steps of server side AUTH authentication.
 *
 * @param   session    Server session.
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_server_authenticate(auth_ctx_t *session);
/**
 * @fn      int auth_internal_client_authenticate(auth_ctx_t *session)
 *
 * @brief   Function that executes steps of client side AUTH authentication.
 *
 * @param   session    Client session.
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_client_authenticate(auth_ctx_t *session, unsigned char sk[]);
/**
 * @fn      int auth_internal_server_send(auth_ctx_t *session, const unsigned
 * char *msg, unsigned short msg_length)
 *
 * @brief   Function that is used to send message from server to client.
 *
 * @param   session     Server session
 * @param   msg         Message to send
 * @param   msg_length  Message length
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_server_send(auth_ctx_t *session, const unsigned char *msg, unsigned short msg_length);
/**
 * @fn      int auth_internal_client_send(auth_ctx_t *session, const unsigned
 * char *data, unsigned short  data_len)
 *
 * @brief   Function that is used to send message from client to server.
 *
 * @param   session     Client session
 * @param   data_len    Message to send
 * @param   data_len    Message length
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_client_send(auth_ctx_t *session, const unsigned char *data, unsigned short data_len);
/**
 * @fn      int auth_internal_server_receive(auth_ctx_t *session, unsigned char
 * **msg, unsigned short  *msg_length)
 *
 * @brief   Function that is used to receive and read message from client.
 *
 * @param   session     Server session
 * @param   msg         Address of buffer for received message
 * @param   msg_length  Received message length
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_server_receive(auth_ctx_t *session, unsigned char **msg, unsigned short *msg_length);
/**
 * @fn      int auth_internal_client_receive(auth_ctx_t *session, unsigned char
 * **msg, unsigned short  *msg_length)
 *
 * @brief   Function that is used to receive and read message from server.
 *
 * @param   session     Client session
 * @param   msg         Address of buffer for received message
 * @param   msg_length  Received message length
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_client_receive(auth_ctx_t *session, unsigned char **msg, unsigned short *msg_length);

/**
 * @fn      void auth_internal_release_server(auth_ctx_t *session)
 *
 * @brief   Function that releases memory alocated for server session.
 *
 * @param   session     Server session
 */
void auth_internal_release_server(auth_ctx_t *session);
/**
 * @fn      void auth_internal_release_client(auth_ctx_t *session)
 *
 * @brief   Function that releases memory alocated for client session.
 *
 * @param   session     Client session
 */
void auth_internal_release_client(auth_ctx_t *session);

/**
 * @fn      /int auth_internal_server_set_option(auth_ctx_t *, const char *,
 * unsigned char *)
 *
 * @brief   Function that releases memory alocated for server session.
 *
 * @param   session     Server session
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_server_set_option(auth_ctx_t *, const char *, unsigned char *);

/**
 * @fn      int auth_internal_client_set_option(auth_ctx_t *, const char *,
 * unsigned char *)
 *
 * @brief   Function that releases memory alocated for client session.
 *
 * @param   session     Client session
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_client_set_option(auth_ctx_t *, const char *, unsigned char *);

#endif /* AUTH_INTERNAL_H_ */
