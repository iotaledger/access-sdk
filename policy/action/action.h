#ifndef ACTION_H
#define ACTION_H

#include <stdint.h>

#define ACTION_OK 0
#define ACTION_ERROR -1

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
 * @brief attribute struct type
 */
typedef struct attribute {

  /*@{*/
  struct attribute *next_attribute;
  char *attribute_id; /**< attribute identifier */
  void *attribute_value; /**< attribute pointer */
  /*@}*/

} attribute_t;

/**
 * @brief obligation struct type
 */
typedef struct obligation {

  /*@{*/
  struct obligation * next_obligation; /**< pointer to next obligation */
  char *obligation_id; /**< obligation identifier */
  /*@}*/

} obligation_t;

#define TX_ADDR_LEN 512
#define TX_HASH_LEN 512

/**
 * @brief action struct type
 */
typedef struct action {

  /*@{*/
  struct action *next_action; /**< pointer to next action */
  char *action_id; /**< action identifier */
  /*@}*/

//  /*@{*/
//  fee_t fee; /**< fee type */
//  unsigned long tx_value; /**< (optional) fee tx value */
//  char tx_addr[TX_ADDR_LEN]; /**< (optional) fee tx address */
//  char tx_hash[TX_HASH_LEN]; /**< (optional) fee tx hash */
//  /*@}*/

  /*@{*/
  attribute_t *attributes; /**< (optional) attribute list */
  obligation_t *obligations; /**< (optional) obligation list */
  /*@}*/

} action_t;

/**
 * @brief initialize action_t
 * @param action pointer to action_t
 * @return ACTION_OK or ACTION_ERROR
 */
uint8_t action_init(action_t *action);

/**
 * @brief destroy action_t
 * @param action pointer to action_t
 * @return ACTION_OK or ACTION_ERROR
 */
uint8_t action_destroy(action_t *action);

/**
 * @brief add new attribute to action_t
 * @param action pointer to action_t
 * @param attribute_id string with attribute id
 * @param attribute_value void pointer with attribute value
 * @return
 */
uint8_t action_add_attribute(action_t *action, char *attribute_id, void *attribute_value);

/**
 * @brief add obligation_t to action_t
 * @param action pointer to action_t
 * @param obligation pointer to obligation_t
 * @return ACTION_OK or ACTION_ERROR
 */
uint8_t action_add_obligation(action_t *action, char *obligation_id);

uint8_t action_encode_json(action_t *action, unsigned char action_json[]);

#endif  // ACTION_H