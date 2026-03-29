#define _GNU_SOURCE
#include "parser.h"
#include <err.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50

int serverfd;

void signal_sigint(int sig) {
    printf("Очистка всего\n");
    close(serverfd);
    exit(0);
}

int init_server() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        err(EXIT_FAILURE, "socket");

    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) ==
        -1)
        err(EXIT_FAILURE, "setsockopt");

    struct sockaddr_in my_addr = {0};
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8080);
    my_addr.sin_addr = (struct in_addr){0};

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        err(EXIT_FAILURE, "bind");

    if (listen(sockfd, LISTEN_BACKLOG) == -1)
        err(EXIT_FAILURE, "listen");

    return sockfd;
}

int main() {
    signal(SIGINT, signal_sigint);
    serverfd = init_server();

    // wait client
    int connfd;
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);

    while (true) {
        if ((connfd = accept(serverfd, (struct sockaddr *)&client_addr,
                             &client_addr_len)) == -1)
            err(EXIT_FAILURE, "accept");

        char buf[4096] = {0};
        recv(connfd, buf, sizeof(buf), 0);

        printf("Request:\n");
        http_headers *headers = parse_request(buf);

        char *template = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "\r\n"
                         "<h1>C SIMPLE HTTP SERVER!</h1>"
                         "<p>%s</p>"
                         "<p>%s</p>"
                         "<p>%s</p>";

        char response[4096];
        sprintf(response, template, headers->url, headers->version,
                headers->method);

        send(connfd, response, strlen(response), 0);

        printf("Отправлен ответ клиенту\n");

        free_headers(headers);
        close(connfd);
    }

    close(serverfd);
    return 0;
}
