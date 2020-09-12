#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "cJSON.h"
#include "action.h"
#include "action_logger.h"

uint8_t action_add_attribute(action_t *action, char *attribute_id, void *attribute_value, attribute_enum attribute_type) {

  if (action == NULL || attribute_id == NULL || attribute_value == NULL) {
    log_error(action_logger_id, "[%s:%d] invalid input.\n", __func__, __LINE__);
    return ACTION_ERROR;
  }

  attribute_t *attribute = malloc(sizeof(attribute_t));
  attribute->attribute_id = attribute_id;
  attribute->attribute_value = attribute_value;
  // attribute->attribute_type = attribute_type;

  if (action->attributes == NULL) {
    action->attributes = (attribute_t*) malloc(sizeof(attribute_t));
    memcpy(action->attributes, attribute, sizeof(attribute_t));
  }else{

    attribute_t *current = action->attributes;

    while(current->next_attribute != NULL){
      current = current->next_attribute;
    }
    current->next_attribute = (attribute_t*) malloc(sizeof(attribute_t));
    memcpy(current->next_attribute, attribute, sizeof(attribute_t));
  }

  free(attribute);

  return ACTION_OK;
}

uint8_t action_add_obligation(action_t *action, char *obligation_id) {

  if (action == NULL || obligation_id == NULL) {
    log_error(action_logger_id, "[%s:%d] invalid input.\n", __func__, __LINE__);
    return ACTION_ERROR;
  }

  obligation_t *obligation = malloc(sizeof(obligation_t));
  obligation->obligation_id = obligation_id;

  if (action->obligations == NULL) {
    action->obligations = malloc(sizeof(obligation_t));
    memcpy(action->obligations, obligation, sizeof(obligation_t));
  } else {
    obligation_t *current = action->obligations;

    while (current->next_obligation != NULL) {
      current = current->next_obligation;
    }

    current->next_obligation = (obligation_t*) malloc(sizeof(obligation_t));
    memcpy(current->next_obligation, obligation, sizeof(obligation_t));
  }

  free(obligation);

  return ACTION_OK;
}

uint8_t action_encode_json(action_t *actions, char *action_json_output) {

  cJSON *json = cJSON_CreateObject();
  cJSON *actions_json = cJSON_CreateArray();
  if (actions_json == NULL || json == NULL) {
    log_error(action_logger_id, "[%s:%d] failed to create JSON array for actions.\n", __func__, __LINE__);
    return ACTION_ERROR;
  }

  cJSON_AddItemToObject(json, "actions", actions_json);

  action_t *current_action = actions;

  cJSON *current_action_json = NULL;
  cJSON *current_action_id_json = NULL;
  cJSON *current_action_attributes_json = NULL;

  bool loop = true;

  while(loop) {

    current_action_json = cJSON_CreateObject();
    if (current_action_json == NULL) {
      log_error(action_logger_id, "[%s:%d] failed to create JSON object for action.\n", __func__, __LINE__);
      return ACTION_ERROR;
    }

    cJSON_AddItemToArray(actions_json, current_action_json);

    current_action_id_json = cJSON_CreateString(current_action->action_id);
    if (current_action_id_json == NULL) {
      log_error(action_logger_id, "[%s:%d] failed to encode action_id to JSON.\n", __func__, __LINE__);
      return ACTION_ERROR;
    }

    cJSON_AddItemToObject(current_action_json, "action_id", current_action_id_json);

    if (current_action->attributes != NULL){

      cJSON *attributes_json = cJSON_CreateArray();
      if (attributes_json == NULL || json == NULL) {
        log_error(action_logger_id, "[%s:%d] failed to create JSON array for attributes.\n", __func__, __LINE__);
        return ACTION_ERROR;
      }

      cJSON_AddItemToObject(current_action_json, "attributes", attributes_json);

      attribute_t *current_attribute = current_action->attributes;

      cJSON *current_attribute_json = NULL;
      cJSON *current_attribute_id_json = NULL;
      cJSON *current_attribute_value = NULL;

      bool loop = true;

      while(loop) {

        current_attribute_json = cJSON_CreateObject();
        if (current_attribute_json == NULL) {
          log_error(action_logger_id, "[%s:%d] failed to create JSON object for attribute.\n", __func__, __LINE__);
          return ACTION_ERROR;
        }

        cJSON_AddItemToArray(attributes_json, current_attribute_json);

        current_attribute_id_json = cJSON_CreateString(current_attribute->attribute_id);
        if (current_attribute_id_json == NULL) {
          log_error(action_logger_id, "[%s:%d] failed to parse attribute_id to JSON.\n", __func__, __LINE__);
          return ACTION_ERROR;
        }

        cJSON_AddItemToObject(current_attribute_json, "attribute_id", current_attribute_id_json);

//        switch (current_attribute->attribute_type) {
//          case ATTR_NUMBER:
//            ; // empty statement, otherwise declaration below is illegal
//            uint32_t *attribute_value = (uint32_t) current_attribute->attribute_value;
//            current_attribute_value = cJSON_CreateNumber(*attribute_value);
//            break;
//          case ATTR_STRING:
//            current_attribute_value = cJSON_CreateString(current_attribute->attribute_value);
//            break;
//          case ATTR_BOOL:
//            current_attribute_value = cJSON_CreateString(current_attribute->attribute_value);
//            break;
//          default:
//            current_attribute_value = cJSON_CreateString("?");
//            break;
//        }

        if (current_attribute_id_json == NULL) {
          log_error(action_logger_id, "[%s:%d] failed to parse attribute_value to JSON.\n", __func__, __LINE__);
          return ACTION_ERROR;
        }

        cJSON_AddItemToObject(current_attribute_json, "attribute_value (raw)", current_attribute_value);

        current_attribute = current_attribute->next_attribute;
        loop = (current_attribute != NULL);
      }

      cJSON_AddItemToObject(current_action_json, "attributes", current_action_attributes_json);
    }

    current_action = current_action->next_action;
    loop = (current_action != NULL);
  }

  char *output = cJSON_Print(json);
  strcpy(action_json_output, output);

  cJSON_Delete(json);

  return ACTION_OK;

}