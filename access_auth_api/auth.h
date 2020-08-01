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

/* AUTH_ERRORS */
#define AUTH_OK 0
#define AUTH_ERROR 1

#define AUTH_SERVER_PORT 9998

#ifdef __cplusplus
extern "C" {
#endif

typedef ssize_t f_auth_ext_t(void *, void *, unsigned short);

typedef int f_auth_key_verify(unsigned char *, int);

typedef struct auth_struct auth_struct_t;

typedef struct {
  auth_struct_t *internal;

  void *ext; /* External data structure */

  f_auth_ext_t *f_write;
  f_auth_ext_t *f_read;

  f_auth_key_verify *f_verify;

  int status;
} auth_ctx_t;

int auth_init_client(auth_ctx_t *, void *);
int auth_init_server(auth_ctx_t *, void *);

int auth_set_option(auth_ctx_t *, const char *, unsigned char *);

int auth_authenticate(auth_ctx_t *);

int auth_send(auth_ctx_t *, const unsigned char *, unsigned short);

int auth_receive(auth_ctx_t *, unsigned char **, unsigned short *);

int auth_release(auth_ctx_t *);
#ifdef __cplusplus
};
#endif
#endif /* AUTH_H */
