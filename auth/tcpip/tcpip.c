#include "tcpip.h"


#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>>
#include <unistd.h>

ssize_t tcpip_read_socket(int *sockfd, void *data, unsigned short len) {
  return read(*sockfd, data, len);
}

ssize_t tcpip_write_socket(int *sockfd, void *data, unsigned short len) {
  return write(*sockfd, data, len);
}

int tcpip_connect(int sockfd, char *servip, int port) {

  struct sockaddr_in serv_addr;
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, servip, &serv_addr.sin_addr) <= 0) {
    printf("inet_pton error\n");
    return TCPIP_ERROR;
  }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("connection error\n");
    return TCPIP_ERROR;
  }

  return TCPIP_OK;
}