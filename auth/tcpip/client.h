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

#include <stdint.h>

#include "tcpip.h"

#ifndef TCPIP_CLIENT_H
#define TCPIP_CLIENT_H

/**
 * @brief connects to server via TCP/IP
 *
 * @param sockfd socket file descriptor
 * @param peer_ip peer IP address
 * @param port server port
 *
 * @return TCPIP_OK or TCPIP_ERROR
 */

uint8_t tcpip_connect(uint8_t sockfd, struct sockaddr_in *peer_ip, uint8_t port);

#endif  // TCPIP_CLIENT_H
