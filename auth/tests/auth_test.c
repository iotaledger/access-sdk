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
#include <stdio.h>
#include <string.h>

#include "auth.h"
#include "auth_client_test.h"
#include "auth_server_test.h"

#include "tcpip.h"
#include "pthread.h"
#include "auth_server_test.h"
#include "auth_client_test.h"

int main(int argc, char **argv) {

  logger_helper_init(LOGGER_INFO);
  logger_init_auth(LOGGER_INFO);

  // test server
  static bool serve = true;
  pthread_t server;
  int ret = pthread_create(&server, NULL, &auth_server_test, &serve);
  ret = pthread_detach(server);

  sleep(1);
  // kill server
  serve = false;

  // assert(auth_init_client(&session, &sockfd) == AUTH_OK);

//  // assuming there's an open socket on server side
//  assert(tcpip_connect(sockfd, "127.0.0.1", 9998) == AUTH_OK);
//  assert(auth_authenticate(&session) == AUTH_OK);
//  assert(auth_helper_send_decision(1, &session, "test", strlen("test")) == AUTH_OK);
//  assert(auth_receive(&session, (unsigned char **)&buf, &length) == AUTH_OK);
//  assert(strcmp(buf, "{\"response\":\"access denied \"}") == 0);

  // ToDo: test server

  return 0;
}
