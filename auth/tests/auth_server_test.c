#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "assert.h"

#include "auth.h"
#include "auth_server_test.h"

int *auth_server_test(bool *serve) {

  auth_ctx_t *session = calloc(1, sizeof(auth_ctx_t));;

  int port = 9999;
  auth_init_server(session, port);
  assert(session->side == AUTH_SERVER);
  assert(session->sockfd > 0);
  assert(session->port == port);

  auth_authenticate(session);

}