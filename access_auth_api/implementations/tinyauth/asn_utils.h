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
 * Header file with function definitions for authentication.
 *
 * @Author Nikola Kuzmanovic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 14.08.2018. Initial version.
 * 01.08.2020. Renaming.
 ****************************************************************************/
#ifndef AUTH_UTILS_H_
#define AUTH_UTILS_H_

#include <stdlib.h>

#include "auth_internal.h"

/**
 * @fn  int auth_utils_compute_signature_s(unsigned char *sig, auth_ctx_t
 * *session, unsigned char *hash);
 *
 * @brief   Function that signes data with private key
 *
 * @param   sig           Result of signing function
 * @param   session    Data structure that contains session related information
 * @param   hash          Computed hash h for signing
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_compute_signature_s(unsigned char *sig, auth_ctx_t *session, unsigned char *hash);

/**
 * @fn  int auth_utils_verify_signature(unsigned char *sig, unsigned char
 * *public_key, unsigned char *hash);
 *
 * @brief   Function that verifies signed date with public key
 *
 * @param   sig          Signature to be verified
 * @param   public_key   Public key for signature verification
 * @param   hash         Computed hash h for signing
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_verify_signature(unsigned char *sig, unsigned char *public_key, unsigned char *hash);

/**
 * @fn  int auth_utils_dh_generate_keys(auth_ctx_t *session);
 *
 * @brief   Function that generates Diffie-Hellman private and public keys
 *
 * @param   session    Data structure that contain private and public
 * Diffie-Hellman keys
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_dh_generate_keys(auth_ctx_t *session);

/**
 * @fn  int auth_utils_dh_compute_secret_k(auth_ctx_t *session,  const unsigned
 * char *public_key);
 *
 * @brief   Function that computes shared secret from Diffie-Hellman key
 * exchange
 *
 * @param   session    Data structure that contain session related data
 * @param   public_key    Received public key
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_dh_compute_secret_k(auth_ctx_t *session, const unsigned char *public_key);

/**
 * @fn  int auth_utils_compute_session_identifier_h(unsigned char *exchange_hash,
 * char *vc, char *vs, char *ks, unsigned char *c_public, unsigned char
 * *s_public, unsigned char *secretK);
 *
 * @brief   Function that computes hash h from Diffie-Hellman key exchange
 *
 * @param   exchange_hash    Shared hash h
 * @param   vc          Client idetification string
 * @param   vs          Server idetification string
 * @param   k           Public key
 * @param   c_public    Client Diffie-Hellman public key
 * @param   s_public    Server Diffie-Hellman public key
 * @param   secretK     Shared secret k
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_compute_session_identifier_h(unsigned char *exchange_hash, unsigned char *vc, unsigned char *vs,
                                          unsigned char *k, unsigned char *c_public, unsigned char *s_public,
                                          unsigned char *secretK);

/**
 * @fn  int auth_utils_generate_enc_auth_keys(unsigned char *hash, char
 * *shared_secret_K, char *shared_H, char magic_letter);
 *
 * @brief   Function that computes AES keys
 *
 * @param   hash               Computed AES key
 * @param   shared_secret_K    Shared secret k
 * @param   shared_H           Shared hash h
 * @param   magic_letter       Character, unique for different key types ('A' -
 * 'F')
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_generate_enc_auth_keys(unsigned char *hash, unsigned char *shared_secret_K, unsigned char *shared_H,
                                    char magic_letter);

/**
 * @fn  int auth_utils_concatenate_strings(unsigned char *concatenatedString,
 * unsigned char *str1, int str1_l, unsigned char * str2, int str2_l);
 *
 * @brief   Function that takes two strings and concatenates them
 *
 * @param   concatenatedString    Concatinated string
 * @param   str1                  First string to be concatenated
 * @param   str1_l                First string length
 * @param   str2                  Second string to be concatenated
 * @param   str2_l                Second string length
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_concatenate_strings(unsigned char *concatenatedString, unsigned char *str1, int str1_l,
                                 unsigned char *str2, int str2_l);

/**
 * @fn  int auth_utils_read(auth_ctx_t *session, unsigned char **msg, int
 * length);
 *
 * @brief   Function that reads messages
 *
 * @param   session   Data structure that contain session related data
 * @param   msg       Buffer for message to be read
 * @param   length    Message length
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_read(auth_ctx_t *session, unsigned char **msg, unsigned short *message_length);

/**
 * @fn  int auth_utils_write(auth_ctx_t *session, unsigned char *msg, int
 * length);
 *
 * @brief   Function that writes messages
 *
 * @param   session   Data structure that contain session related data
 * @param   msg       Message to be written
 * @param   length    Message length
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_write(auth_ctx_t *session, const unsigned char *msg, unsigned short message_length);

/**
 * @fn  int auth_utils_set_option(auth_ctx_t *session, const char *key,
 * unsigned char *value)
 *
 * @brief   Function that writes messages
 *
 * @param   session   	Data structure that contain session related data
 * @param   key       	Option to be set
 * @param   value    	Value for the option
 *
 * @return  0 if it succeeds, 1 if it fails.
 */
int auth_utils_set_option(auth_ctx_t *session, const char *key, unsigned char *value);

#endif /* AUTH_UTILS_H_ */
