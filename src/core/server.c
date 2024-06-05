
#include "server.h"

#include <arpa/inet.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "core/dns_packet.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 1500

static volatile bool keep_running = 1;

static void int_handler(int dummy)
{
    if (!keep_running) {
        printf("\nForcing exit..\n");
        exit(1);
    }
    keep_running = 0;
    printf("\nExiting.. (press Ctrl-C second time to force exit - beware, this won't free any memory, nor properly "
           "close the sockets!)\n");
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int run_server(struct HoldrConfig *conf)
{
    struct sockaddr_in addr; // address we are listening on
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int sock; // socket we are listening on

    // Exit strategies
    signal(SIGINT, int_handler); // CTRL-C
    // signal(SIGTERM, int_handler); // systemctl stop,

    // Set address and port
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, conf->address, &(addr.sin_addr));
    addr.sin_port = htons(conf->port);

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Set timeout for socket
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting SO_RCVTIMEO on socket");
    }

    // Bind
    if (bind(sock, (struct sockaddr *)&addr, addr_len) < 0) {
        printf("Could not bind: %s\n", strerror(errno));
        return 1;
    }

    printf("Listening on port %u.\n", conf->port);

    // Buffer for input/output packet
    uint8_t buffer[BUFFER_SIZE];
    ssize_t nbytes;

    // To print remote client IP
    char remote_ip[100];
    struct sockaddr_storage client_addr;
    // struct sockaddr_in client_addr;

    // For message processing
    struct Message msg;
    memset(&msg, 0, sizeof(struct Message));

    while (keep_running) {
        memset(&msg, 0, sizeof(struct Message));

        /* Receive DNS query */
        nbytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);

        /* failed to read query */
        if (nbytes < 0) {
            printf("Received nothing..\n");
            continue;
        }
        fprintf(stdout, "New client request from: %s\n",
                inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), remote_ip,
                          INET6_ADDRSTRLEN));

        /* Try to decode query */
        if (!message_decode(&msg, buffer, nbytes)) {
            continue;
        }

        /* Print query */
        message_print(&msg);
    }
}
