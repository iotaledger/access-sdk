#include <stddef.h>

#include "sodium.h"

#ifndef POLICY_H
#define POLICY_H

#define POLICY_OK 0
#define POLICY_ERROR 1

/**
 * @brief max length of JSON encoding
 */
#define POLICY_JSON_MAX_LEN 512

/**
 * @brief fee enum type
 */
typedef enum {

  /*@{*/
  NULL_FEE, /**< no fee */
  ONE_TIME_FEE, /**< one time fee */
  PER_USE_FEE /**< per use fee */
  /*@}*/

} fee_t;

/**
 * @brief obligation struct type
 */
typedef struct {

  /*@{*/
  char *obligation_id; /**< obligation identifier */
  void *attributes; /**< obligation attributes */
  /*@}*/

} obligation_t;

/**
 * @brief action struct type
 */
typedef struct {

  /*@{*/
  char *action_id; /**< action identifier */
  /*@}*/

  /*@{*/
  fee_t fee; /**< fee type */
  unsigned long tx_value; /**< (optional) fee tx value */
  char *tx_addr; /**< (optional) fee tx address */
  char *tx_hash; /**< (optional) fee tx hash */
  /*@}*/

  /*@{*/
  void *attributes; /**< (optional) attribute list */
  obligation_t *obligations; /**< (optional) obligation list */
  /*@}*/

} action_t;

/**
 * @brief policy struct type
 */
typedef struct {

  /*@{*/
  uint8_t policy_id[crypto_generichash_blake2b_BYTES + crypto_sign_BYTES]; /**< policy identifier (signed hash) */
  /*@}*/

  /*@{*/
  uint8_t object_pk[crypto_sign_ed25519_PUBLICKEYBYTES]; /**< object identifier (public key) */
  uint8_t subject_pk[crypto_sign_ed25519_PUBLICKEYBYTES]; /**< subject identifier (public key) */
  /*@}*/

  /*@{*/
  action_t *actions; /**< action list */
  /*@}*/

} policy_t;

/**
 * @brief sign policy
 *
 * warning: for security reasons, the secret key is erased from memory before this function returs.
 *
 * @param pol policy_t struct
 * @param sk signer secret key
 * @return POLICY_OK or POLICY_ERROR
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
uint8_t policy_decode_json(unsigned char *pol_json, policy_t *pol);

#endif //POLICY_H
