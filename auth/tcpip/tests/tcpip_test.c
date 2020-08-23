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
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>

#include "tcpip.h"
#include "server.h"
#include "client.h"

#include "tcpip_client_test.h"
#include "tcpip_server_test.h"

/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char **argv) {

  // test server
  static bool serve = true;
  pthread_t server;
  pthread_create(&server, NULL, &tcpip_server_test, &serve);
  pthread_detach(server);

  // test client
  tcpip_client_test();

  // kill server
  serve = false;

  return( 0 );
}
