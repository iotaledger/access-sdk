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

#include "tcpip.h"
#include "auth.h"
#include "auth_logger.h"

#include "apiorig.h"

//////////////////////////////////////////
// Macros and defines
//////////////////////////////////////////

/* AUTH_ERRORS */
#define AUTH_TYPE_CLIENT (0)
#define AUTH_TYPE_SERVER (1)

//////////////////////////////////////////
// Structure definitions
//////////////////////////////////////////

struct auth_struct {
  /*@{*/
  int     type; /**< server or client */
  uint8_t ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES]; /**< ed25519 public key */
  uint8_t x25519_pk[crypto_scalarmult_curve25519_BYTES]; /**< DH x25519 public key */
  uint8_t nonce[crypto_box_NONCEBYTES]; /**< DH nonce */
  uint8_t peer_ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES]; /**< peer's ed25519 public key */
  uint8_t peer_x25519_pk[crypto_scalarmult_curve25519_BYTES]; /**< peer's DH x25519 public key */
  /*@}*/
};

//////////////////////////////////////////
// Function declarations and definitions
//////////////////////////////////////////
/**
 * @fn      int auth_internal_server_authenticate(auth_ctx_t *session)
 *
 * @brief   Function that executes steps of server side AUTH authentication.
 *
 * @param   session         Server session.
 * @param   ed25519_sk[]    ed25519 secret key.
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_server_authenticate(auth_ctx_t *session, uint8_t ed25519_sk[]);
/**
 * @fn      int auth_internal_client_authenticate(auth_ctx_t *session)
 *
 * @brief   Function that executes steps of client side AUTH authentication.
 *
 * @param   session    Client session.
 * @param   ed25519_sk[]    ed25519 secret key.
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_internal_client_authenticate(auth_ctx_t *session, uint8_t ed25519_sk[]);

uint8_t auth_internal_encrypt(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *cipher, const uint8_t *m);

uint8_t auth_internal_decrypt(auth_ctx_t*session, uint8_t ed25519_sk[], uint8_t *m, const uint8_t *cipher);

uint8_t auth_internal_sign(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *sm, size_t *smlen, uint8_t *m, size_t mlen);

uint8_t auth_internal_verify(auth_ctx_t *session, uint8_t *m, size_t *mlen, uint8_t *sm, size_t smlen);

uint8_t auth_internal_send(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *m, size_t mlen);

uint8_t auth_internal_receive(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *m, size_t mlen);

#endif // AUTH_INTERNAL_H_