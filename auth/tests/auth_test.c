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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "sodium.h"
#include "hex.h"

#include "auth.h"
#include "auth_logger.h"
#include "tcpip.h"
#include "auth_client_test.h"
#include "auth_server_test.h"




int main(int argc, char **argv) {

  logger_helper_init(LOGGER_INFO);
  logger_init_auth(LOGGER_INFO);

  // test server
  static bool serve = true;
  pthread_t server;
  int ret = pthread_create(&server, NULL, &auth_server_test, &serve);
  ret = pthread_detach(server);

  // wait for thread bootstrap
  sleep(10000);

  auth_client_test("0.0.0.0", 9998);

  // kill server
  serve = false;

  // wait for socket release
  sleep(1);

  return 0;
}
