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

#include "sodium.h"

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

//////////////////////////////////////////
// Structure definitions
//////////////////////////////////////////

struct auth_struct {
  int type; /* server or client */

  uint8_t ed25519_pk[crypto_sign_PUBLICKEYBYTES];
  uint8_t ed25519_sk[crypto_sign_SECRETKEYBYTES];


  uint8_t x25519_pk[crypto_scalarmult_curve25519_BYTES];
  uint8_t x25519_sk[crypto_scalarmult_curve25519_BYTES];

  uint8_t nonce[crypto_box_NONCEBYTES];
  uint8_t identification_v[IDENTIFICATION_STRING_L];

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
int auth_internal_server_authenticate(auth_ctx_t *session, uint8_t sk[]);
/**
 * @fn      int auth_internal_client_authenticate(auth_ctx_t *session)
 *
 * @brief   Function that executes steps of client side AUTH authentication.
 *
 * @param   session    Client session.
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_client_authenticate(auth_ctx_t *session, uint8_t sk[]);
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
