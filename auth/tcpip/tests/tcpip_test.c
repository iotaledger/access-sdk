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
#include <unistd.h>
#include <pthread.h>

#include "tcpip.h"
#include "server.h"
#include "client.h"

#define BUFSIZE 1024

static bool serve = 1;
static int portno = 9999;
static struct sockaddr_in serveraddr; /* server's addr */
static struct sockaddr_in clientaddr; /* client addr */

static pthread_t server_thread;

void init_client(){
  int sockfd = tcpip_init_socket();

  bzero((char *) &clientaddr, sizeof(clientaddr));
  clientaddr.sin_family = AF_INET;
  clientaddr.sin_addr.s_addr = htonl("127.0.0.1");
  clientaddr.sin_port = htons((unsigned short)portno);

  assert(tcpip_connect(sockfd, &serveraddr, portno) == TCPIP_OK);
}

void *init_server(){
  int listen_fd; /* listen file descriptor */
  struct sockaddr_in internal_client;
  int internal_client_len = sizeof(internal_client); /* byte size of client's address */

  bzero((char *) &clientaddr, sizeof(clientaddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  listen_fd = tcpip_init_socket();

  assert(tcpip_bind(listen_fd, &serveraddr, sizeof(serveraddr)) == TCPIP_OK);
  assert(tcpip_listen(listen_fd, 1) == TCPIP_OK);
  printf("listening...\n");

  while (serve){
    printf("serving...\n");

    int accept_fd = tcpip_accept(listen_fd, &internal_client, &internal_client_len);
    assert(accept_fd != TCPIP_ERROR);

  }

}

int main(int argc, char **argv) {

  init_client();

  printf("Before Thread\n");
  pthread_create(&server_thread, NULL, init_server, NULL);
  pthread_detach(server_thread);
  pthread_join(server_thread, NULL);
  printf("After Thread\n");

  for (int i = 0; i < 1000; i++) printf("%i\n", i);

  // kill the server
  serve = 0;

}
