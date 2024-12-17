#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define MAX_BUFFER 1024
#define MAX_DATA 100 // Максимальное количество данных в базе

// Простая база данных для хранения строк
char database[MAX_DATA][MAX_BUFFER];
int database_size = 0;

void handle_client(SOCKET client_socket) {
    char buffer[MAX_BUFFER];
    int bytes_received;

    while (1) {
        // Получаем запрос от клиента
        bytes_received = recv(client_socket, buffer, MAX_BUFFER, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';  // Завершаем строку
        printf("Received from client: %s\n", buffer);

        if (strncmp(buffer, "ADD ", 4) == 0) {
            // Добавляем данные в базу
            if (database_size < MAX_DATA) {
                strcpy(database[database_size], buffer + 4); // Сохраняем данные в базе
                database_size++;
                send(client_socket, "Data added to database.", 23, 0);
            } else {
                send(client_socket, "Database is full.", 18, 0);
            }
        } else if (strcmp(buffer, "VIEW") == 0) {
            // Отправляем все данные из базы
            if (database_size == 0) {
                send(client_socket, "No data in database.", 21, 0);
            } else {
                char response[MAX_BUFFER * MAX_DATA];
                memset(response, 0, sizeof(response));
                for (int i = 0; i < database_size; i++) {
                    strcat(response, database[i]);
                    strcat(response, "\n");
                }
                send(client_socket, response, strlen(response), 0);
            }
        } else if (strncmp(buffer, "DEL ", 4) == 0) {
            // Удаляем данные из базы
            int i, found = 0;
            for (i = 0; i < database_size; i++) {
                if (strcmp(database[i], buffer + 4) == 0) {
                    // Сдвигаем элементы, чтобы удалить строку
                    for (int j = i; j < database_size - 1; j++) {
                        strcpy(database[j], database[j + 1]);
                    }
                    database_size--;
                    found = 1;
                    break;
                }
            }
            if (found) {
                send(client_socket, "Data deleted from database.", 26, 0);
            } else {
                send(client_socket, "Data not found.", 16, 0);
            }
        } else if (strcmp(buffer, "EXIT") == 0) {
            // Закрываем соединение с клиентом
            send(client_socket, "Goodbye!", 8, 0);
            break;
        } else {
            // Неизвестная команда
            send(client_socket, "Unknown command.", 17, 0);
        }
    }

    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);

    // Инициализация библиотеки Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Error creating socket.\n");
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Привязываем сокет к адресу
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening on port 12345...\n");

    // Ожидаем подключение клиента
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed.\n");
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        printf("Client connected.\n");

        // Обрабатываем запросы от клиента
        handle_client(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
