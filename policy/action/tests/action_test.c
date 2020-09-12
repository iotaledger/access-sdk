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
#include <stdlib.h>
#include <stdio.h>

#include "action.h"

int main(int argc, char **argv) {

  // ToDo: implement action_init
  action_t *action_0 = malloc(sizeof(action_t));
  action_0->action_id = "action_0";

  char *attribute_0_id = "attribute_0";
  uint8_t attribute_0_value = 0;
  char *attribute_1_id = "attribute_1";
  float attribute_1_value = 1.0;

  assert(action_add_attribute(action_0, attribute_0_id, &attribute_0_value, ATTR_NUMBER) == ACTION_OK);
  assert(action_add_attribute(action_0, attribute_1_id, &attribute_1_value, ATTR_NUMBER) == ACTION_OK);
  assert(action_0->attributes->attribute_id == "attribute_0");
  assert(action_0->attributes->next_attribute->attribute_id == "attribute_1");

  uint8_t *derref_attribute_0_value = action_0->attributes->attribute_value;
  float *derref_attribute_1_value = action_0->attributes->next_attribute->attribute_value;
  assert(*derref_attribute_0_value == attribute_0_value);
  assert(*derref_attribute_1_value == attribute_1_value);

  char *obligation_0_id = "obligation_0";
  char *obligation_1_id = "obligation_1";
  assert(action_add_obligation(action_0, obligation_0_id) == ACTION_OK);
  assert(action_add_obligation(action_0, obligation_1_id) == ACTION_OK);

  assert(action_0->obligations->obligation_id == obligation_0_id);
  assert(action_0->obligations->next_obligation->obligation_id == obligation_1_id);

  // ToDo: implement action_init
  action_t *action_1 = malloc(sizeof(action_t));
  action_1->action_id = "action_1";
  action_0->next_action = action_1;

  char *action_json;
  assert(action_encode_json(action_0, action_json) == ACTION_OK);

  printf("%s\n", action_json);

  // ToDo: implement action_destroy

  return 0;

}
