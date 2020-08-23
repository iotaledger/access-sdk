#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>

#include "assert.h"

#include "sodium.h"

#include "auth.h"
#include "auth_logger.h"
#include "auth_server_test.h"

#include "tcpip.h"

#define BUFSIZE 128

int *auth_client_test(char *hostname, uint16_t portno) {

  ///////////////////////////////////////////////////////////////
  // tcpip
  auth_ctx_t client;
  int sockfd = tcpip_socket();
  int n;
  struct sockaddr_in serveraddr;
  struct hostent *server;
  char buf[BUFSIZE];

  /* socket: create the socket */
  sockfd = tcpip_socket(AF_INET, SOCK_STREAM, 0);
  assert(sockfd >= 0);

  /* gethostbyname: get the server's hostname */
  server = gethostbyname(hostname);
  if (server == NULL) {
    log_error(auth_logger_id, "[%s:%d] no such host as %s.\n", __func__, __LINE__, hostname);
    exit(0);
  }

  /* build the server's Internet address */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
        (char *)&serveraddr.sin_addr.s_addr, server->h_length);
  serveraddr.sin_port = htons(portno);

  /* connect: create a connection with the server */
  assert(connect(sockfd, &serveraddr, sizeof(serveraddr)) >= 0);

  log_info(auth_logger_id, "[%s:%d] connected.\n", __func__, __LINE__);

  ////////////////////////////////////////////////////////////////////
  // auth

  assert(auth_init_client(&client, &sockfd) == AUTH_OK);

  uint8_t seed[crypto_sign_SEEDBYTES];
  bzero(seed, crypto_sign_SEEDBYTES);

  for (int i = 0; i < crypto_sign_SEEDBYTES; i++) { seed[i] = rand(); }

  uint8_t ed25519_sk[crypto_sign_SECRETKEYBYTES];
  uint8_t ed25519_pk[crypto_sign_PUBLICKEYBYTES];

  crypto_sign_seed_keypair(ed25519_pk, ed25519_sk, seed);

  char shexbuf[2*crypto_box_SECRETKEYBYTES+1];
  char phexbuf[2*crypto_box_PUBLICKEYBYTES+1];

  bzero(shexbuf,2*crypto_box_SECRETKEYBYTES+1);
  bzero(phexbuf,2*crypto_box_PUBLICKEYBYTES+1);

  hex_encode(ed25519_sk, crypto_box_SECRETKEYBYTES, shexbuf, 2*crypto_box_SECRETKEYBYTES+1);
  log_info(auth_logger_id, "[%s:%d] ed25519_sk: %s\n", __func__, __LINE__, shexbuf);

  hex_encode(ed25519_pk, crypto_box_PUBLICKEYBYTES, phexbuf, 2*crypto_box_PUBLICKEYBYTES+1);
  log_info(auth_logger_id, "[%s:%d] ed25519_pk: %s\n", __func__, __LINE__, phexbuf);

  auth_authenticate(&client, ed25519_sk);
  ///////////////////////////////////////////////////////////////////////
  // tcpip send
  /* set message on buffer */
  bzero(buf, BUFSIZE);
  strncpy(buf, "msg\0", strlen("msg\0"));

  /* write message to server */
  assert(tcpip_write(sockfd, buf, strlen(buf)) >= 0);

  /* read the server's reply */
  bzero(buf, BUFSIZE);
  assert(tcpip_read(sockfd, buf, BUFSIZE) > 0);
  log_info(auth_logger_id, "[%s:%d] echo from server: %s\n", __func__, __LINE__, buf);

  close(sockfd);
  return 0;

}
