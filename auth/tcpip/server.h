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

#ifndef AUTH_SERVER_H
#define AUTH_SERVER_H

#include "tcpip.h"

/**
 * @brief bind tcpip socket
 *
 * @param sockfd socket file descriptor
 * @param saddr server ip address
 * @param saddrlen length of servaddr
 * @return TCPIP_OK or TCPIP_ERROR
 */
int tcpip_bind(uint8_t sockfd, const struct sockaddr_in *saddr, socklen_t saddrlen);

/**
 * @brief listen on tcpip socket
 *
 * @param sockfd socket file descriptor
 * @param backlog max len of queue for pending connections
 * @return TCPIP_OK or TCPIP_ERROR
 */
int tcpip_listen(uint8_t sockfd, uint8_t backlog);

/**
 * @brief accept on tcpip socket
 * @param sockfd
 * @param cliaddr
 * @return file descriptor
 */
int tcpip_accept(uint8_t sockfd, struct sockaddr_in *cliaddr, socklen_t *restrict address_len);

#endif  // AUTH_SERVER_H