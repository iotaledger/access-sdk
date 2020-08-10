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
 * IOTA Access
 * @file auth.h
 * @brief authentication module API
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

/**
 * @brief no authentication error
 */
#define AUTH_OK 0

/**
 * @brief authentication error
 */
#define AUTH_ERROR 1

/**
 * @brief authenticator server port
 */
#define AUTH_SERVER_PORT 9998

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief generic function signature type for socket read/write
 *
 * it is used on the definition of auth_ctx_t to declare f_read and f_write function members.
 *
 * @param ext pointer to external data.
 * @param data data to be written or read.
 * @param len length of the data.
 */
typedef ssize_t f_auth_ext_t(void *ext, void *data, unsigned short len);

/**
 * @brief verifies key exchange
 *
 * @param
 */
typedef int f_auth_key_verify(unsigned char *, int);

/**
 * @brief declaration of auth_struct typedef.
 *
 * actual definition of this struct happens at access_auth_api/implementations/${auth_implementation}/auth_internal.h
 */
typedef struct auth_struct auth_struct_t;


/**
 * @struct auth_ctx_t
 * @brief structure to represent authentication sessions.
 */
typedef struct {
  /*@{*/
  auth_struct_t *internal; /**< internal data */
  void *ext; /**< external data */
  /*@}*/

  /*@{*/
  f_auth_ext_t *f_write; /**< function to write on authenticated session */
  f_auth_ext_t *f_read; /**< function to read from authenticated session */
  /*@}*/

  /*@{*/
  f_auth_key_verify *f_verify; /**< function to verify keys of authenticated session */
  /*@}*/

  /*@{*/
  int status; /**< status of authenticated session */
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
int auth_init_client(auth_ctx_t *session, void *ext);

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
int auth_init_server(auth_ctx_t *session, void *ext);

/**
 * @brief sets option (priv/pub keys) to authenticator session context.
 *
 * sets option to authenticator session context with key/value pairs.
 *
 * @param session pointer to authenticator session context.
 * @param key pointer to key, where expected values are "public" and "private"
 * @param value pointer to the actual body of the key.
 *
 * @return AUTH_OR or AUTH_ERROR
 */
int auth_set_option(auth_ctx_t *session, const char *key, unsigned char *value);

/**
 * @brief performs actual authentication
 *
 * performs actual authentication, be it server or client side
 *
 * @param session pointer to authenticator session context.
 *
 * @return AUTH_OR or AUTH_ERROR
 */
int auth_authenticate(auth_ctx_t *session);

/**
 * @brief send data over authenticated session
 *
 * sends data over authenticated session
 *
 * @param session pointer to authenticator session context.
 * @param data pointer to data to be sent.
 * @param len data length (bytes)
 *
 * @return AUTH_OK or AUTH_ERROR
 */
int auth_send(auth_ctx_t *session, const unsigned char *data, unsigned short len);

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
int auth_receive(auth_ctx_t *session, unsigned char **data, unsigned short *len);

/**
 * @brief release authenticated session
 *
 * releases authenticated session and unbinds socket
 *
 * @param session pointer to authenticator session context.
 *
 * @return AUTH_OK or AUTH_ERROR
 */
int auth_release(auth_ctx_t *session);

#ifdef __cplusplus
};
#endif
#endif /* AUTH_H */
