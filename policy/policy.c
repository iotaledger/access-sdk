#include <string.h>
#include <strings.h>
#include <stdint.h>

#include "mpack.h"
#include "cJSON.h"
#include "hex.h"

#include "policy.h"
#include "policy_logger.h"

uint8_t policy_sign(policy_t *pol, uint8_t sk[]) {

  if (pol == NULL || sk == NULL) {
    log_error(policy_logger_id, "[%s:%d] invalid input.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  // serialize for crypto_generichash_blake2b
  uint8_t *ser_pb = (uint8_t*) &pol->policy_body;
  size_t ser_pb_len = sizeof(ser_pb);

  // hash serialized policy body
  uint8_t hash_pol[crypto_generichash_blake2b_BYTES];
  crypto_generichash_blake2b(hash_pol, crypto_generichash_blake2b_BYTES, ser_pb, ser_pb_len, NULL, 0);

  size_t policy_id_len;

  if (crypto_sign_ed25519(pol->policy_id, &policy_id_len, hash_pol, crypto_generichash_blake2b_BYTES, sk) != 0) {
    log_error(policy_logger_id, "[%s:%d] failed to sign policy.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  sodium_memzero(sk, crypto_sign_SECRETKEYBYTES);

  return policy_id_len;
}

uint8_t policy_verify(policy_t *pol, uint8_t pk[]) {

  // serialize for crypto_generichash_blake2b
  uint8_t *ser_pb = (uint8_t*) &pol->policy_body;
  size_t ser_pb_len = sizeof(ser_pb);

  // hash serialized policy body
  size_t hash_pol_len = crypto_generichash_blake2b_BYTES;
  uint8_t hash_pol[crypto_generichash_blake2b_BYTES];
  crypto_generichash_blake2b(hash_pol, crypto_generichash_blake2b_BYTES, ser_pb, ser_pb_len, NULL, 0);

  if (crypto_sign_ed25519_open(hash_pol, &hash_pol_len, pol->policy_id, sizeof(pol->policy_id), pk) != 0) {
    log_error(policy_logger_id, "[%s:%d] failed to verify policy.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  return POLICY_OK;
}

uint8_t policy_encode_json(policy_t *pol, unsigned char pol_json[]) {

  size_t policy_id_len = crypto_generichash_blake2b_BYTES + crypto_sign_BYTES;
  size_t object_pk_len = crypto_sign_ed25519_PUBLICKEYBYTES;
  size_t subject_pk_len = crypto_sign_ed25519_PUBLICKEYBYTES;
  // ToDo: actions

  //////////////////////////////////////////////
  // encode to hex

  size_t hex_policy_id_len = 2 * policy_id_len + 1;
  size_t hex_object_id_len = 2 * object_pk_len + 1;
  size_t hex_subject_id_len = 2 * subject_pk_len + 1;
  // ToDo: actions

  unsigned char hex_policy_id[hex_policy_id_len];
  unsigned char hex_object_pk[hex_object_id_len];
  unsigned char hex_subject_pk[hex_subject_id_len];
  // ToDo: actions

  if ((hex_encode(pol->policy_id, policy_id_len, hex_policy_id, hex_policy_id_len) == false)
      || (hex_encode(pol->policy_body.object_pk, object_pk_len, hex_object_pk, hex_object_id_len) == false)
      || (hex_encode(pol->policy_body.subject_pk, subject_pk_len, hex_subject_pk, hex_subject_id_len) == false)
      // ToDo: actions
      ) {
      log_error(policy_logger_id, "[%s:%d] failed to encode policy fields to hex.\n", __func__, __LINE__);
      return POLICY_ERROR;
  }

  //////////////////////////////////////////
  // encode JSON

  cJSON *json = cJSON_CreateObject();

  cJSON *policy_id = cJSON_CreateString(hex_policy_id);;
  cJSON *object_pk = cJSON_CreateString(hex_object_pk);;
  cJSON *subject_pk = cJSON_CreateString(hex_subject_pk);;
  // ToDo: actions

  if ((policy_id == NULL) || (object_pk == NULL) || (subject_pk == NULL)) {
    log_error(policy_logger_id, "[%s:%d] failed to encode policy fields to JSON strings.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  cJSON_AddItemToObject(json, "policy_id", policy_id);
  cJSON_AddItemToObject(json, "object_pk", object_pk);
  cJSON_AddItemToObject(json, "subject_pk", subject_pk);
  // ToDo: actions

  char *output = cJSON_Print(json);

  if ((policy_id == NULL)
      || (object_pk == NULL)
      || (subject_pk == NULL)
      // ToDo: actions
      ) {
    log_error(policy_logger_id, "[%s:%d] failed to add items to JSON object.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  if (strlen(output) > POLICY_JSON_MAX_LEN) {
    log_error(policy_logger_id, "[%s:%d] JSON string exceeds POLICY_JSON_MAX_LEN.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  strcpy(pol_json, output);

  free(output);
  cJSON_Delete(json);

  return POLICY_OK;

}

uint8_t policy_decode_json(unsigned char pol_json[], policy_t *pol) {

  cJSON *json = cJSON_Parse(pol_json);
  if (json == NULL) {
    log_error(policy_logger_id, "[%s:%d] failed to parse Policy JSON.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  const cJSON *hex_policy_id = cJSON_GetObjectItem(json, "policy_id");
  const cJSON *hex_object_pk = cJSON_GetObjectItem(json, "object_pk");
  const cJSON *hex_subject_pk = cJSON_GetObjectItem(json, "subject_pk");
  // ToDo: actions

  uint8_t policy_id[crypto_generichash_blake2b_BYTES + crypto_sign_BYTES];
  uint8_t object_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
  uint8_t subject_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
  // ToDo: actions

  hex_decode(hex_policy_id->valuestring, strlen(hex_policy_id->valuestring), policy_id, crypto_generichash_blake2b_BYTES + crypto_sign_BYTES);
  hex_decode(hex_object_pk->valuestring, strlen(hex_object_pk->valuestring), object_pk, crypto_sign_ed25519_PUBLICKEYBYTES);
  hex_decode(hex_subject_pk->valuestring, strlen(hex_subject_pk->valuestring), subject_pk, crypto_sign_ed25519_PUBLICKEYBYTES);
  // ToDo: actions

  memcpy(pol->policy_id, policy_id, crypto_generichash_blake2b_BYTES + crypto_sign_BYTES);
  memcpy(pol->policy_body.object_pk, object_pk, crypto_sign_ed25519_PUBLICKEYBYTES);
  memcpy(pol->policy_body.subject_pk, subject_pk, crypto_sign_ed25519_PUBLICKEYBYTES);

  return POLICY_OK;
}

uint8_t policy_body_encode_binary(policy_body_t pol_body, char **pol_body_bin, size_t *pol_body_bin_len) {

  char *data;
  size_t data_len;
  mpack_writer_t writer;
  mpack_writer_init_growable(&writer, &data, &data_len);

  mpack_start_map(&writer, 2); //ToDo: count = 3, action list

  mpack_write_cstr(&writer, "object_pk");
  mpack_write_bin(&writer, pol_body.object_pk, crypto_sign_ed25519_PUBLICKEYBYTES);
  mpack_write_cstr(&writer, "subject_pk");
  mpack_write_bin(&writer, pol_body.subject_pk, crypto_sign_ed25519_PUBLICKEYBYTES);
  // ToDo: write action list

  mpack_finish_map(&writer);

  if (mpack_writer_destroy(&writer) != mpack_ok) {
    log_error(policy_logger_id, "[%s:%d] failed to encode Policy body to binary.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  *pol_body_bin = (char*)malloc(data_len);
  memcpy(*pol_body_bin, data, data_len);
  memcpy(pol_body_bin_len, &data_len, sizeof(size_t));

  return POLICY_OK;
}

uint8_t policy_decode_binary_body(char *pol_body_bin, size_t pol_body_bin_size, policy_body_t *pol_body) {

  mpack_tree_t tree;
  mpack_tree_init_data(&tree, pol_body_bin, pol_body_bin_size);
  mpack_tree_parse(&tree);
  mpack_node_t root = mpack_tree_root(&tree);

  if (mpack_tree_destroy(&tree) != mpack_ok) {
    log_error(policy_logger_id, "[%s:%d] failed to deencode Policy body from binary.\n", __func__, __LINE__);
    return POLICY_ERROR;
  }

  memcpy(pol_body->object_pk, mpack_node_bin_data(mpack_node_map_cstr(root, "object_pk")), crypto_sign_ed25519_PUBLICKEYBYTES);
  memcpy(pol_body->subject_pk, mpack_node_bin_data(mpack_node_map_cstr(root, "subject_pk")), crypto_sign_ed25519_PUBLICKEYBYTES);
  // ToDo: parse actions

  return POLICY_OK;
}