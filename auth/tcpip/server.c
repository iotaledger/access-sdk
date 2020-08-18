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

#include <sys/socket.h>

#include "server.h"

uint8_t tcpip_bind(uint8_t sockfd, const struct sockaddr_in *saddr, socklen_t saddrlen){
  if (bind(sockfd, saddr, saddrlen) < 0) {
    log_error(tcpip_logger_id, "[%s:%d] bind socket failed.\n", __func__, __LINE__);
    return TCPIP_ERROR;
  }

  log_info(tcpip_logger_id, "[%s:%d] bind socket successful.\n", __func__, __LINE__);

  return TCPIP_OK;
}

uint8_t tcpip_listen(uint8_t sockfd, uint8_t backlog){
  if (listen(sockfd, backlog) != 0){
    log_error(tcpip_logger_id, "[%s:%d] listen socket failed.\n", __func__, __LINE__);
    return TCPIP_ERROR;
  }

  log_info(tcpip_logger_id, "[%s:%d] listening to socket...\n", __func__, __LINE__);

  return TCPIP_OK;
}

uint8_t tcpip_accept(uint8_t sockfd, struct sockaddr_in *cliaddr, socklen_t *restrict address_len){
  uint8_t ret = accept(sockfd, cliaddr, sizeof(cliaddr));

  if (ret < 0){
    return TCPIP_ERROR;
  }

  log_info(tcpip_logger_id, "[%s:%d] accepted client: %s\n", __func__, __LINE__, inet_ntoa(cliaddr->sin_addr));

  return ret;
}