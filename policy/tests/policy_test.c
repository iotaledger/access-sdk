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
#include <stdbool.h>

#include "policy.h"

static uint8_t owner_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
static uint8_t owner_sk[crypto_sign_SECRETKEYBYTES];

static uint8_t object_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
static uint8_t object_sk[crypto_sign_SECRETKEYBYTES];

static uint8_t subject_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
static uint8_t subject_sk[crypto_sign_SECRETKEYBYTES];

int main(int argc, char **argv) {

  //////////////////////////////////////
  // keypairs

  // init seeds

  uint8_t owner_seed[crypto_sign_SEEDBYTES];
  uint8_t object_seed[crypto_sign_SEEDBYTES];
  uint8_t subject_seed[crypto_sign_SEEDBYTES];

  for (int i = 0; i < crypto_sign_SEEDBYTES; i++) {
    owner_seed[i] = rand();
    object_seed[i] = rand() + 1;
    subject_seed[i] = rand() + 2;
  }

  // generate keypairs deterministically
  crypto_sign_seed_keypair(owner_pk, owner_sk, owner_seed);
  crypto_sign_seed_keypair(object_pk, object_sk, object_seed);
  crypto_sign_seed_keypair(subject_pk, subject_sk, subject_seed);

  /////////////////////////////////////////////
  // actions

  // create action_0
  action_t *action_0 = malloc(sizeof(action_t));
  action_0->action_id = "action_0";

  char *attribute_0_id = "attribute_0";
  uint8_t attribute_0_value = 0;
  char *attribute_1_id = "attribute_1";
  char attribute_1_value = '1';

  assert(action_add_attribute(action_0, attribute_0_id, &attribute_0_value) == ACTION_OK);
  assert(action_add_attribute(action_0, attribute_1_id, &attribute_1_value) == ACTION_OK);

  char *obligation_0_id = "obligation_0";
  char *obligation_1_id = "obligation_1";
  assert(action_add_obligation(action_0, obligation_0_id) == ACTION_OK);
  assert(action_add_obligation(action_0, obligation_1_id) == ACTION_OK);

  // create action_0
  action_t *action_1 = malloc(sizeof(action_t));
  action_1->action_id = "action_1";

  char *attribute_2_id = "attribute_2";
  bool attribute_2_value = true;
  char *attribute_3_id = "attribute_3";
  float attribute_3_value = 3.0;

  assert(action_add_attribute(action_1, attribute_2_id, &attribute_2_value) == ACTION_OK);
  assert(action_add_attribute(action_1, attribute_3_id, &attribute_3_value) == ACTION_OK);

  char *obligation_2_id = "obligation_2";
  char *obligation_3_id = "obligation_3";
  char *obligation_4_id = "obligation_4";
  assert(action_add_obligation(action_1, obligation_2_id) == ACTION_OK);
  assert(action_add_obligation(action_1, obligation_3_id) == ACTION_OK);
  assert(action_add_obligation(action_1, obligation_4_id) == ACTION_OK);

  ///////////////////////////////////////////////
  // policy

  // ToDo: implement policy_init
  policy_t *pol = calloc(1, sizeof(policy_t));
  memcpy(&pol->policy_body.object_pk, object_pk, crypto_sign_ed25519_PUBLICKEYBYTES);
  memcpy(&pol->policy_body.subject_pk, subject_pk, crypto_sign_ed25519_PUBLICKEYBYTES);

  assert(policy_add_action(pol, action_0) == POLICY_OK);
  assert(policy_add_action(pol, action_1) == POLICY_OK);

  free(action_0);
  free(action_1);

  assert(pol->policy_body.actions->action_id == "action_0");
  assert(pol->policy_body.actions->attributes->attribute_id == "attribute_0");
  uint8_t *derref_attribute_0_value = pol->policy_body.actions->attributes->attribute_value;
  assert(*derref_attribute_0_value == 0);

  assert(pol->policy_body.actions->attributes->next_attribute->attribute_id == "attribute_1");
  char *derref_attribute_1_value = pol->policy_body.actions->attributes->next_attribute->attribute_value;
  assert(*derref_attribute_1_value == '1');

  assert(pol->policy_body.actions->obligations->obligation_id == "obligation_0");
  assert(pol->policy_body.actions->obligations->next_obligation->obligation_id == "obligation_1");

  assert(pol->policy_body.actions->next_action->action_id == "action_1");
  assert(pol->policy_body.actions->next_action->attributes->attribute_id == "attribute_2");
  bool *derref_attribute_2_value = pol->policy_body.actions->next_action->attributes->attribute_value;
  assert(*derref_attribute_2_value == true);

  assert(pol->policy_body.actions->next_action->attributes->next_attribute->attribute_id == "attribute_3");
  float *derref_attribute_3_value = pol->policy_body.actions->next_action->attributes->next_attribute->attribute_value;
  assert(*derref_attribute_3_value == 3.0);

  assert(pol->policy_body.actions->next_action->obligations->obligation_id == "obligation_2");
  assert(pol->policy_body.actions->next_action->obligations->next_obligation->obligation_id = "obligation_3");
  assert(pol->policy_body.actions->next_action->obligations->next_obligation->next_obligation->obligation_id == "obligation_4");

  // encode policy body to binary
  char *pol_body_bin;
  size_t pol_body_bin_len;

  assert(policy_body_encode_binary(pol->policy_body, &pol_body_bin, &pol_body_bin_len) == POLICY_OK);

  // decode policy body from binary
  policy_body_t *decoded_pol_body_bin = calloc(1, sizeof(policy_body_t));
  assert(policy_decode_binary_body(pol_body_bin, pol_body_bin_len, decoded_pol_body_bin) == POLICY_OK);

  free(decoded_pol_body_bin);
  free(pol_body_bin);

  // sign policy
  assert(policy_sign(pol, owner_sk) > 0);

  // verify policy
  assert(policy_verify(pol, owner_pk) == POLICY_OK);

  // encode policy to JSON
  char pol_json[POLICY_JSON_MAX_LEN];
  assert(policy_encode_json(pol, pol_json) == POLICY_OK);

  printf("json: %s\n", pol_json);

  // decode policy JSON
  policy_t *decoded_pol_json = calloc(1, sizeof(policy_t));
  assert(policy_decode_json(pol_json, decoded_pol_json) == POLICY_OK);

  // compare values
  assert(memcmp(pol->policy_id, decoded_pol_json->policy_id, crypto_generichash_blake2b_BYTES + crypto_sign_BYTES) == 0);
  assert(memcmp(pol->policy_body.object_pk, decoded_pol_json->policy_body.object_pk, crypto_sign_ed25519_PUBLICKEYBYTES) == 0);
  assert(memcmp(pol->policy_body.subject_pk, decoded_pol_json->policy_body.subject_pk, crypto_sign_ed25519_PUBLICKEYBYTES) == 0);
  // ToDo: assert actions

  free(decoded_pol_json);

  // ToDo: implement policy_destroy

  return 0;

}
