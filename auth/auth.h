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

#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <stdint.h>

#include "auth_logger.h"

#define AUTH_OK 0
#define AUTH_ERROR 1

#define AUTH_SERVER 0
#define AUTH_CLIENT 1

/**
 * @struct auth_ctx_t
 * @brief structure to represent authenticated TCP/IP sessions.
 *
 * it is important to note that secret keys are never stored on memory
 * secret key representation only happens inside the scope of function calls
 * secret key representation is always destroyed from memory before function returns
 */
typedef struct {
  /*@{*/
  uint8_t side; /**< client or server*/
  uint8_t sockfd; /**< socket file descriptor */
  uint16_t port; /**< port */
  struct sockaddr_in *peer_ip; /** peer IP address */
  /*@}*/

  /*@{*/
  uint8_t *nonce; /**< session nonce */
  uint8_t *peer_DH_pk; /** peer Diffie Hellman public key */
  uint8_t *peer_sign_pk; /** peer signing public key */
  /*@}*/

  /*@{*/
  unsigned char *m; /**< message */
  size_t mlen; /**< message length */
  unsigned char *sm; /**< signed message */
  size_t smlen; /**< signed message length */
  unsigned char *em; /**< encrypted message */
  size_t emlen; /** encrypted message length */
  /*@}*/
} auth_ctx_t;

/**
 * @brief initialize authenticator in client mode.
 *
 * initialize authenticator in client mode.
 *
 * @param session pointer to authenticator session context.
 * @param serv_ip server IP address
 * @param port port
 *
 * @return AUTH_OK or AUTH_ERROR
 */
uint8_t auth_init_client(auth_ctx_t *session, char *serv_ip, uint16_t port);

/**
 * @brief initialize authenticator in server mode.
 *
 * initialize authenticator in server mode.
 *
 * @param session pointer to authenticator session context.
 * @param port port
 *
 * @return AUTH_OK or AUTH_ERROR
 */
uint8_t auth_init_server(auth_ctx_t *session, uint16_t port);

/**
 * @brief authenticate session
 *
 * populates the following auth_ctx_t members:
 * - uint8_t *nonce: session nonce
 * - *peer_DH_pk: peer's Diffie Hellman public key
 * - *peer_sign_pk: peer's signing public key
 *
 * @param session pointer to authenticator session context.
 *
 * @return AUTH_OR or AUTH_ERROR
 */
uint8_t auth_authenticate(auth_ctx_t *session);

/**
 * @brief send message over authenticated session
 *
 * sends data over authenticated session
 *
 * @param session pointer to auth session context.
 * @param m pointer to message.
 * @param mlen data length (bytes)
 *
 * @return AUTH_OK or AUTH_ERROR
 */

uint8_t auth_send(auth_ctx_t *session, const unsigned char *m, unsigned short mlen);

/**
 * @brief receive data over authenticated session
 *
 * receives data over authenticated session
 *
 * @param pointer to authenticator session context.
 * @param data pointer to received data.
 * @param len pointer to data length (bytes).
 *
 * @return AUTH_OK or AUTH_ERROR
 */
uint8_t auth_receive(auth_ctx_t *session, unsigned char *m, unsigned short mlen);

/**
 * @brief release authenticated session
 *
 * releases authenticated session and unbinds socket
 *
 * @param session pointer to authenticator session context.
 *
 * @return AUTH_OK or AUTH_ERROR
 */
uint8_t auth_release(auth_ctx_t *session);

#endif /* AUTH_H */
