#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "net.h"

// Прототип функции
void run_client(const char *server_address, int port);

void print_welcome_message() {
    printf("Welcome to the Client!\n");
    printf("Available commands:\n");
    printf("  ADD <key> <value>   - Add a key-value pair to the database.\n");
    printf("  GET <key>           - Retrieve the value for a given key.\n");
    printf("  DELETE <key>        - Remove a key-value pair from the database.\n");
    printf("  VIEW                - View all key-value pairs in the database.\n");
    printf("  EXIT                - Disconnect from the server.\n");
    printf("-------------------------------------------\n");
}

int main(int argc, char *argv[]) {
    const char *server_address = "127.0.0.1";
    int port = 8080;

    if (argc > 1) {
        server_address = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    run_client(server_address, port);

    return 0;
}

void run_client(const char *server_address, int port) {
    SOCKET client_socket = create_client_socket(server_address, port);
    char buffer[1024];

    print_welcome_message();

    while (1) {
        printf("Enter command: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            break;
        }

        if (send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            fprintf(stderr, "Failed to send data to server. Error: %d\n", WSAGetLastError());
            break;
        }

        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            fprintf(stderr, "Connection closed by server or error occurred.\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Server response: %s\n", buffer);

        if (strncmp(buffer, "EXIT", 4) == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    close_socket(client_socket);
}
