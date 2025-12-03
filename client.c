#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {

    struct sockaddr_in address;
    int sock_fd;
    char buff[1024];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < ) {
        perror("socket");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   //127.0.0.1 aka localhost
    address.sin_port = htons(7878);

    if (connect(sock_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("connect");
        return 1;
    }

    int nonblock = fcntl(0, F_SETFLG, O_NONBLOCKING);                 // make stdin nonblocking
    if (nonblock == -1) {
        perror("fcntl");
        return 1;
    }

    int nonblock_sock = fcntl(sock_fd, F_SETFLG, O_NONBLOCKING);     // make server file descriptor nonblocking
    if (nonblock_sock == -1) {
        perror("fcntl");
        return 1;
    }

    FILE *server = fdopen(sock_fd, "r+");                            // convert socket file descriptor in server file
    for (;;) {
        if (fgets(buff, sizeof(buff), stdin) != NULL) {
            if (fprintf(server, "%s", buff) < 0) {
                perror("fprintf");
                fclose(server);
                exit(0);
            }
            fflush(server);
        } else {
            if (errno != EAGAIN || errno != EWOULDBLOCK) {
                fclose(server);
                exit(0);
            }
        }

        if (fgets(buff, sizeof(buff), server) != NULL) {
            if (fprintf(stdout, "%s", buff) < 0) {
                perror("fprintf");
                fclose(server);
                exit(0);
            }
            fflush(stdout);
        } else {
            if (errno != EAGAIN || errno != EWOULDBLOCK) {
                fclose(server);
                exit(0);
            }
        }

        usleep(100 * 1000);                                      // wait 100 ms before checking again
    }

    return 0;
}