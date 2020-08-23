#include "auth_internal.h"

uint8_t auth_internal_encrypt(auth_ctx_t *session, uint8_t ed25519_sk[], uint8_t ciphertext[], const uint8_t *data) {

  uint8_t x25519_sk[crypto_scalarmult_curve25519_BYTES];
  crypto_sign_ed25519_sk_to_curve25519(x25519_sk, ed25519_sk);

  if (crypto_box_easy(ciphertext, data, MSGLEN, session->internal->nonce,
                      session->internal->peer_x25519_pk, x25519_sk) != 0) {
    log_error(auth_logger_id, "[%s:%d] failed to encrypt data.\n", __func__, __LINE__);

    return AUTH_ERROR;
  }

  sodium_memzero(x25519_sk, crypto_scalarmult_curve25519_BYTES);
  sodium_memzero(ed25519_sk, crypto_scalarmult_curve25519_BYTES);

  return AUTH_OK;
}

uint8_t auth_internal_decrypt(auth_ctx_t*session, uint8_t ed25519_sk[], uint8_t *data, const uint8_t *ciphertext){

  uint8_t x25519_sk[crypto_scalarmult_curve25519_BYTES];
  crypto_sign_ed25519_sk_to_curve25519(x25519_sk, ed25519_sk);

  if (crypto_box_open_easy(data, ciphertext, CHIPERLEN, session->internal->nonce,
                           session->internal->peer_x25519_pk, x25519_sk) != 0) {
    log_error(auth_logger_id, "[%s:%d] failed to decrypt cipher.\n", __func__, __LINE__);

    return AUTH_ERROR;
  }

  sodium_memzero(x25519_sk, crypto_scalarmult_curve25519_BYTES);
  sodium_memzero(ed25519_sk, crypto_scalarmult_curve25519_BYTES);

  return AUTH_OK;
}

void auth_internal_release_server(auth_ctx_t *session) {

}