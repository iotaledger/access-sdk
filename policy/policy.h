#ifndef POLICY_H
#define POLICY_H

#include <stddef.h>

#include "sodium.h"

#include "action.h"

#define POLICY_OK 0
#define POLICY_ERROR -1

/**
 * @brief max length of JSON encoding
 */
#define POLICY_JSON_MAX_LEN 512

/**
 * @brief policy body struct type
 */
typedef struct {

  /*@{*/
  uint8_t object_pk[crypto_sign_ed25519_PUBLICKEYBYTES]; /**< object identifier (public key) */
  uint8_t subject_pk[crypto_sign_ed25519_PUBLICKEYBYTES]; /**< subject identifier (public key) */
  /*@}*/

  /*@{*/
  action_t *actions; /**< action list */
  /*@}*/

} policy_body_t;

/**
 * @brief policy struct type
 */
typedef struct {

  /*@{*/
  uint8_t policy_id[crypto_generichash_blake2b_BYTES + crypto_sign_BYTES]; /**< policy identifier (signed hash) */
  /*@}*/

  /*@{*/
  policy_body_t policy_body;
  /*@}*/

} policy_t;

/**
 * @brief policy initializer
 * @param pol pointer to policy_t
 * @return POLICY_OK or POLICY_ERROR
 */
uint8_t policy_init(policy_t *pol);

/**
 * @brief destroy policy_t
 * @param pol pointer to policy_t
 * @return POLICY_OK or POLICY_ERROR
 */
uint8_t policy_destroy(policy_t *pol);

/**
 * @brief add action_t to policy_t
 * @param pol pointer to policy_t
 * @param action pointer to action_t
 * @return POLICY_OK or POLICY_ERROR
 */
uint8_t policy_add_action(policy_t *pol, action_t *action);

/**
 * @brief sign policy
 *
 * warning: for security reasons, the secret key is erased from memory before this function returs.
 *
 * @param pol policy_t struct
 * @param sk signer secret key
 * @return policy_id_len or POLICY_ERROR
 */
uint8_t policy_sign(policy_t *pol, uint8_t sk[]);

/**
 * @brief verify policy signature
 *
 * @param pol policy_t struct
 * @param pk signer public key
 * @return POLICY_OK or POLICY_ERROR
 */
uint8_t policy_verify(policy_t *pol, uint8_t pk[]);

/**
 * @brief encode policy struct into JSON
 * @param pol pointer to policy_t struct (input)
 * @param pol_json string with JSON (output)
 * @return POLICY_OK or POLICY_ERROR
 */
uint8_t policy_encode_json(policy_t *pol, unsigned char pol_json[]);

/**
 * @brief decode policy struct from JSON
 * @param pol_json string with JSON (input)
 * @param pol pointer to policy_t struct (output)
 * @return POLICY_OK or POLICY_ERROR
 */
uint8_t policy_decode_json(unsigned char pol_json[], policy_t *pol);

uint8_t policy_encode_binary(policy_t *pol, char *pol_bin, size_t *pol_bin_size);

uint8_t policy_body_encode_binary(policy_body_t pol_body, char **pol_body_bin, size_t *pol_body_bin_size);

uint8_t policy_decode_binary(char *pol_bin, size_t *pol_bin_size, policy_t *pol);

uint8_t policy_decode_binary_body(char *pol_body_bin, size_t pol_body_bin_size, policy_body_t *pol_body);

#endif //POLICY_H
