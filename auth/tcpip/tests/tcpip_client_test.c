/*
 * adapted from: https://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "tcpip.h"
#include "client.h"

#include "tcpip_test.h"

#define BUFSIZE 1024

int tcpip_client_test() {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

    hostname = "0.0.0.0";
    portno = 9999;

    /* socket: create the socket */
    sockfd = tcpip_socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    /* gethostbyname: get the server's hostname */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"client error:  no such host as %s.\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    assert(tcpip_connect(sockfd, &serveraddr, sizeof(serveraddr)) >= 0);

    printf("client: connected with\n");

    /* set message on buffer */
    bzero(buf, BUFSIZE);
    strncpy(buf, "msg\0", strlen("msg\0"));

    /* write message to server */
    assert(tcpip_write(sockfd, buf, strlen(buf)) >= 0);

    /* read the server's reply */
    bzero(buf, BUFSIZE);
    assert(tcpip_read(sockfd, buf, BUFSIZE) > 0);
    printf("client: echo from server: %s\n", buf);
    close(sockfd);
    return 0;
}
