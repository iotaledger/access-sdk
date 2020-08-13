/*
 * This file is part of the IOTA Access Distribution
 * (https://github.com/iotaledger/access)
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

/****************************************************************************
 * \project IOTA Access
 * \file pdp.c
 * \brief
 * Implementation of Policy Decision Point
 *
 * @Author Dejan Nedic, Milivoje Knezevic, Strahinja Golic
 *
 *
 * \notes
 *
 * \history
 * 04.09.2018. Initial version.
 * 09.11.2018 Modified to work together with PIP module
 * 21.02.2020. Added obligations functionality.
 * 05.05.20200 Refactoring
 ****************************************************************************/

/****************************************************************************
 * INCLUDES
 ****************************************************************************/
#include "pdp.h"
#include "pdp_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_helper.h"
#include "pip.h"

/****************************************************************************
 * MACROS
 ****************************************************************************/
#define PDP_DATA_VAL_SIZE 131
#define PDP_DATA_TYPE_SIZE 21
#define PDP_STRTOUL_BASE 10
#define PDP_USER_LEN 256
#define PDP_WALLET_ADDR_LEN 81

/****************************************************************************
 * LOCAL FUNCTIONS DECLARATION
 ****************************************************************************/
static int resolve_attribute(char *pol, char *policy_id, int atribute_position, char *user);

/****************************************************************************
 * LOCAL FUNCTIONS DEFINITION
 ****************************************************************************/
static pdp_operation_e get_operation_new(const char *operation, int size) {
  pdp_operation_e ret = UNDEFINED;

  if (size == 2) {
    if (memcmp("or", operation, size) == 0) {
      ret = OR;
    } else if (memcmp("eq", operation, size) == 0) {
      ret = EQ;
    } else if (memcmp("lt", operation, size) == 0) {
      ret = LT;
    } else if (memcmp("gt", operation, size) == 0) {
      ret = GT;
    } else if (memcmp("if", operation, size) == 0) {
      ret = IF;
    }
  } else if (size == 3) {
    if (memcmp("and", operation, size) == 0) {
      ret = AND;
    } else if (memcmp("not", operation, size) == 0) {
      ret = NOT;
    } else if (memcmp("leq", operation, size) == 0) {
      ret = LEQ;
    } else if (memcmp("geq", operation, size) == 0) {
      ret = GEQ;
    }
  }

  return ret;
}

static int and (char *policy_object, char *policy_id, int attribute_list, char *user) {
  int decision = TRUE;

  int i = 0;
  int res;
  int attribute_count = 0;
  int attribute = -1;

  attribute_count = jsonhelper_array_size(attribute_list);

  for (i = 0; i < attribute_count; i++) {
    attribute = jsonhelper_get_attribute_from_array(attribute_list, i);

    res = resolve_attribute(policy_object, policy_id, attribute_list + attribute, user);

    if (res != FALSE) {
      decision = decision && res;
    } else {
      decision = FALSE;
      break;
    }
  }

  return decision;
}

static int or (char *policy_object, char *policy_id, int attribute_list, char *user) {
  int decision = FALSE;

  int i = 0;
  int res;
  int attribute_count = 0;
  int attribute = -1;

  attribute_count = jsonhelper_array_size(attribute_list);

  for (i = 0; i < attribute_count; i++) {
    attribute = jsonhelper_get_attribute_from_array(attribute_list, i);

    res = resolve_attribute(policy_object, policy_id, attribute_list + attribute, user);

    if (res != FALSE) {
      decision = decision || res;
    } else {
      decision = FALSE;
      break;
    }
  }

  return decision;
}

static int resolve_condition(char *policy_object, char *policy_id, int attribute_list, pdp_operation_e condition, char *user) {
  char uri[PDP_MAX_STR_LEN] = {0};
  char data_value[PDP_DATA_VAL_SIZE] = {0};
  char data_type[PDP_DATA_TYPE_SIZE] = {0};
  int ret = FALSE;
  int attr1 = -1;
  int attr2 = -1;
  int type1 = -1;
  int value1 = -1;
  int size_of_type1 = -1;
  int size_of_value1 = -1;
  int type2 = -1;
  int value2 = -1;
  int size_of_type2 = -1;
  int size_of_value2 = -1;
  pip_attribute_object_t attribute;

  memset(attribute.type, 0, sizeof(char) * PIP_MAX_STR_LEN);
  memset(attribute.value, 0, sizeof(char) * PIP_MAX_STR_LEN);

  attr1 = jsonhelper_get_attribute_from_array(attribute_list, 0);
  attr2 = jsonhelper_get_attribute_from_array(attribute_list, 1);

  type1 = jsonhelper_get_value(policy_object, attribute_list + attr1, "type");
  type2 = jsonhelper_get_value(policy_object, attribute_list + attr2, "type");
  value1 = jsonhelper_get_value(policy_object, attribute_list + attr1, "value");
  value2 = jsonhelper_get_value(policy_object, attribute_list + attr2, "value");

  memcpy(data_type, policy_object + jsonhelper_get_token_start(type2), jsonhelper_token_size(type2));
  memcpy(data_value, policy_object + jsonhelper_get_token_start(value2), jsonhelper_token_size(value2));

  if (memcmp(data_type, "request.subject.type", strlen("request.subject.type")) == 0 &&
      memcmp(data_value, "request.subject.value", strlen("request.subject.value")) == 0) {
        /* TODO: This is a temporary solution, untill authentication of the user is implemented.
           When it's done, PIP module will use credentials, in order to verify subject. */
        memcpy(attribute.type, "public_id", strlen("public_id"));
        memcpy(attribute.value, "0x", strlen("0x"));
        memcpy(&attribute.value[strlen("0x")], user, strlen(user));
  } else if (memcmp(data_type, "request", strlen("request")) == 0 &&
             memcmp(data_value, "request", strlen("request")) == 0) { // Request for PIP
    // If any authority other than iota is to be supported, this needs to be modified
    sprintf(uri, "iota:%s/%s?%s", policy_id, data_type, data_value);

    if (pip_get_data(uri, &attribute) != PIP_NO_ERROR) {
      log_error(pdp_logger_id, "[%s:%d] Getting data from PIP failed.\n", __func__, __LINE__);
      return FALSE;
    }
  } else { // No request, just copy attribute 2
      memcpy(attribute.type, data_type, jsonhelper_token_size(type2));
      memcpy(attribute.value, data_value, jsonhelper_token_size(value2));
  }

  size_of_type1 = jsonhelper_token_size(type1);
  size_of_type2 = strlen(attribute.type);
  size_of_value1 = jsonhelper_token_size(value1);
  size_of_value2 = strlen(attribute.value);

  switch (condition) {
    case EQ: {
      if (((size_of_type1 == size_of_type2) &&
            (strncasecmp(policy_object + jsonhelper_get_token_start(type1), attribute.type, size_of_type1) == 0)) &&
          ((size_of_value1 == size_of_value2) &&
            (strncasecmp(policy_object + jsonhelper_get_token_start(value1), attribute.value, size_of_value1) == 0))) {
        ret = TRUE;
      } else {
        log_info(pdp_logger_id, "[%s:%d] PDP condition FALSE.\n", __func__, __LINE__);
      }
      break;
    }
    case LEQ: {
      if (((size_of_type1 == size_of_type2) &&
            (strncasecmp(policy_object + jsonhelper_get_token_start(type1), attribute.type, size_of_type1) == 0))) {
        if (size_of_value1 < size_of_value2) {
          ret = TRUE;
        } else if ((size_of_value1 == size_of_value2) &&
                    strncasecmp(policy_object + jsonhelper_get_token_start(value1), attribute.value, size_of_value1) <=
                        0) {
          ret = TRUE;
        }
      }
      break;
    }
    case LT: {
      if (((size_of_type1 == size_of_type2) &&
            (strncasecmp(policy_object + jsonhelper_get_token_start(type1), attribute.type, size_of_type1) == 0))) {
        if (size_of_value1 < size_of_value2) {
          ret = TRUE;
        } else if ((size_of_value1 == size_of_value2) &&
                    strncasecmp(policy_object + jsonhelper_get_token_start(value1), attribute.value, size_of_value1) <
                        0) {
          ret = TRUE;
        }
      }
      break;
    }
    case GEQ: {
      if (((size_of_type1 == size_of_type2) &&
            (strncasecmp(policy_object + jsonhelper_get_token_start(type1), attribute.type, size_of_type1) == 0))) {
        if (size_of_value1 > size_of_value2) {
          ret = TRUE;
        } else if ((size_of_value1 == size_of_value2) &&
                    strncasecmp(policy_object + jsonhelper_get_token_start(value1), attribute.value, size_of_value1) >=
                        0) {
          ret = TRUE;
        }
      }
      break;
    }
    case GT: {
      if (((size_of_type1 == size_of_type2) &&
            (strncasecmp(policy_object + jsonhelper_get_token_start(type1), attribute.type, size_of_type1) == 0))) {
        if (size_of_value1 > size_of_value2) {
          ret = TRUE;
        } else if ((size_of_value1 == size_of_value2) &&
                    strncasecmp(policy_object + jsonhelper_get_token_start(value1), attribute.value, size_of_value1) >
                        0) {
          ret = TRUE;
        }
      }
      break;
    }
    default:
      break;
  }

  return ret;
}

static void get_time_from_attr(char *policy_object, int atribute_position, pdp_operation_e attr_operation,
                               unsigned long *start_time, unsigned long *end_time) {
  if (policy_object == NULL) {
    log_error(pdp_logger_id, "[%s:%d] Wrong input parameters.\n", __func__, __LINE__);
    return;
  }

  if (start_time == NULL || end_time == NULL) {
    // Not an error. Sometimes, time is not needed
    return;
  }

  int operation = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "operation");
  int operation_start = -1;
  int operation_end = -1;
  int attribute_list = -1;
  int i = 0;
  pdp_operation_e opt;

  if ((operation != -1) &&
      (jsonhelper_get_token_start(operation) <
       jsonhelper_get_token_end(atribute_position)))  // Check only operations within this json object
  {
    attribute_list = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "attribute_list");

    // Check if operation is listed before or after attribure list
    if (attribute_list >= 0 && operation > attribute_list) {
      // If attribute list is listed first, get operation given after att. list

      int number_of_tokens = jsonhelper_get_token_num();
      int tok_cnt = attribute_list;

      while ((jsonhelper_get_token_end(attribute_list) > jsonhelper_get_token_start(operation)) &&
             (tok_cnt <= number_of_tokens) && (tok_cnt >= 0)) {
        operation = jsonhelper_get_token_index_from_pos(policy_object, tok_cnt, "operation");
        tok_cnt = operation;
      }
    }

    operation_start = jsonhelper_get_token_start(operation);
    operation_end = jsonhelper_get_token_end(operation);

    opt = get_operation_new(policy_object + operation_start, operation_end - operation_start);

    i = 0;
    while (i < jsonhelper_array_size(attribute_list)) {
      get_time_from_attr(policy_object, jsonhelper_get_array_element(attribute_list, i), opt, start_time, end_time);
      i++;
    }
  } else {
    int type = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "type");
    if ((type != -1) && (jsonhelper_get_token_start(type) <
                         jsonhelper_get_token_end(atribute_position)))  // Check only type within this json object
    {
      int start = jsonhelper_get_token_start(type);
      int size_of_type = jsonhelper_token_size(type);

      if ((size_of_type == strlen("time")) && (strncasecmp(policy_object + start, "time", strlen("time")) == 0)) {
        int value = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "value");
        int start_of_value = jsonhelper_get_token_start(value);
        int size_of_value = jsonhelper_token_size(value);
        char *val_str = malloc(sizeof(char) * size_of_value + 1);
        memcpy(val_str, policy_object + start_of_value, size_of_value);
        val_str[size_of_value] = '\0';

        switch (attr_operation) {
          case EQ: {
            *start_time = strtoul(val_str, NULL, PDP_STRTOUL_BASE);
            *end_time = *start_time;
            break;
          }
          case LEQ: {
            *end_time = strtoul(val_str, NULL, PDP_STRTOUL_BASE);
            break;
          }
          case GEQ: {
            *start_time = strtoul(val_str, NULL, PDP_STRTOUL_BASE);
            break;
          }
          case LT: {
            *end_time = strtoul(val_str, NULL, PDP_STRTOUL_BASE) - 1;  // Must be less then value
            break;
          }
          case GT: {
            *start_time = strtoul(val_str, NULL, PDP_STRTOUL_BASE) + 1;  // Must be greater then value
            break;
          }
          default: { break; }
        }

        free(val_str);
      }
    }
  }
}

static int resolve_attribute(char *policy_object, char *policy_id, int atribute_position, char *user) {
  int ret = FALSE;
  pdp_operation_e opt;

  int operation = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "operation");
  int operation_start = -1;
  int operation_end = -1;

  int attribute_list = -1;

  if ((operation != -1) &&
      (jsonhelper_get_token_start(operation) <
       jsonhelper_get_token_end(atribute_position)))  // Check only operations within this json object
  {
    attribute_list = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "attribute_list");

    // Check if operation is listed before or after attribure list
    if (attribute_list >= 0 && operation > attribute_list) {
      // If attribute list is listed first, get operation given after att. list

      int number_of_tokens = jsonhelper_get_token_num();
      int tok_cnt = attribute_list;

      while ((jsonhelper_get_token_end(attribute_list) > jsonhelper_get_token_start(operation)) &&
             (tok_cnt <= number_of_tokens) && (tok_cnt >= 0)) {
        operation = jsonhelper_get_token_index_from_pos(policy_object, tok_cnt, "operation");
        tok_cnt = operation;
      }
    }

    operation_start = jsonhelper_get_token_start(operation);
    operation_end = jsonhelper_get_token_end(operation);

    opt = get_operation_new(policy_object + operation_start, operation_end - operation_start);

    switch (opt) {
      case OR: {
        ret = or (policy_object, policy_id, attribute_list, user);
        break;
      }
      case AND: {
        ret = and(policy_object, policy_id, attribute_list, user);
        break;
      }
      case EQ: {
        ret = resolve_condition(policy_object, policy_id, attribute_list, EQ, user);
        break;
      }
      case LEQ: {
        ret = resolve_condition(policy_object, policy_id, attribute_list, LEQ, user);
        break;
      }
      case GEQ: {
        ret = resolve_condition(policy_object, policy_id, attribute_list, GEQ, user);
        break;
      }
      case LT: {
        ret = resolve_condition(policy_object, policy_id, attribute_list, LT, user);
        break;
      }
      case GT: {
        ret = resolve_condition(policy_object, policy_id, attribute_list, GT, user);
        break;
      }
      default: {
        ret = FALSE;
        break;
      }
    }
  } else {
    int type = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "type");
    if ((type != -1) && (jsonhelper_get_token_start(type) <
                         jsonhelper_get_token_end(atribute_position)))  // Check only type within this json object
    {
      int start = jsonhelper_get_token_start(type);
      int size_of_type = jsonhelper_token_size(type);

      if ((size_of_type == strlen("boolean")) &&
          (strncasecmp(policy_object + start, "boolean", strlen("boolean")) == 0)) {
        int value = jsonhelper_get_token_index_from_pos(policy_object, atribute_position, "value");
        int start_of_value = jsonhelper_get_token_start(value);
        int size_of_value = jsonhelper_token_size(value);
        if ((size_of_value >= strlen("true")) &&
            (memcmp(policy_object + start_of_value, "true", strlen("true")) == 0)) {
          ret = TRUE;
        } else {
          ret = FALSE;
        }
      }
    }
  }

  return ret;
}

static int get_obligation(char *policy_object, int obl_position, char *obligation) {
  int ret = PDP_ERROR_RET;

  int type = jsonhelper_get_token_index_from_pos(policy_object, obl_position, "type");
  if ((type != -1) && (jsonhelper_get_token_start(type) <
                       jsonhelper_get_token_end(obl_position)))  // Check only type within this json object
  {
    int start = jsonhelper_get_token_start(type);
    int size_of_type = jsonhelper_token_size(type);

    if ((size_of_type == strlen("obligation")) &&
        (!strncasecmp(policy_object + start, "obligation", strlen("obligation")))) {
      int value = jsonhelper_get_token_index_from_pos(policy_object, obl_position, "value");
      int start_of_value = jsonhelper_get_token_start(value);
      int size_of_value = jsonhelper_token_size(value);

      if (size_of_value > PDP_OBLIGATION_LEN) {
        size_of_value = PDP_OBLIGATION_LEN;
      }

      if (value >= 0) {
        memcpy(obligation, policy_object + start_of_value, size_of_value);
        ret = value;
      }
    }
  }

  return ret;
}

// TODO: obligations should be linked list of the elements of the 'obligation_s' structure type
static int resolve_obligation(char *policy_object, char *policy_id, char *user, int obl_position, char *obligation) {
  int ret = PDP_ERROR_RET;
  int operation = -1;
  int attribute_list = -1;
  int operation_start = -1;
  int operation_end = -1;
  int obl_value = -1;
  pdp_operation_e opt;

  if (policy_object == NULL || obligation == NULL) {
    log_error(pdp_logger_id, "[%s:%d] Wrong input parameters.\n", __func__, __LINE__);
    return ret;
  }

  // Size of obligation buff is 15
  memset(obligation, 0, sizeof(char) * PDP_OBLIGATION_LEN);

  operation = jsonhelper_get_token_index_from_pos(policy_object, obl_position, "operation");
  attribute_list = jsonhelper_get_token_index_from_pos(policy_object, obl_position, "attribute_list");
  obl_value = jsonhelper_get_token_index_from_pos(policy_object, obl_position, "obligations");

  // In case of IF operation, multiple obligations are available
  if ((attribute_list >= 0) && (jsonhelper_get_token_start(attribute_list) < jsonhelper_get_token_end(obl_position)) &&
      (operation >= 0) && (jsonhelper_get_token_start(operation) < jsonhelper_get_token_end(obl_position))) {
    // Check if operation is listed before or after attribure list
    if (operation > attribute_list) {
      // If attribute list is listed first, get operation given after att. list

      int number_of_tokens = jsonhelper_get_token_num();
      int tok_cnt = attribute_list;

      while ((jsonhelper_get_token_end(attribute_list) > jsonhelper_get_token_start(operation)) &&
             (tok_cnt <= number_of_tokens) && (tok_cnt >= 0)) {
        operation = jsonhelper_get_token_index_from_pos(policy_object, tok_cnt, "operation");
        tok_cnt = operation;
      }
    }

    operation_start = jsonhelper_get_token_start(operation);
    operation_end = jsonhelper_get_token_end(operation);

    opt = get_operation_new(policy_object + operation_start, operation_end - operation_start);

    // TODO: For now, only IF operation is supported
    switch (opt) {
      case IF:
        if (!resolve_attribute(policy_object, policy_id, attribute_list, user)) {
          // Take second obligation if condition is false (else branch)
          obl_value = jsonhelper_get_token_index_from_pos(policy_object, obl_value + 1, "obligations");
        }

        break;

      default:
        break;
    }
  }

  if (obl_value >= 0) {
    ret = get_obligation(policy_object, obl_value, obligation);
  }

  return ret;
}

/****************************************************************************
 * API FUNCTIONS
 ****************************************************************************/
bool pdp_init() {
  // Initialize PAP
  if (pap_init() == PAP_ERROR) {
    printf("\nERROR[%s]: PAP initialization failed.\n", __FUNCTION__);
    return FALSE;
  }

  // Initialize PIP
  if (pip_init() == PIP_ERROR) {
    printf("\nERROR[%s]: PIP initialization failed.\n", __FUNCTION__);
    return FALSE;
  }

  return TRUE;
}

bool pdp_term(void) {
  // Terminate PAP
  if (pap_term() == PAP_ERROR) {
    printf("\nERROR[%s]: PAP termination failed.\n", __FUNCTION__);
    return FALSE;
  }

  // Terminate PIP
  if (pip_term() == PIP_ERROR) {
    printf("\nERROR[%s]: PIP termination failed.\n", __FUNCTION__);
    return FALSE;
  }

  return TRUE;
}

// TODO: obligations should be linked list of the elements of the 'obligation_s' structure type
pdp_decision_e pdp_calculate_decision(char *request_norm, char *obligation, pdp_action_t *action) {
  char user_str[PDP_USER_LEN] = {0};
  char *policy_object = NULL;
  int request_policy_id = -1;
  int request_policy_list = -1;
  int request_balance = -1;
  int request_wallet_addr = -1;
  int user = -1;
  int tr_hash = -1;
  int size = -1;
  int pol_obj_len = 0;
  pdp_decision_e ret = PDP_ERROR;
  pap_policy_t policy;

  // Check input parameters
  if (request_norm == NULL || obligation == NULL || action == NULL || action->value == NULL) {
    log_error(pdp_logger_id, "[%s:%d] Invalid input parameters.\n", __func__, __LINE__);
    return ret;
  }

  jsonhelper_parser_init(request_norm);

  // Get user id
  user = jsonhelper_get_value(request_norm, 0, "user_id");
  if (user == -1) {
    log_error(pdp_logger_id, "[%s:%d] Invalid request. No user id.\n", __func__, __LINE__);
    return ret;
  }
  else {
    memcpy(user_str, request_norm + jsonhelper_get_token_start(user), jsonhelper_token_size(user));
  }

  // Check if get_policy_list request is received
  request_policy_list = jsonhelper_get_value(request_norm, 0, "cmd");
  if ((request_policy_list != -1) &&
      (memcmp(request_norm + jsonhelper_get_token_start(request_policy_list), "get_policy_list", strlen("get_policy_list")) == 0)) {
    char uri[PDP_MAX_STR_LEN] = {0};
    pap_action_list_t *action_elem = NULL;
    pip_attribute_object_t attribute;

    // Get action list
    pap_get_subjects_list_of_actions(user_str, strlen(user_str), &action->action_list);

    // Fill is payed value
    action_elem = action->action_list;
    while (action_elem) {
      if (memcmp(action_elem->is_available.cost, "0.0", strlen("0.0")) != 0) {
        memset(&attribute, 0, sizeof(pip_attribute_object_t));
        sprintf(uri, "iota:%s/request.isPayed.type?request.isPayed.value", action_elem->policy_id_str);
        pip_get_data(uri, &attribute);
        if (memcmp(attribute.value, "verified", strlen("verified")) == 0) {
          action_elem->is_available.is_payed = PAP_PAYED_VERIFIED;
        } else if (memcmp(attribute.value, "paid", strlen("paid")) == 0) {
          action_elem->is_available.is_payed = PAP_PAYED_PENDING;
        } else {
          action_elem->is_available.is_payed = PAP_NOT_PAYED;
        }

        memset(&attribute, 0, sizeof(pip_attribute_object_t));
        sprintf(uri, "iota:%s/request.walletAddress.type?request.walletAddress.value", action_elem->policy_id_str);
        pip_get_data(uri, &attribute);
        memcpy(action_elem->is_available.wallet_address, attribute.value, PDP_WALLET_ADDR_LEN);
      }
      else {
        action_elem->is_available.is_payed = PAP_PAYED_VERIFIED;
      }

      action_elem = action_elem->next;
    }

    return PDP_GRANT;
  }

  // Get policy ID from request
  request_policy_id = jsonhelper_get_value(request_norm, 0, "policy_id");
  size = jsonhelper_token_size(request_policy_id);

  memcpy(action->pol_id_str, request_norm + jsonhelper_get_token_start(request_policy_id), size * sizeof(char));

  // Get pol. object size and allocate policy object
  if (pap_get_policy_obj_len(action->pol_id_str, size, &pol_obj_len) == PAP_ERROR) {
    log_error(pdp_logger_id, "[%s:%d] Could not get the policy object length.\n", __func__, __LINE__);
    return PDP_ERROR;
  }

  // Check if any wallet action is requested
  request_balance = jsonhelper_get_value(request_norm, 0, "balance");
  if (request_balance != -1) {
    action->balance = strtoul(request_norm + jsonhelper_get_token_start(request_balance), NULL, PDP_STRTOUL_BASE);
  }

  request_wallet_addr = jsonhelper_get_value(request_norm, 0, "user_wallet");
  if (request_wallet_addr != -1 && action->wallet_address != NULL) {
    memcpy(action->wallet_address, request_norm + jsonhelper_get_token_start(request_wallet_addr),
           jsonhelper_token_size(request_wallet_addr));
  }

  // Check if transaction hash is given in request
  tr_hash = jsonhelper_get_value(request_norm, 0, "transaction_hash");
  if (tr_hash != -1) {
    memcpy(action->transaction_hash, request_norm + jsonhelper_get_token_start(tr_hash),
           jsonhelper_token_size(tr_hash));
    action->transaction_hash_len = jsonhelper_token_size(tr_hash);
  }

  policy_object = calloc(pol_obj_len * sizeof(char), 1);

  policy.policy_object.policy_object = policy_object;

  // Get policy from PAP
  if (pap_get_policy(action->pol_id_str, size, &policy) == PAP_ERROR) {
    log_error(pdp_logger_id, "[%s:%d] Could not get the policy.\n", __func__, __LINE__);
    free(policy_object);
    return PDP_ERROR;
  }

  // Get circuits
  jsonhelper_parser_init(policy.policy_object.policy_object);

  int policy_goc = jsonhelper_get_token_index(policy.policy_object.policy_object, "policy_goc");
  int policy_doc = jsonhelper_get_token_index(policy.policy_object.policy_object, "policy_doc");
  int policy_gobl = jsonhelper_get_token_index(policy.policy_object.policy_object, "obligation_grant");
  int policy_dobl = jsonhelper_get_token_index(policy.policy_object.policy_object, "obligation_deny");

  if (policy_goc == -1) {
    log_info(pdp_logger_id, "[%s:%d] POLICY policy_goc IS NULL.\n", __func__, __LINE__);
  }

  if (policy_doc == -1) {
    log_info(pdp_logger_id, "[%s:%d] POLICY policy_doc IS NULL.\n", __func__, __LINE__);
  }

  if (policy_gobl == -1) {
    log_info(pdp_logger_id, "[%s:%d] OBLIGATION obligation_grant IS NULL.\n", __func__, __LINE__);
  }

  if (policy_dobl == -1) {
    log_info(pdp_logger_id, "[%s:%d] OBLIGATION obligation_deny IS NULL.\n", __func__, __LINE__);
  }

  // Resolve attributes
  int pol_goc = resolve_attribute(policy.policy_object.policy_object, action->pol_id_str, policy_goc, user_str);
  int pol_doc = resolve_attribute(policy.policy_object.policy_object, action->pol_id_str, policy_doc, user_str);

  // Calculate decision
  ret = pol_goc + 2 * pol_doc;  // => (0, 1, 2, 3) <=> (gap, grant, deny, conflict)

  if (ret == PDP_GRANT) {
    // Get action
    // FIXME: Should action be taken for deny case also?
    int number_of_tokens = jsonhelper_get_token_num();
    jsonhelper_get_action(action->value, policy.policy_object.policy_object, number_of_tokens);

    action->start_time = 0;
    action->stop_time = 0;
    get_time_from_attr(policy.policy_object.policy_object, policy_goc, UNDEFINED, &(action->start_time),
                       &(action->stop_time));

    if (policy_gobl >= 0) {
      resolve_obligation(policy.policy_object.policy_object, action->pol_id_str, user_str, policy_gobl, obligation);
    }
  } else if (ret == PDP_DENY) {
    if (policy_dobl >= 0) {
      resolve_obligation(policy.policy_object.policy_object, action->pol_id_str, user_str, policy_dobl, obligation);
    }
  }

  log_info(pdp_logger_id, "[%s:%d] POLICY GoC RESOLVED: %d\n", __func__, __LINE__, pol_goc);
  log_info(pdp_logger_id, "[%s:%d] POLICY DoC RESOLVED: %d\n", __func__, __LINE__, pol_doc);
  log_info(pdp_logger_id, "[%s:%d] POLICY RESOLVED: %d\n", __func__, __LINE__, ret);

  free(policy_object);
  return ret;
}
