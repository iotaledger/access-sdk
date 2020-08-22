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
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "sodium.h"
#include "hex.h"
#include "auth.h"
#include "auth_helper.h"

int main(int argc, char **argv) {

  int sockfd;
  auth_ctx_t session;
  unsigned char *buf;
  unsigned short length;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("\n Error : Could not create socket \n");
    return 1;
  }

  uint8_t seed[crypto_sign_SEEDBYTES];
  bzero(seed, crypto_sign_SEEDBYTES);

  uint8_t ed25519_sk[crypto_sign_SECRETKEYBYTES];
  uint8_t ed25519_pk[crypto_sign_PUBLICKEYBYTES];

  seed[0] = 31;

  crypto_sign_seed_keypair(ed25519_pk, ed25519_sk, seed);

  char shexbuf[2*crypto_box_SECRETKEYBYTES+1];
  char phexbuf[2*crypto_box_PUBLICKEYBYTES+1];

  bzero(shexbuf,2*crypto_box_SECRETKEYBYTES+1);
  bzero(phexbuf,2*crypto_box_PUBLICKEYBYTES+1);

  hex_encode(ed25519_sk, crypto_box_SECRETKEYBYTES, shexbuf, 2*crypto_box_SECRETKEYBYTES+1);
  printf("sk: %s\n", shexbuf);

  hex_encode(ed25519_pk, crypto_box_PUBLICKEYBYTES, phexbuf, 2*crypto_box_PUBLICKEYBYTES+1);
  printf("pk: %s\n", phexbuf);

  assert(auth_init_client(&session, &sockfd, ed25519_sk) == AUTH_OK);

  // assuming there's an open socket on server side
  assert(auth_connect_client(sockfd, "127.0.0.1", 9998) == AUTH_OK);
  assert(auth_authenticate(&session, ed25519_sk) == AUTH_OK);
  assert(auth_helper_send_decision(1, &session, "test", strlen("test")) == AUTH_OK);
  assert(auth_receive(&session, (unsigned char **)&buf, &length) == AUTH_OK);
  assert(strcmp(buf, "{\"response\":\"access denied \"}") == 0);

  // ToDo: test server

  return 0;
}
