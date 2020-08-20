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

/****************************************************************************
 * \project IOTA Access
 * \file auth.h
 * \brief
 *    Authentication API submodule TCP/IP
 *
 * @Author Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 18.08.2020. Initial Version
 ****************************************************************************/

#ifndef AUTH_TCPIP_H
#define AUTH_TCPIP_H

#include <sys/types.h>

/**
 * @brief no TCP/IP error
 */
#define TCPIP_OK 0

/**
 * @brief TCP/IP error
 */
#define TCPIP_ERROR 1

/**
 * @brief write to socket
 *
 * @param sockfd pointer to socker file descriptor
 * @param buffer with data to be written
 * @param len length of data
 *
 * @return On success, the number of bytes written is returned (zero indicates nothing was written). On error, -1 is returned, and errno is set appropriately.
 */
ssize_t tcpip_write_socket(int *sockfd, void *data, unsigned short len);

/**
 * @brief read from socket
 *
 * @param sockfd pointer to socker file descriptor
 * @param buffer with for data to be read
 * @param len length of data
 *
 * @return On success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number. On error, -1 is returned, and errno is set appropriately. In this case, it is left unspecified whether the file position (if any) changes.
 */
ssize_t tcpip_read_socket(int *sockfd, void *data, unsigned short len);


/**
 * @brief connects to server via TCP/IP
 *
 * @param sockfd socket file descriptor
 * @param servip pointer to string with server IP
 * @param port server port
 * @return TCPIP_OK or TCPIP_ERROR
 */

int tcpip_connect(int sockfd, char *servip, int port);

int tcpip_serve(int );

#endif  // AUTH_TCPIP_H
