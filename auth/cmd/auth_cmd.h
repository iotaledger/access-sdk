#include "policy.h"

#ifndef AUTH_CMD_H
#define AUTH_CMD_H

typedef struct {
  char *policy_id;
  action_t action;
} auth_cmd_t;

#endif //AUTH_CMD_H
