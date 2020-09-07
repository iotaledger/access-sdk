#include <string.h>
#include <strings.h>
#include <stdint.h>

#include "cJSON.h"
#include "libbase58.h"

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

uint8_t policy_encode_json(policy_t *pol, unsigned char pol_json[]) {

  size_t policy_id_len = crypto_generichash_blake2b_BYTES + crypto_sign_BYTES;
  size_t object_pk_len = crypto_sign_ed25519_PUBLICKEYBYTES;
  size_t subject_pk_len = crypto_sign_ed25519_PUBLICKEYBYTES;

  //////////////////////////////////////////////
  // encode to base58

  // 512 because we don't know b58_*_len beforehand
  char b58_policy_id_tmp[512]; size_t b58_policy_id_len;
  char b58_object_pk_tmp[512]; size_t b58_object_pk_len;
  char b58_subject_pk_tmp[512]; size_t b58_subject_pk_len;

  if ((b58enc(b58_policy_id_tmp, &b58_policy_id_len, pol->policy_id, policy_id_len) == false) ||
      (b58enc(b58_object_pk_tmp, &b58_object_pk_len, pol->object_pk, object_pk_len) == false) ||
      (b58enc(b58_subject_pk_tmp, &b58_subject_pk_len, pol->subject_pk, subject_pk_len) == false)){
    log_error(policy_logger_id, "[%s:%d] failed to encode policy fields to base58.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  char b58_policy_id[b58_policy_id_len];
  char b58_object_pk[b58_object_pk_len];
  char b58_subject_pk[b58_subject_pk_len];

  // copies of base58 arrays with correct sizes
  memcpy(b58_policy_id, b58_policy_id_tmp, b58_policy_id_len);
  memcpy(b58_object_pk, b58_object_pk_tmp, b58_object_pk_len);
  memcpy(b58_subject_pk, b58_subject_pk_tmp, b58_subject_pk_len);

  //////////////////////////////////////////
  // encode JSON

  cJSON *json = cJSON_CreateObject();

  cJSON *policy_id = cJSON_CreateString(b58_policy_id);;
  cJSON *object_pk = cJSON_CreateString(b58_object_pk);;
  cJSON *subject_pk = cJSON_CreateString(b58_subject_pk);;
  //cJSON *actions = cJSON_CreateArray();

  if ((policy_id == NULL) || (object_pk == NULL) || (subject_pk == NULL)) {
    log_error(policy_logger_id, "[%s:%d] failed to encode policy fields to JSON.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  cJSON_AddItemToObject(json, "policy_id", policy_id);
  cJSON_AddItemToObject(json, "object_pk", object_pk);
  cJSON_AddItemToObject(json, "subject_pk", subject_pk);

  char *output = cJSON_Print(json);

  if ((policy_id == NULL) || (object_pk == NULL) || (subject_pk == NULL)) {
    log_error(policy_logger_id, "[%s:%d] JSON string exceeds POLICY_JSON_MAX_LEN.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  strcpy(pol_json, output);

  free(output);
  cJSON_Delete(json);

  return POLICY_OK;

}

uint8_t policy_decode_json(unsigned char *json, policy_t *pol) {

}