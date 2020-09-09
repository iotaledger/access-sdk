/*
 * This file is part of the Frost distribution
 * (https://github.com/xainag/frost)
 *
 * Copyright (c) 2020 IOTA Stiftung
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "libbase58.h"

#include "policy.h"

static uint8_t owner_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
static uint8_t owner_sk[crypto_sign_SECRETKEYBYTES];

static uint8_t object_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
static uint8_t object_sk[crypto_sign_SECRETKEYBYTES];

static uint8_t subject_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
static uint8_t subject_sk[crypto_sign_SECRETKEYBYTES];

int main(int argc, char **argv) {

  // generate keypairs
  crypto_sign_keypair(owner_pk, owner_sk);
  crypto_sign_keypair(object_pk, object_sk);
  crypto_sign_keypair(subject_pk, subject_sk);

  uint8_t *subject_pk_tmp = subject_pk;

  //create policy
  policy_t *pol = calloc(1, sizeof(policy_t));
  memcpy(&pol->policy_body.object_pk, object_pk, crypto_sign_ed25519_PUBLICKEYBYTES);
  memcpy(&pol->policy_body.subject_pk, subject_pk, crypto_sign_ed25519_PUBLICKEYBYTES);

  // sign policy
  assert(policy_sign(pol, owner_sk) > 0);

  // verify policy
  assert(policy_verify(pol, owner_pk) == POLICY_OK);

  // encode policy to JSON
  char pol_json[POLICY_JSON_MAX_LEN];
  assert(policy_encode_json(pol, pol_json) == POLICY_OK);

  printf("json: %s\n", pol_json);

  return 0;

}
