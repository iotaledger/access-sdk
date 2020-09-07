#include <stddef.h>

#include "sodium.h"

#ifndef POLICY_H
#define POLICY_H

#define POLICY_OK 0
#define POLICY_ERROR 1

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
  char policy_id[crypto_generichash_BYTES + crypto_sign_BYTES]; /**< policy identifier (signed hash) */
  /*@}*/

  /*@{*/
  uint8_t object_pk[crypto_sign_PUBLICKEYBYTES]; /**< object identifier (public key) */
  uint8_t subject_pk[crypto_sign_PUBLICKEYBYTES]; /**< subject identifier (public key) */
  /*@}*/

  /*@{*/
  action_t *actions; /**< action list */
  /*@}*/

} policy_t;

#endif //POLICY_H
