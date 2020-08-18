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

#include "client.h"

uint8_t tcpip_connect(uint8_t sockfd, struct sockaddr_in *peer_ip, uint8_t port) {
  if (connect(sockfd, peer_ip, sizeof(peer_ip)) != 0) {
    log_error(tcpip_logger_id, "[%s:%d] connect socket failed.\n", __func__, __LINE__);
    return TCPIP_ERROR;
  }

  return TCPIP_OK;
};
