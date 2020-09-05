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
 * \file auth_client.c
 * \brief
 * Client side implemntation for EdDSA based authentication module
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

#include "auth_logger.h"
#include "auth_internal.h"
#include "tcpip.h"
/////////////////////////////////////
/// Macros and defines
/////////////////////////////////////

/* AUTH_STAGES */
#define AUTH_ERROR 0
#define AUTH_INIT 1
#define AUTH_COMPUTE 2
#define AUTH_VERIFY 3
#define AUTH_FINISH 4
#define AUTH_DONE 5

//////////////////////////////////////////////////////////////////
/// Client authantication function declarations and definitions
//////////////////////////////////////////////////////////////////

int auth_client_verify(auth_ctx_t *session) {
//  int next_stage = AUTH_ERROR;
//
//  int SIZE_OF_READ_BUFFER = PUBLIC_KEY_L + crypto_sign_PUBLICKEYBYTES + SIGNED_MESSAGE_L;  // size of ks + f + s
//  unsigned char readBuffer[SIZE_OF_READ_BUFFER];
//  unsigned char *s_signed;
//  unsigned char *received_dh_public;
//  unsigned char *server_public_key;
//
//  unsigned char vs[IDENTIFICATION_STRING_L];
//  memcpy(vs, "server", IDENTIFICATION_STRING_L);
//  unsigned char signature[SIGNED_MESSAGE_L];
//  unsigned char message[PUBLIC_KEY_L + SIGNED_MESSAGE_L];
//
//  // Client receives ( ks || f || s )
//  ssize_t read_message = tcpip_read(session->sockfd, readBuffer, SIZE_OF_READ_BUFFER);
//  if (read_message != SIZE_OF_READ_BUFFER) {
//    return AUTH_ERROR;
//  }
//
//  server_public_key = readBuffer;
//  received_dh_public = readBuffer + PUBLIC_KEY_L;
//  s_signed = readBuffer + PUBLIC_KEY_L + crypto_scalarmult_curve25519_BYTES;
//
//  // Client verifies that ks is public key of the Server
//  int key_verified = verify(received_dh_public, PUBLIC_KEY_L);

//  // Client computes k = fx mod p
//  int secret_computed = auth_utils_dh_compute_secret_k(session, received_dh_public);
//
//  // Client computes h  = hash( vc || vs || ks || e || f || k )
//  int h_computed = auth_utils_compute_session_identifier_h(
//      AUTH_GET_INTERNAL_EXCHANGE_HASH(session), AUTH_GET_INTERNAL_ID_V(session), vs, server_public_key,
//      AUTH_GET_INTERNAL_DH_PUBLIC(session), received_dh_public, AUTH_GET_INTERNAL_SECREY_K(session));
//
//  // Client verifies the signature s on h
//  int signature_verified =
//      auth_utils_verify_signature(s_signed, server_public_key, AUTH_GET_INTERNAL_EXCHANGE_HASH(session));
//
//  // Client computes hc = hash( vc || vs || kc || e || f || k )
//  int computed_H = auth_utils_compute_session_identifier_h(
//      AUTH_GET_INTERNAL_EXCHANGE_HASH2(session), AUTH_GET_INTERNAL_ID_V(session), vs,
//      AUTH_GET_INTERNAL_PUBLIC_KEY(session), AUTH_GET_INTERNAL_DH_PUBLIC(session), received_dh_public,
//      AUTH_GET_INTERNAL_SECREY_K(session));
//
//  // Client computes signature sc = sign( skc, hc )
//  int message_signed = auth_utils_compute_signature_s(signature, session, AUTH_GET_INTERNAL_EXCHANGE_HASH2(session));
//
//  auth_utils_concatenate_strings(message, AUTH_GET_INTERNAL_PUBLIC_KEY(session), PUBLIC_KEY_L, signature,
//                               SIGNED_MESSAGE_L);
//  ssize_t message_written = tcpip_write(session->sockfd, message, PUBLIC_KEY_L + SIGNED_MESSAGE_L);

//  if ((read_message == SIZE_OF_READ_BUFFER) && (key_verified == 0) && (secret_computed == 0) && (h_computed == 0) &&
//      (signature_verified == 0) && (computed_H == 0) && (message_signed == 0) &&
//      (message_written == PUBLIC_KEY_L + SIGNED_MESSAGE_L)) {
//    next_stage = AUTH_FINISH;
//  } else {
//    printf("\n%d\n", read_message);
//    printf("%d\n", key_verified);
//    printf("%d\n", secret_computed);
//    printf("%d\n", h_computed);
//    printf("%d\n", signature_verified);
//    printf("%d\n", computed_H);
//    printf("%d\n", message_signed);
//    printf("%d\n", message_written);
//  }
//
//  return next_stage;
}

int auth_internal_client_authenticate(auth_ctx_t *session, uint8_t ed25519_sk[]) {
  int ret = AUTH_ERROR;

  session->internal->type = AUTH_TYPE_CLIENT;

  // derive ed25519_pk
  uint8_t ed25519_pk[crypto_sign_PUBLICKEYBYTES];
  crypto_sign_ed25519_sk_to_pk(ed25519_pk, ed25519_sk);

  // save internal ed25519_pk
  memcpy(session->internal->ed25519_pk, ed25519_pk, crypto_sign_PUBLICKEYBYTES);

  // x25519
  uint8_t x25519_pk[crypto_scalarmult_curve25519_BYTES];
  uint8_t x25519_sk[crypto_scalarmult_curve25519_BYTES];

  // x25519 from ed25519
  crypto_sign_ed25519_pk_to_curve25519(x25519_pk, ed25519_pk);
  crypto_sign_ed25519_sk_to_curve25519(x25519_sk, ed25519_sk);

  // save internal x25519_pk
  memcpy(session->internal->x25519_pk, x25519_pk, crypto_scalarmult_curve25519_BYTES);

  // client sends ed25519_pk to server.
  log_info(auth_logger_id, "[%s:%d] sending ed25519_pk.\n", __func__, __LINE__);
  int write_message = tcpip_write(session->sockfd, session->internal->ed25519_pk, crypto_scalarmult_curve25519_BYTES);
  if (write_message != crypto_scalarmult_curve25519_BYTES){
    log_error(auth_logger_id, "[%s:%d] failed to send ed25519_pk.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  log_info(auth_logger_id, "[%s:%d] ed25519_pk sent.\n", __func__, __LINE__);

  // generate nonce
  for (int i = 0; i < crypto_box_NONCEBYTES; i++) {session->internal->nonce[i] = rand();}

  // Client sends nonce to Server.
  log_info(auth_logger_id, "[%s:%d] sending DH nonce.\n", __func__, __LINE__);
  write_message = tcpip_write(session->sockfd, session->internal->nonce, crypto_box_NONCEBYTES);
  if (write_message <= 0){
    log_error(auth_logger_id, "[%s:%d] failed to send DH nonce.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  log_info(auth_logger_id, "[%s:%d] DH nonce sent.\n", __func__, __LINE__);

  // receive server ed25519_pk
  log_info(auth_logger_id, "[%s:%d] waiting for server's ed25519_pk.\n", __func__, __LINE__);

  uint8_t peer_ed25519_pk[crypto_sign_PUBLICKEYBYTES];
  int read_message = tcpip_read(session->sockfd, peer_ed25519_pk, crypto_sign_PUBLICKEYBYTES);
  if (read_message != crypto_sign_PUBLICKEYBYTES){
    log_error(auth_logger_id, "[%s:%d] failed to read server ed25519_pk.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  log_info(auth_logger_id, "[%s:%d] received server's ed25519_pk.\n", __func__, __LINE__);

  memcpy(session->internal->peer_ed25519_pk, peer_ed25519_pk,crypto_sign_PUBLICKEYBYTES );
  log_info(auth_logger_id, "[%s:%d] server's ed25519_pk registered.\n", __func__, __LINE__);

  // x25519 from ed25519
  uint8_t peer_x25519_pk[crypto_scalarmult_curve25519_BYTES];
  crypto_sign_ed25519_pk_to_curve25519(peer_x25519_pk, peer_ed25519_pk);

  // save internal peer_x25519_pk
  memcpy(session->internal->peer_x25519_pk, peer_x25519_pk, crypto_scalarmult_curve25519_BYTES);

  log_info(auth_logger_id, "[%s:%d] server's x25519_pk registered.\n", __func__, __LINE__);

  // destroy private keys
  sodium_memzero(ed25519_sk, crypto_sign_SECRETKEYBYTES);
  sodium_memzero(x25519_sk, crypto_scalarmult_curve25519_BYTES);

  return ret;
}

int auth_internal_client_send(auth_ctx_t *session, const unsigned char *data, unsigned short data_len) {
  //return auth_utils_write(session, data, data_len);
}

int auth_internal_client_receive(auth_ctx_t *session, unsigned char **data, unsigned short *data_len) {
  //return auth_utils_read(session, data, data_len);
}

void auth_internal_release_client(auth_ctx_t *session) {}

int auth_internal_client_set_option(auth_ctx_t *session, const char *key, unsigned char *value) {
 // return auth_utils_set_option(session, key, value);
}
