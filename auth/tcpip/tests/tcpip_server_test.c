/*
 * adapted from: https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcpip.h"
#include "server.h"
#include "tcpip_test.h"

#define BUFSIZE 1024

int *tcpip_server_test(bool *serve) {
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  portno = 9999;

  /*
   * socket: create the parent socket
   */
  parentfd = tcpip_socket(AF_INET, SOCK_STREAM, 0);
  assert(parentfd >= 0);

  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /*
   * bind: associate the parent socket with a port
   */
  assert(tcpip_bind(parentfd, (struct sockaddr *) &serveraddr,
	   sizeof(serveraddr)) >= 0);

  /*
   * listen: make this socket ready to accept connection requests
   */
  assert(tcpip_listen(parentfd, 5) >= 0); /* allow 5 requests to queue up */

  /*
   * main loop: wait for a connection request, echo input line,
   * then close connection.
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * accept: wait for a connection request
     */
    childfd = tcpip_accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    assert(childfd >= 0);

    /*
     * gethostbyaddr: determine who sent the message
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    assert(hostp != NULL);
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    assert(hostaddrp != NULL);

    printf("server: established connection with %s (%s)\n",
	   hostp->h_name, hostaddrp);

    /*
     * read: read input string from the client
     */
    bzero(buf, BUFSIZE);
    n = tcpip_read(childfd, buf, BUFSIZE);
    assert(n >= 0);

    printf("server: received %d bytes: %s\n", n, buf);

    /*
     * write: echo the input string back to the client
     */
    assert(tcpip_write(childfd, buf, strlen(buf)) >= 0);

    close(childfd);
  }
}
