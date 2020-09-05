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
 * \file auth.h
 * \brief
 *    Authentication module API
 *
 * @Author Nikola Kuzmanovic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 31.07.2018. Initial version.
 * 01.08.2020. Renaming.
 ****************************************************************************/

#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief fixed message size (ToDo: https://github.com/iotaledger/access-sdk/issues/15)
 */
#define MSGLEN 1024

/**
 * @brief fixed cipher size (ToDo: https://github.com/iotaledger/access-sdk/issues/15)
 */
#define CIPHERLEN (crypto_box_MACBYTES + MSGLEN)

/**
 * @brief fixed signed cipher size (ToDo: https://github.com/iotaledger/access-sdk/issues/15)
 */
#define SIGCIPHERLEN (crypto_sign_BYTES + CIPHERLEN)

/**
 * @brief no authentication error
 */
#define AUTH_OK 0

/**
 * @brief authentication error
 */
#define AUTH_ERROR -1

/**
 * @brief declaration of auth_struct typedef.
 *
 * actual definition of this struct happens at auth/${AUTH_FLAVOUR}/auth_internal.h
 */
typedef struct auth_struct auth_struct_t;

/**
 * @struct auth_ctx_t
 * @brief structure to represent authentication sessions.
 */
typedef struct {
  /*@{*/
  uint8_t type;
  /*@}*/

  /*@{*/
  auth_struct_t *internal; /**< internal data (flavour dependent) */
    uint8_t sockfd; /**< socket file descriptor */
  /*@}*/

  /*@{*/
  uint8_t status; /**< status of authenticated session */
  /*@}*/
} auth_ctx_t;

/**
 * @brief authenticator client initializer function
 *
 * initializes the authenticator in client mode.
 *
 * @param session pointer to authenticator session context.
 * @param ext external data structure.
 *
 * @return AUTH_OK or AUTH_ERROR
 */
uint8_t auth_init_client(auth_ctx_t *session, uint8_t *sockfd);

/**
 * @brief authenticator server intiializer function
 *
 * initializes the authenticator in server mode.
 *
 * @param session pointer to authenticator session context.
 * @param ext external data structure.
 *
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_init_server(auth_ctx_t *session, uint8_t *sockfd);

/**
 * @brief performs actual authentication
 *
 * performs actual authentication, be it server or client side
 *
 * @param session pointer to authenticator session context.
 * @param sk array with secret key
 *
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_authenticate(auth_ctx_t *session, uint8_t sk[]);

/**
 * @brief generate cipher from message m
 * @param session pointer to authenticated session struct
 * @param sk array with secret key (ed25519 or RSA)
 * @param cipher array with cipher (output)
 * @param m pointer with message (input)
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_encrypt(auth_ctx_t *session, uint8_t sk[], uint8_t cipher[], const uint8_t *m);

/**
 * @brief generate message m from cipher
 * @param session pointer to authenticated session struct
 * @param sk array with secret key (ed25519 or RSA)
 * @param m pointer with message (output)
 * @param cipher array with cipher (input)
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_decrypt(auth_ctx_t *session, uint8_t sk[], uint8_t *m, const uint8_t *cipher);

/**
 * @brief sign message m
 * @param session pointer to authenticated session struct
 * @param sk array with secret key (ed25519 or RSA)
 * @param sm pointer with signed message (output)
 * @param smlen pointer with length of sm (output)
 * @param m pointer with message (input)
 * @param mlen pointer with length of m (input)
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_sign(auth_ctx_t *session, uint8_t sk[], uint8_t *sm, size_t *smlen, uint8_t *m, size_t mlen);

/**
 * @brief verify signature
 * @param session pointer to authenticated session struct
 * @param m pointer with message (output)
 * @param mlen pointer with length of m (output)
 * @param sm pointer with signed message (input)
 * @param smlen pointer with length of sm (input)
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_verify(auth_ctx_t *session, uint8_t *m, size_t *mlen, uint8_t *sm, size_t smlen);

/**
 * @brief send message over authenticated session
 * @param session pointer to authenticated session struct
 * @param sk array with secret key (ed25519 or RSA)
 * @param m pointer with message (input)
 * @param mlen pointer with length of m (input)
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_send(auth_ctx_t *session, uint8_t sk[], uint8_t *m, size_t mlen);

/**
 * @brief receive message over authenticated session
 * @param session pointer to authenticated session struct
 * @param sk array with secret key (ed25519 or RSA)
 * @param m pointer with message (input)
 * @param mlen pointer with length of m (input)
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_receive(auth_ctx_t *session, uint8_t sk[], uint8_t *m, size_t mlen);

/**
 * @brief releases authenticated session and unbinds socket
 *
 * @param session pointer to authenticator session context.
 *
 * @return AUTH_OK or AUTH_ERROR
 */
uint8_t auth_release(auth_ctx_t *session);

#ifdef __cplusplus
};
#endif
#endif /* AUTH_H */
