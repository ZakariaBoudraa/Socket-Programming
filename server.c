#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

const int MAX_CLIENTS = 4;
FILE *CLIENTS[MAX_CLIENTS] = {0};

void redistribute_message(int sender_i, char *buf) {
    for (int i = 0; i < MAX_CLIENTS; i ++) {
        if (CLIENTS[i] == NULL || i == sender_i) {
            continue;
        } else {
            if (0 > fprintf(CLIENTS[i], "%s", buf)) {
                fclose(CLIENTS[i]);
                CLIENTS[i] == NULL;
            } else if (EOF == fflush(CLIENTS[i])) {
                fclose(CLIENTS[i]);
                CLIENTS[i] == NULL;
            }
        }
    }
}

int main(int argc, char *argv[]) {


}