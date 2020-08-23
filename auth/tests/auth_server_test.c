#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>

#include "assert.h"

#include "tcpip/tcpip.h"
#include "tcpip/server.h"
#include "auth.h"
#include "auth_logger.h"
#include "auth_server_test.h"

#define BUFSIZE 128

int *auth_server_test(bool *serve) {

  auth_ctx_t *session = calloc(1, sizeof(auth_ctx_t));

  int listen_sockfd = tcpip_socket(AF_INET, SOCK_STREAM, 0);
  assert(listen_sockfd > 0);

  assert(auth_init_server(session, listen_sockfd) == AUTH_OK);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(9998);

  int retstat = bind(listen_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (retstat != 0) {
    log_error(auth_logger_id, "[%s:%d] bind failed.\n", __func__, __LINE__);
    free(session);
    return 1;
  }

  retstat = listen(listen_sockfd, 10);
  if (retstat != 0) {
    log_error(auth_logger_id, "[%s:%d] listen failed.\n", __func__, __LINE__);
    free(session);
    return 1;
  }

  while (serve) {
    /*
    * accept: wait for a connection request
    */
    struct sockaddr_in clientaddr; /* client addr */
    int clientlen = sizeof(clientaddr);
    int accept_sockfd = tcpip_accept(listen_sockfd, (struct sockaddr *) &clientaddr, &clientlen);
    assert(accept_sockfd >= 0);

    /*
     * gethostbyaddr: determine who sent the message
     */
    struct hostent *hostp; /* client host info */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                          sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    assert(hostp != NULL);

    char *hostaddrp; /* dotted decimal host addr string */
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    assert(hostaddrp != NULL);

    log_info(auth_logger_id, "[%s:%d] established connection with %s (%s).\n", __func__, __LINE__, hostp->h_name, hostaddrp);

    /*
     * read: read input string from the client
     */
    char buf[BUFSIZE]; /* message buffer */
    bzero(buf, BUFSIZE);
    int n = read(accept_sockfd, buf, BUFSIZE);
    assert(n >= 0);

    log_info(auth_logger_id, "[%s:%d] received %d bytes: %s\n", __func__, __LINE__, n, buf);

    /*
     * write: echo the input string back to the client
     */
    assert(write(accept_sockfd, buf, strlen(buf)) >= 0);

    close(accept_sockfd);
    log_info(auth_logger_id, "[%s:%d] released accept_sockfd.\n", __func__, __LINE__);

    // test: release while
    serve = false;
  }

  close(listen_sockfd);
  log_info(auth_logger_id, "[%s:%d] released listen_sockfd.\n", __func__, __LINE__);

}
