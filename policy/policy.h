#include <stddef.h>

#include "sodium.h"

#ifndef POLICY_H
#define POLICY_H

#define POLICY_ID_LEN 64 // todo: verify this value
#define POLICY_SIG_LEN 64 // todo: verify this value

#define POLICY_OK 0
#define POLICY_ERROR 1

typedef enum {
  NULL_FEE,
  ONE_TIME_FEE,
  PER_USE_FEE
} fee_t;

typedef struct {
  char *obligation_id;
} obligation_t;

typedef struct {
  char *action_id;

  fee_t fee;
  unsigned long tx_value;
  char *tx_addr;
  char *tx_hash;
  size_t tx_hash_len;

  void *attributes;
  obligation_t *obligations;
} action_t;

typedef struct {
  char policy_id[POLICY_ID_LEN];
  char policy_sig[POLICY_SIG_LEN];

  uint8_t object_pk[crypto_sign_PUBLICKEYBYTES];
  uint8_t subject_pk[crypto_sign_PUBLICKEYBYTES];

  action_t *actions;
} policy_t;

#endif //POLICY_H
