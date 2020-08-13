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
#include <sys/socket.h>
#include <arpa/inet.h>

#include "auth.h"
#include "auth_helper.h"

int auth_connect_client(int sockfd, char *servip, int port) {

  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, servip, &serv_addr.sin_addr) <= 0) {
    printf("inet_pton error\n");
    return AUTH_ERROR;
  }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("connection error\n");
    return AUTH_ERROR;
  }

  return AUTH_OK;
}

int main(int argc, char **argv) {

  int sockfd;
  auth_ctx_t session;
  unsigned char *buf;
  unsigned short length;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  assert(auth_init_client(&session, &sockfd) == AUTH_OK);
  assert(auth_connect_client(sockfd, "127.0.0.1", 9998) == AUTH_OK);
  assert(auth_authenticate(&session) == AUTH_OK);
  assert(auth_helper_send_decision(1, &session, "test", strlen("test")) == AUTH_OK);
  assert(auth_receive(&session, (unsigned char **)&buf, &length) == AUTH_OK);
  assert(strcmp(buf, "{\"response\":\"access denied \"}") == 0);

  return 0;
}
