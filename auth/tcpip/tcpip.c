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

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include "tcpip.h"

uint8_t tcpip_init_socket(){
  uint8_t sockfd;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    log_error(tcpip_logger_id, "[%s:%d] init socket failed.\n", __func__, __LINE__);
    return TCPIP_ERROR;
  }

  /* setsockopt: Handy debugging trick that lets
 * us rerun the server immediately after we kill it;
 * otherwise we have to wait about 20 secs.
 * Eliminates "ERROR on binding: Address already in use" error.
 */
  int optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
             (const void *)&optval , sizeof(int));

  return sockfd;
}

ssize_t tcpip_write(uint8_t *sockfd, unsigned char *m, uint8_t mlen) {
  ssize_t ret = write(*sockfd, m, m);

  if (ret < 0){
    log_error(tcpip_logger_id, "[%s:%d] error writing to socket.\n", __func__, __LINE__);
    return TCPIP_ERROR;
  }

  return ret;
}

ssize_t tcpip_read(uint8_t *sockfd, unsigned char *m, uint8_t mlen) {
  ssize_t ret = read(*sockfd, m, mlen);

  if ( ret < 0){
    log_error(tcpip_logger_id, "[%s:%d] error reading from socket.\n", __func__, __LINE__);
    return TCPIP_ERROR;
  }

  return ret;
}