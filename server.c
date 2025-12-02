#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

const int MAX_CLIENTS = 4;
FILE *CLIENTS[MAX_CLIENTS] = {0};

int main(int argc, char *argv[]) {
    struct sockaddr_in address;
    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, which is the localhost
    address.sin_port = htons(7878);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    if (fcntl(server_fd, F_SETFLG, O_NONBLOCKING) == -1) {
        perror("fcntl server_fd");
        close(server_fd);
        return 1;
    }

    fcntl(stdin, F_SETFLG, O_NONBLOCKING);
    int status = main_loop(server_fd);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (NULL != CLIENTS[i]) {
            fclose(CLIENTS[i]);
        }
    }
        
    close(server_fd);
    return status;
}