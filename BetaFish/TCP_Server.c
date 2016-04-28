#include "TCP_Server.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void closeServer()
{
    close(newsockfd);
    close(sockfd);
}


void tcp_init() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi("15006");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
             error("ERROR on accept");
}

int tcp_read(uint8_t *buf, int len) {
    bzero(buf, len);
    n = read(newsockfd, buf, len);
    if (n < 0)
        error("ERROR reading from socket");
    return n;
}

int tcp_write(uint8_t *buf, int len) {
   // bzero(buf, 9840);
    n = write(newsockfd, buf, len);
    if (n < 0)
        error("ERROR reading from socket");
    return n;
}
