#include <string.h>
#include <strings.h>

#include "policy.h"
#include "policy_logger.h"

uint8_t policy_sign(policy_t *pol, uint8_t sk[]) {

  if (pol == NULL || sk == NULL) {
    log_error(policy_logger_id, "[%s:%d] invalid input.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  // normalize by erasing policy_id
  bzero(pol->policy_id, crypto_generichash_blake2b_BYTES + crypto_sign_BYTES);
  size_t policy_id_len;

  // serialize for crypto_generichash_blake2b
  uint8_t *ser_pol = (uint8_t*) pol;
  size_t ser_pol_len = sizeof(ser_pol);

  // policy hash
  uint8_t hash_pol[crypto_generichash_blake2b_BYTES];
  crypto_generichash_blake2b(hash_pol, crypto_generichash_blake2b_BYTES, ser_pol, ser_pol_len, NULL, 0);

  if (crypto_sign_ed25519(pol->policy_id, &policy_id_len, hash_pol, crypto_generichash_blake2b_BYTES, sk) != 0) {
    log_error(policy_logger_id, "[%s:%d] failed to sign policy.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  sodium_memzero(sk, crypto_sign_SECRETKEYBYTES);

  return POLICY_OK;
}

uint8_t policy_verify(policy_t *pol, uint8_t pk[]) {

  // normalize by erasing policy_id
  policy_t *norm_pol = calloc(1, sizeof(policy_t));
  memcpy(norm_pol, pol, sizeof(policy_t));
  bzero(norm_pol->policy_id, crypto_generichash_BYTES + crypto_sign_BYTES);

  // serialize for crypto_generichash_blake2b
  uint8_t *ser_norm_pol = (uint8_t*) norm_pol;
  size_t ser_norm_pol_len = sizeof(ser_norm_pol);

  // policy hash
  size_t serialized_normalized_hash_pol_len = crypto_generichash_blake2b_BYTES;
  uint8_t hash_pol[serialized_normalized_hash_pol_len];
  crypto_generichash_blake2b(hash_pol, crypto_generichash_blake2b_BYTES, ser_norm_pol, ser_norm_pol_len, NULL, 0);

  if (crypto_sign_ed25519_open(hash_pol, &serialized_normalized_hash_pol_len, pol->policy_id, sizeof(pol->policy_id), pk) != 0) {
    log_error(policy_logger_id, "[%s:%d] failed to verify policy.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  free(norm_pol);
  return POLICY_OK;
}

uint8_t policy_encode_json(policy_t pol, unsigned char *json) {

}

uint8_t policy_decode_json(unsigned char *json, policy_t *pol) {

}