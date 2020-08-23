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

#define MSGLEN 1024
#define CHIPERLEN (crypto_box_MACBYTES + MSGLEN)

#include <stdio.h>
#include <stdint.h>
/**
 * @brief no authentication error
 */
#define AUTH_OK 0

/**
 * @brief authentication error
 */
#define AUTH_ERROR 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief write to socket
 *
 * @param sockfd pointer to socker file descriptor
 * @param buffer with data to be written
 * @param len length of data
 *
 * @return On success, the number of bytes written is returned (zero indicates nothing was written). On error, -1 is returned, and errno is set appropriately.
 */
ssize_t write_socket(int *sockfd, void *data, unsigned short len);

/**
 * @brief read from socket
 *
 * @param sockfd pointer to socker file descriptor
 * @param buffer with for data to be read
 * @param len length of data
 *
 * @return On success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number. On error, -1 is returned, and errno is set appropriately. In this case, it is left unspecified whether the file position (if any) changes.
 */
ssize_t read_socket(int *sockfd, void *data, unsigned short len);

/**
 * @brief verify key
 * @param key
 * @param len
 * @return
 */
int verify(unsigned char *key, int len);

/**
 * @brief generic function signature type for socket read/write
 *
 * it is used on the definition of auth_ctx_t to declare f_read and f_write function members.
 *
 * @param sockfd pointer to socket file descriptor.
 * @param data data to be written or read.
 * @param len length of the data.
 */
typedef ssize_t f_auth_socket_t(int *sockfd, void *data, unsigned short len);

/**
 * @brief generic function signature type for key exchange verification
 *
 * @param
 */
typedef int f_auth_key_verify(unsigned char *, int);

/**
 * @brief declaration of auth_struct typedef.
 *
 * actual definition of this struct happens at auth/flavours/${auth_flavour}/auth_internal.h
 */
typedef struct auth_struct auth_struct_t;


/**
 * @struct auth_ctx_t
 * @brief structure to represent authentication sessions.
 */
typedef struct {
  /*@{*/
  int type;
  /*@}*/

  /*@{*/
  auth_struct_t *internal; /**< internal data */
  int sockfd; /**< socket file descriptor */
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
int auth_init_client(auth_ctx_t *session, int *sockfd);

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
int auth_init_server(auth_ctx_t *session, int *sockfd);

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
int auth_authenticate(auth_ctx_t *session, uint8_t sk[]);

// todo: test this
uint8_t auth_encrypt(auth_ctx_t *session, uint8_t ed25516_sk[], uint8_t ciphertext[], const uint8_t *data);

// todo: test this
uint8_t auth_decrypt(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *data, const uint8_t *ciphertext);

uint8_t auth_sign(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *sm, size_t *smlen, uint8_t *m, size_t mlen);

uint8_t auth_verify();

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
