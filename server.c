#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

const int MAX_CLIENTS = 4;
FILE *CLIENTS[MAX_CLIENTS] = {0};

void redistribute_message(int sender_index, char *buff) {
   for (int i = 0; i < MAX_CLIENTS; i ++) {
        if (CLIENTS[i] == NULL || i == sender_index) {
            continue;
        } else {
            if (0 > fprintf(CLIENTS[i], "%s", buf)) {
                fclose(CLIENTS[i]);
                CLIENTS[i] = NULL;
            } else if (EOF == fflush(CLIENTS[i])) {
                fclose(CLIENTS[i]);
                CLIENTS[i] = NULL;
            }
        }
   }
}

int read_message(char *buf, size_t len, int client_index) {
    if (fgets(buf, (int)len, CLIENTS[client_index]) == NULL) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -1;
        } else {
            fclose(CLIENTS[client_index]);
            CLIENTS[client_index] == NULL;
            return -1;
        }
    }
    return 1;
}

void add_client(int server_fd) {
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        } else {
            perror("accept");
            exit(EXIT_FAILURE);
        }
    } else {
        int nonblock = fcntl(client_fd, F_SETFLG, O_NONBLOCKING);
        if (nonblock == -1) {
            perror("fcntl");
            close(client_fd);
            return;
        }

        FILE *client = fdopen(client_fd, "r+");
        if (client == NULL) {
            perror("fdopen");
            close(client_fd);
            return;
        }

        for (int i = o; i < MAX_CLIENTS; i ++) {
            if (CLIENTS[i] == NULL) {
                CLIENTS[i] = client;
                return;
            }
        }

        fprintf(client, "%s", "Sorry, the server is full");
        fflush(client);
        fclose(client);
        return;
    }
}

int loop(int server_fd) {
    char buff[1024];

    while (1) {
        for (int i = 0; i < MAX_CLIENTS; i ++) {
            if (CLIENTS[i] == NULL) {
                continue;
            }
            
        }
    }
}

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
    int status = loop(server_fd);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (NULL != CLIENTS[i]) {
            fclose(CLIENTS[i]);
        }
    }

    close(server_fd);
    return status;
}