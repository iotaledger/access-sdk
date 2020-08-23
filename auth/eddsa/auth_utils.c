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
 * \file auth_utils.c
 * \brief
 * Implementation of EdDSA key exchange and encryption key computation
 *
 * @Author Dejan Nedic, Milivoje Knezevic, Bernardo Araujo.
 *
 * \notes
 *
 * \history
 * 31.07.2018. Initial version.
 * 06.12.2018. Implemented ed25519 signature algorithm
 * 01.08.2020. Renaming.
 ****************************************************************************/
#include "auth_utils.h"
#include "auth_logger.h"
#include "tcpip.h"

#define ENC_DATA_LEN 2
#define SEC_NUM_LEN 1
#define READ_BUF_LEN 5
#define CHARS_TO_READ 3

//////////////////////////////////////
///
//////////////////////////////////////



static int hash(unsigned char *exchange_hash, unsigned char *message, int message_length) {
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, message, message_length);
  sha256_final(&ctx, exchange_hash);

  return 0;
}

int auth_utils_compute_signature_s(unsigned char *sig, auth_ctx_t *session, unsigned char *hash) {
  unsigned long long smlen;

  //crypto_sign(sig, &smlen, hash, DH_SHARED_SECRET_L, AUTH_GET_INTERNAL_PRIVATE_KEY(session));

  log_info(auth_logger_id, "[%s:%d] SMLEN: %d\n", __func__, __LINE__, smlen);
  return 0;
}

int auth_utils_verify_signature(unsigned char *sig, unsigned char *public_key, unsigned char *hash) {
  unsigned long long mlen;
  int ret = crypto_sign_open(hash, &mlen, sig, SIGNED_MESSAGE_L, public_key);

  if (ret == -1) {
    log_error(auth_logger_id, "[%s:%d] Verification failed.\n", __func__, __LINE__);
  }

  return -ret;
}

int auth_utils_concatenate_strings(unsigned char *concatenatedString, unsigned char *str1, int str1_l,
                                 unsigned char *str2, int str2_l) {
  for (int i = 0; i < str1_l; i++) {
    concatenatedString[i] = str1[i];
  }

  for (int i = 0; i < str2_l; i++) {
    concatenatedString[str1_l + i] = str2[i];
  }

  return 0;
}

static int aes_encrypt(AES_ctx_t *ctx, unsigned char *message, int length) {
  AES_CBC_encrypt_buffer(ctx, message, length);

  return 0;
}

static int aes_decrypt(AES_ctx_t *ctx, unsigned char *message, int length) {
  AES_CBC_decrypt_buffer(ctx, message, length);

  return 0;
}

static void hmac_sha256(unsigned char *mac, unsigned char *integrityKey, uint16_t keyLength, unsigned char *message,
                        uint32_t message_length) {
  SHA256_CTX ss;
  unsigned char kh[HASH_OUTPUT_L];
  unsigned char key[keyLength];
  unsigned char *internal_key;
  size_t internal_key_l = 0;

  memcpy(key, integrityKey, keyLength);

  if (keyLength > SHA256_BLOCK_BYTES) {
    sha256_init(&ss);
    sha256_update(&ss, key, keyLength);
    sha256_final(&ss, kh);

    internal_key = kh;
    internal_key_l = HASH_OUTPUT_L;
  } else {
    internal_key = key;
    internal_key_l = keyLength;
  }

  unsigned char kx[SHA256_BLOCK_BYTES];
  for (size_t i = 0; i < internal_key_l; i++) {
    kx[i] = I_PAD ^ internal_key[i];
  }
  for (size_t i = internal_key_l; i < SHA256_BLOCK_BYTES; i++) {
    kx[i] = I_PAD ^ 0;
  }

  sha256_init(&ss);
  sha256_update(&ss, kx, SHA256_BLOCK_BYTES);
  sha256_update(&ss, message, message_length);
  sha256_final(&ss, mac);

  for (size_t i = 0; i < internal_key_l; i++) {
    kx[i] = O_PAD ^ internal_key[i];
  }
  for (size_t i = internal_key_l; i < SHA256_BLOCK_BYTES; i++) {
    kx[i] = O_PAD ^ 0;
  }

  sha256_init(&ss);
  sha256_update(&ss, kx, SHA256_BLOCK_BYTES);
  sha256_update(&ss, mac, HASH_OUTPUT_L);
  sha256_final(&ss, mac);
}

int auth_utils_write(auth_ctx_t *session, const unsigned char *msg, unsigned short message_length) {
  // TODO: From where this magic numbers came from?
  unsigned short encrypted_data_length =
      ((message_length + 2 + 15) / 16) * 16;  // determine size of encrypted data with padding
  unsigned short buffer_length = encrypted_data_length + MAC_HASH_L + ENC_DATA_LEN + SEC_NUM_LEN;
  unsigned char mac[MAC_HASH_L];

  unsigned char buffer[buffer_length];

  //buffer[0] = AUTH_GET_INTERNAL_SEQ_NUM_ENCRYPT(session);
  buffer[1] = ((encrypted_data_length >> 8));
  buffer[2] = (encrypted_data_length);
  buffer[3] = ((message_length >> 8));
  buffer[4] = message_length;

  for (int i = 0; i < message_length; i++) {
    buffer[i + 5] = msg[i];
  }

  for (int i = message_length + 2; i < encrypted_data_length; i++) {
    buffer[i + 3] = 0;
  }

  //aes_encrypt(&AUTH_GET_INTERNAL_CTX_ENCRYPT(session), buffer + 3, encrypted_data_length);

  //hmac_sha256(mac, AUTH_GET_INTERNAL_INTEGRITY_KEY_ENCRYPTION(session), INTEGRITY_KEY_L, buffer,
  //            encrypted_data_length + 3);

//  for (int i = 0; i < MAC_HASH_L; i++) {
//    buffer[i + encrypted_data_length + 2 + 1] = mac[i];
//  }
//
//  int n = tcpip_write(session->sockfd, buffer, buffer_length);
//
//  AUTH_GET_INTERNAL_SEQ_NUM_ENCRYPT(session)++;
//
//  if (n <= 0) {
//    return 1;
//  }

  return 0;
}

