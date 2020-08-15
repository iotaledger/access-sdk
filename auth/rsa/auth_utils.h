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
 * \file auth_utils.h
 * \brief
 * Header file with function definitions for RSA based authentication module
 *
 * @Author Djordje Golubovic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 05.05.2020. Initial version.
 * 01.08.2020. Renaming.
 ****************************************************************************/

#ifndef AUTH_UTILS_H_
#define AUTH_UTILS_H_

#include <stdlib.h>
#include <string.h>

#include "auth_debug.h"
#include "auth_internal.h"

void auth_utils_randmem(unsigned char *randomString, int length);

int auth_utils_send_message_part(auth_ctx_t *session, void *data, unsigned short data_len);

int auth_utils_send_message_part_bignum(auth_ctx_t *session, const BIGNUM *bn);

int auth_utils_receive_message_part(auth_ctx_t *session, unsigned char **data, unsigned short *data_len);

int auth_utils_compute_hash(auth_ctx_t *session, unsigned char *md, unsigned char *pkey, int pkey_len, const BIGNUM *e,
                          const BIGNUM *pub_key);

void auth_utils_debug_binary(char *name, unsigned char *data, int len);

void auth_utils_generate_keys(auth_ctx_t *session);

int auth_utils_send(auth_ctx_t *session, const void *hmacKey, AES_KEY *aes_key, unsigned char *iv,
                  const unsigned char *data, unsigned short data_len);

int auth_utils_receive(auth_ctx_t *session, const void *hmacKey, AES_KEY *aes_key, unsigned char *iv, unsigned char **data,
                     unsigned short *data_len);

#endif /* AUTH_UTILS_H_ */
