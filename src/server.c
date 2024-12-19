#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "server.h"
#include "db.h"
#include "./config.h"

int main(int argc, char *argv[]) {
    int port = 8080; // Порт по умолчанию

    // Если указан аргумент, устанавливаем порт
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    printf("Starting server on port %d...\n", port);

    // Запускаем сервер
    start_server(port);

    return 0;
}


void start_server(int port) {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int client_len = sizeof(client_address);

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

    // Создание серверного сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create server socket. Error: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Привязка сокета
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        fprintf(stderr, "Failed to bind server socket. Error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Перевод сокета в режим ожидания подключений
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "Failed to listen on server socket. Error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d. Waiting for clients...\n", port);

    // Инициализация базы данных
    Database *db = create_database();

    while (1) {
        // Принятие подключения клиента
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);
        if (client_socket == INVALID_SOCKET) {
            fprintf(stderr, "Failed to accept client connection. Error: %d\n", WSAGetLastError());
            continue;
        }

        printf("Client connected\n");

        // Обработка подключения клиента
        handle_client_connection(client_socket, db);

        // Закрытие клиентского сокета
        closesocket(client_socket);
        printf("Client disconnected\n");
    }

    // Очистка ресурсов
    destroy_database(db);
    closesocket(server_socket);
    WSACleanup();
}

void handle_client_connection(SOCKET client_socket, Database *db) {
    char buffer[1024];
    int bytes_received;

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Получение данных от клиента
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected or error occurred.\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        if (strcmp(buffer, "EXIT\n") == 0) {
            printf("Client requested to disconnect.\n");
            break;
        } else if (strncmp(buffer, "ADD ", 4) == 0) {
            char *key = strtok(buffer + 4, " ");
            char *value = strtok(NULL, "\n");
            if (key && value) {
                if (add_data(db, key, value)) {
                    send(client_socket, "Data added successfully\n", 24, 0);
                } else {
                    send(client_socket, "Failed to add data\n", 20, 0);
                }
            } else {
                send(client_socket, "Invalid ADD command format\n", 28, 0);
            }
        } else if (strncmp(buffer, "GET ", 4) == 0) {
            char *key = strtok(buffer + 4, "\n");
            if (key) {
                char *value = get_data(db, key);
                if (value) {
                    send(client_socket, value, strlen(value), 0);
                    send(client_socket, "\n", 1, 0);
                } else {
                    send(client_socket, "Key not found\n", 15, 0);
                }
            } else {
                send(client_socket, "Invalid GET command format\n", 28, 0);
            }
        } else if (strncmp(buffer, "DELETE ", 7) == 0) {
            char *key = strtok(buffer + 7, "\n");
            if (key) {
                if (delete_key(db->map, key)) {
                    send(client_socket, "Data deleted successfully\n", 26, 0);
                } else {
                    send(client_socket, "Key not found\n", 15, 0);
                }
            } else {
                send(client_socket, "Invalid DELETE command format\n", 31, 0);
            }
        } else if (strcmp(buffer, "VIEW\n") == 0) {
            char response[1024];
            memset(response, 0, sizeof(response));

            // Используем временный файл
            FILE *temp = tmpfile();
            if (!temp) {
                send(client_socket, "Failed to view data\n", 21, 0);
                continue;
            }

            // Вывод данных из базы в файл
            view_all_data(db, temp);

            // Перематываем файл и отправляем данные клиенту
            rewind(temp);
            while (fgets(response, sizeof(response), temp)) {
                send(client_socket, response, strlen(response), 0);
            }

            fclose(temp);
            send(client_socket, "\n", 1, 0); // Завершить вывод
        } else {
            send(client_socket, "Unknown command\n", 17, 0);
        }
    }
}
