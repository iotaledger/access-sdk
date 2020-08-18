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

#ifndef AUTH_TCPIP_H
#define AUTH_TCPIP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcpip_logger.h"

#define TCPIP_OK 0
#define TCPIP_ERROR -1

/**
 * @brief init tcpip socket
 * @return socket file descriptor
 */
uint8_t tcpip_init_socket();

/**
 * @brief write message to socket
 *
 * @param sockfd pointer to socket file descriptor
 * @param m message
 * @param mlen length of message
 *
 * @return On success, the number of bytes written is returned (zero indicates nothing was written). On error, -1 is returned, and errno is set appropriately.
 */
ssize_t tcpip_write(uint8_t *sockfd, unsigned char *m, uint8_t mlen);

/**
 * @brief write message to socket
 *
 * @param sockfd pointer to socket file descriptor
 * @param m message
 * @param mlen length of message
 *
 * @return On success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number. On error, -1 is returned, and errno is set appropriately. In this case, it is left unspecified whether the file position (if any) changes.
 */
ssize_t tcpip_read(uint8_t *sockfd, unsigned char *m, uint8_t mlen);

#endif  // AUTH_TCPIP_H