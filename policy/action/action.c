#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "cJSON.h"
#include "action.h"
#include "action_logger.h"

uint8_t action_add_attribute(action_t *action, char *attribute_id, void *attribute_value) {

  if (action == NULL || attribute_id == NULL || attribute_value == NULL) {
    log_error(action_logger_id, "[%s:%d] invalid input.\n", __func__, __LINE__);
    return ACTION_ERROR;
  }

  attribute_t *attribute = malloc(sizeof(attribute_t));
  attribute->attribute_id = attribute_id;
  attribute->attribute_value = attribute_value;

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