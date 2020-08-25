#include "auth_internal.h"

uint8_t auth_internal_encrypt(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *cipher, const uint8_t *m) {

  uint8_t x25519_sk[crypto_scalarmult_curve25519_BYTES];
  crypto_sign_ed25519_sk_to_curve25519(x25519_sk, ed25519_sk);

  if (crypto_box_easy(cipher, m, MSGLEN, session->internal->nonce,
                      session->internal->peer_x25519_pk, x25519_sk) != 0) {
    log_error(auth_logger_id, "[%s:%d] failed to encrypt msg.\n", __func__, __LINE__);

    return AUTH_ERROR;
  }

  sodium_memzero(x25519_sk, crypto_scalarmult_curve25519_BYTES);
  sodium_memzero(ed25519_sk, crypto_sign_SECRETKEYBYTES);

  return AUTH_OK;
}


uint8_t auth_internal_decrypt(auth_ctx_t*session, uint8_t ed25519_sk[], uint8_t *m, const uint8_t *cipher){

  uint8_t x25519_sk[crypto_scalarmult_curve25519_BYTES];
  crypto_sign_ed25519_sk_to_curve25519(x25519_sk, ed25519_sk);

  if (crypto_box_open_easy(m, cipher, CIPHERLEN, session->internal->nonce,
                           session->internal->peer_x25519_pk, x25519_sk) != 0) {
    log_error(auth_logger_id, "[%s:%d] failed to decrypt cipher.\n", __func__, __LINE__);

    return AUTH_ERROR;
  }

  sodium_memzero(x25519_sk, crypto_scalarmult_curve25519_BYTES);
  sodium_memzero(ed25519_sk, crypto_sign_SECRETKEYBYTES);

  return AUTH_OK;
}

uint8_t auth_internal_sign(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *sm, size_t *smlen, uint8_t *m, size_t mlen) {

  if (crypto_sign_ed25519(sm, smlen, m, mlen, ed25519_sk) != 0) {
    log_error(auth_logger_id, "[%s:%d] failed to sign msg.\n", __func__, __LINE__);

    return AUTH_ERROR;
  }

  sodium_memzero(ed25519_sk, crypto_sign_SECRETKEYBYTES);

  return AUTH_OK;
}

uint8_t auth_internal_verify(auth_ctx_t *session, uint8_t *m, size_t *mlen, uint8_t *sm, size_t smlen) {

  if (crypto_sign_ed25519_open(m, mlen, sm, smlen, session->internal->peer_ed25519_pk) != 0) {
    log_error(auth_logger_id, "[%s:%d] failed to open msg.\n", __func__, __LINE__);

    return AUTH_ERROR;
  }

  return AUTH_OK;

}

uint8_t auth_internal_send(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *m, size_t mlen) {

  /* encrypting will destroy key, so we create a copy for signing. */
  uint8_t ed25519_sk_sign[crypto_sign_SECRETKEYBYTES];
  strcpy(ed25519_sk_sign, ed25519_sk);

  uint8_t cipher[CIPHERLEN];
  if (auth_internal_encrypt(session, ed25519_sk, cipher, m) != AUTH_OK) {
    log_error(auth_logger_id, "[%s:%d] failed to encrypt msg.\n", __func__, __LINE__);

    return AUTH_ERROR;
  }

  uint16_t socket_fd = session->sockfd;
  if (session->type == AUTH_TYPE_SERVER){
    socket_fd = session->sockfd;
  }

  uint8_t sigcipher[SIGCIPHERLEN];
  size_t sigcipherlen;
  auth_internal_sign(session, ed25519_sk_sign, sigcipher, &sigcipherlen, cipher, CIPHERLEN);
  if (sigcipherlen != SIGCIPHERLEN) {
    log_error(auth_logger_id, "[%s:%d] failed to sign cipher.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  /* send encrypted message to server */
  tcpip_write(socket_fd, sigcipher, SIGCIPHERLEN);

  // log_info(auth_logger_id, "[%s:%d] send authenticated msg: %s\n", __func__, __LINE__, m);

  sodium_memzero(ed25519_sk, crypto_sign_SECRETKEYBYTES);

  return AUTH_OK;

}

uint8_t auth_internal_receive(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t *m, size_t mlen) {

  // read sigcipher
  uint8_t sigcipher[SIGCIPHERLEN];
  if (tcpip_read(session->sockfd, sigcipher, SIGCIPHERLEN) != SIGCIPHERLEN) {
    log_error(auth_logger_id, "[%s:%d] failed to receive sigcipher.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  // verify
  uint8_t cipher[CIPHERLEN];
  size_t cipherlen = 0;
  if (auth_internal_verify(session, cipher, &cipherlen, sigcipher, SIGCIPHERLEN) != AUTH_OK){
    log_error(auth_logger_id, "[%s:%d] failed to verify signature.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  // decrypt
  if (auth_internal_decrypt(session, ed25519_sk, m, cipher) != AUTH_OK){
    log_error(auth_logger_id, "[%s:%d] failed to decrypt cipher.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  // log_info(auth_logger_id, "[%s:%d] read authenticated msg: %s\n", __func__, __LINE__, m);

  sodium_memzero(ed25519_sk, crypto_sign_SECRETKEYBYTES);

  return AUTH_OK;
}