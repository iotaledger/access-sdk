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
 * \file auth_server.c
 * \brief
 * Server side implemntation for EdDSA based authentication module
 *
 * @Author Dejan Nedic, Milivoje Knezevic, Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 31.07.2018. Initial version.
 * 06.12.2018. Implemented ed25519 signature algorithm
 * 01.08.2020. Renaming.
 ****************************************************************************/

/////////////////
/// Includes
/////////////////
#include "sodium.h"
#include "auth_internal.h"
#include "auth_logger.h"
#include "tcpip.h"

//////////////////////////////////////////////////////////////////
/// internal server authentication function declarations and definitions
//////////////////////////////////////////////////////////////////

int auth_internal_server_authenticate(auth_ctx_t *session, uint8_t ed25519_sk[]) {
  int ret = AUTH_ERROR;

  session->internal->type = AUTH_TYPE_SERVER;

  // ed25519
  uint8_t ed25519_pk[crypto_sign_PUBLICKEYBYTES];
  crypto_sign_ed25519_sk_to_pk(ed25519_pk, ed25519_sk);

  uint8_t *internal_pk = session->internal->ed25519_pk;
  uint8_t *internal_sk = session->internal->ed25519_sk;

  memcpy(internal_pk, ed25519_pk, crypto_sign_PUBLICKEYBYTES);
  memcpy(internal_sk, ed25519_sk, crypto_sign_SECRETKEYBYTES);

  // x25519
  uint8_t x25519_pk[crypto_scalarmult_curve25519_BYTES];
  uint8_t x25519_sk[crypto_scalarmult_curve25519_BYTES];

  crypto_sign_ed25519_pk_to_curve25519(x25519_pk, ed25519_pk);
  crypto_sign_ed25519_sk_to_curve25519(x25519_sk, ed25519_sk);

  memcpy(session->internal->x25519_pk, x25519_pk, crypto_scalarmult_curve25519_BYTES);
  memcpy(session->internal->x25519_sk, x25519_sk, crypto_scalarmult_curve25519_BYTES);

  // Read client's x25519_pk
  log_info(auth_logger_id, "[%s:%d] waiting for client's x25519_pk.\n", __func__, __LINE__);

  int read_message = tcpip_read(session->sockfd, session->internal->peer_x25519_pk, crypto_scalarmult_curve25519_BYTES);
  if (read_message != crypto_scalarmult_curve25519_BYTES){
    log_error(auth_logger_id, "[%s:%d] failed to read client x25519_pk.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  log_info(auth_logger_id, "[%s:%d] client's x25519_pk registered.\n", __func__, __LINE__);

  log_info(auth_logger_id, "[%s:%d] waiting for DH nonce.\n", __func__, __LINE__);
  read_message = tcpip_read(session->sockfd, session->internal->nonce, crypto_box_NONCEBYTES);
  if (read_message != crypto_box_NONCEBYTES){
    log_error(auth_logger_id, "[%s:%d] failed to read nonce.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  log_info(auth_logger_id, "[%s:%d] DH nonce registered.\n", __func__, __LINE__);

  log_info(auth_logger_id, "[%s:%d] sending DH x25519_pk.\n", __func__, __LINE__);
  int write_message = tcpip_write(session->sockfd, session->internal->x25519_pk, crypto_scalarmult_curve25519_BYTES);
  if (write_message != crypto_scalarmult_curve25519_BYTES){
    log_error(auth_logger_id, "[%s:%d] failed to send DH x25519_pk.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  log_info(auth_logger_id, "[%s:%d] x25519 DH sent.\n", __func__, __LINE__);

  // destroy private keys
  bzero(session->internal->ed25519_sk, crypto_sign_SECRETKEYBYTES);
  bzero(session->internal->x25519_sk, crypto_scalarmult_curve25519_BYTES);

  return ret;
}