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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "auth.h"
#include "auth_debug.h"
#include "auth_internal.h"

ssize_t read_socket(int *sockfd, void *data, unsigned short len) {
  return read(*sockfd, data, len);
}

ssize_t write_socket(int *sockfd, void *data, unsigned short len) {
  return write(*sockfd, data, len);
}

// ToDo: implement proper verification.
int verify(unsigned char *key, int len) { return 0; }

typedef struct auth_struct auth_struct_t;
static auth_struct_t internal;

static int auth_init(auth_ctx_t *session, int *sockfd, int type) {
  int ret = AUTH_ERROR;

  if (NULL != session) {
    memset((void *)session, 0, sizeof(auth_ctx_t));

    session->internal = &internal;

    if (NULL != session->internal) {
      memset((void *)session->internal, 0, sizeof(auth_struct_t));

      session->type = type;
      session->sockfd = sockfd;

      ret = AUTH_OK;
    }
  }

  return ret;
}

int auth_init_client(auth_ctx_t *session, int *sockfd) { return auth_init(session, sockfd, AUTH_TYPE_CLIENT); }

int auth_init_server(auth_ctx_t *session, int *sockfd) { return auth_init(session, sockfd, AUTH_TYPE_SERVER); }

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

int auth_authenticate(auth_ctx_t *session, uint8_t sk[]) {
  int ret = AUTH_ERROR;

  if (NULL != session) {
    if (AUTH_TYPE_SERVER == session->type) {
      ret = auth_internal_server_authenticate(session, sk);
    } else if (AUTH_TYPE_CLIENT == session->type) {
      ret = auth_internal_client_authenticate(session, sk);
    }
  }

  return ret;
}

uint8_t auth_encrypt(auth_ctx_t *session, uint8_t ed25516_sk[], uint8_t *ciphertext, const uint8_t *data){
  return auth_internal_encrypt(session, ed25516_sk, ciphertext, data);
}

uint8_t auth_decrypt(auth_ctx_t*session, uint8_t ed25519_sk[], uint8_t *data, const uint8_t *ciphertext){
  return auth_internal_decrypt(session, ed25519_sk, data, ciphertext);
}


int auth_release(auth_ctx_t *session) {
  int ret = AUTH_ERROR;

//  if (NULL != session->internal->type) {
//    if (AUTH_TYPE_SERVER == session->internal->type) {
//      auth_internal_release_server(session);
//    } else if (AUTH_TYPE_CLIENT == session->internal->type) {
//      auth_internal_release_client(session);
//    }
//
//    session->internal = NULL;
//
//    ret = AUTH_OK;
//  }

  if (NULL != session) {
    session = NULL;

    ret = AUTH_OK;
  }

  return ret;
}
