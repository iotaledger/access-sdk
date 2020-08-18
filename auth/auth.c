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
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "auth.h"
#include "tcpip.h"
#include "client.h"
#include "server.h"

uint8_t auth_init_client(auth_ctx_t *session, char *serv_ip, uint16_t port) {
  session = calloc(1, sizeof(auth_ctx_t));

  session->side = AUTH_CLIENT;
  session->sockfd = tcpip_socket();
  session->port = port;

  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  session->peer_ip = &servaddr;

  if (tcpip_connect(session->sockfd, session->peer_ip, session->port) != TCPIP_OK) {
    log_error(auth_logger_id, "[%s:%d] failed to connect to server.\n", __func__, __LINE__);
    return AUTH_ERROR;
  }

  return AUTH_OK;
}

uint8_t auth_init_server(auth_ctx_t *session, uint16_t port) {

  session->side = AUTH_SERVER;
  session->sockfd = tcpip_socket();
  session->port = port;

  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  session->peer_ip = &servaddr;

  log_info(auth_logger_id, "[%s:%d] initialized auth server.\n", __func__, __LINE__);

  return AUTH_OK;
}

uint8_t auth_authenticate(auth_ctx_t *session){

  uint8_t *nonce;
  uint8_t *peer_DH_pk;
  uint8_t *peer_sign_pk;

  // process data

  session->nonce = nonce;
  session->peer_DH_pk = peer_DH_pk;
  session->peer_sign_pk = peer_sign_pk;

  // ToDo: log peer_sign_pk as peer_id
  log_info(auth_logger_id, "[%s:%d] authenticated with peer xxx.\n", __func__, __LINE__);

}
//
// uint8_t auth_send(auth_ctx_t *session, const unsigned char *m, unsigned short mlen);
//
// uint8_t auth_receive(auth_ctx_t *session, unsigned char *m, unsigned short mlen);
//
// uint8_t auth_release(auth_ctx_t *session);