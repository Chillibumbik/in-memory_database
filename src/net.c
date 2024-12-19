#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net.h"

// Создание клиентского сокета
int create_client_socket(const char *server_address, int port) {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_addr;

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

    // Создание сокета
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create socket. Error: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_address);

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Failed to connect to server. Error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

// Закрытие сокета
void close_socket(int socket) {
    closesocket(socket);
    WSACleanup();
}
