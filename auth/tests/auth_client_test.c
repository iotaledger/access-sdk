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

  struct sockaddr_in sa;
  int res;
  int socket_fd;

  socket_fd = tcpip_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket_fd == -1) {
    perror("cannot create socket");
    exit(EXIT_FAILURE);
  }

  memset(&sa, 0, sizeof sa);

  sa.sin_family = AF_INET;
  sa.sin_port = htons(portno);
  res = inet_pton(AF_INET, hostname, &sa.sin_addr);

  if (tcpip_connect(socket_fd, (struct sockaddr *) &sa, sizeof sa) == -1) {
    perror("connect failed");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  // auth
  uint8_t ed_seed[crypto_sign_SEEDBYTES];
  bzero(ed_seed, crypto_sign_SEEDBYTES);

  auth_ctx_t *client = calloc(1, sizeof(auth_ctx_t));
  auth_init_client(client, socket_fd);

  for (int i = 0; i < crypto_sign_SEEDBYTES; i++) { ed_seed[i] = rand(); }

  uint8_t ed25519_sk[crypto_sign_SECRETKEYBYTES];
  uint8_t ed25519_pk[crypto_sign_PUBLICKEYBYTES];

  crypto_sign_seed_keypair(ed25519_pk, ed25519_sk, ed_seed);

  auth_authenticate(client, ed25519_sk); // erases ed25519_sk

  ////////////////////////////////////////////////////////////

  sleep(1);

  // gen ed25519_sk again
  crypto_sign_seed_keypair(ed25519_pk, ed25519_sk, ed_seed);

  char *msg =  "test";

  /* send authenticated message to server */
  log_info(auth_logger_id, "[%s:%d] sending authenticated message: %s\n", __func__, __LINE__, msg);
  auth_send(client, ed25519_sk, msg, strlen(msg));

  ///////////////////////////////////////////////////////////
  // cleanup

  shutdown(socket_fd, SHUT_RDWR);
  close(socket_fd);

  auth_release(client);

  return 0;

}
