#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define MAX_BUFFER 1024

void send_request(SOCKET client_socket, const char *request) {
    send(client_socket, request, strlen(request), 0);
}

void receive_response(SOCKET client_socket) {
    char buffer[MAX_BUFFER];
    int bytes_received = recv(client_socket, buffer, MAX_BUFFER, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';  // Завершаем строку
        printf("Server says: %s\n", buffer);
    }
}

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char server_ip[100];
    int server_port;
    char command[MAX_BUFFER];  // Объявляем как массив символов

    // Инициализация библиотеки Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Ввод IP-адреса и порта сервера
    printf("Enter server IP: ");
    scanf("%s", server_ip);
    printf("Enter server port: ");
    scanf("%d", &server_port);

    // Создание сокета
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        printf("Error creating socket.\n");
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed.\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server at %s:%d\n", server_ip, server_port);

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add data to database\n");
        printf("2. View data in database\n");
        printf("3. Delete data from database\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        getchar();  // чтобы захватить символ новой строки после выбора

        switch (choice) {
            case 1:
                printf("Enter data to add: ");
                fgets(command, MAX_BUFFER, stdin);  // Получаем строку данных
                command[strcspn(command, "\n")] = '\0';  // Убираем символ новой строки

                // Отправляем команду на сервер
                char add_request[MAX_BUFFER];
                snprintf(add_request, MAX_BUFFER, "ADD %s", command);
                send_request(client_socket, add_request);
                receive_response(client_socket);
                break;

            case 2:
                send_request(client_socket, "VIEW");
                receive_response(client_socket);  // Печатаем данные
                break;

            case 3:
                printf("Enter data to delete: ");
                fgets(command, MAX_BUFFER, stdin);  // Получаем строку данных
                command[strcspn(command, "\n")] = '\0';  // Убираем символ новой строки

                // Отправляем команду на сервер
                char del_request[MAX_BUFFER];
                snprintf(del_request, MAX_BUFFER, "DEL %s", command);
                send_request(client_socket, del_request);
                receive_response(client_socket);
                break;

            case 4:
                send_request(client_socket, "EXIT");
                receive_response(client_socket);
                printf("Exiting...\n");
                closesocket(client_socket);
                WSACleanup();
                return 0;

            default:
                printf("Invalid choice! Try again.\n");
                break;
        }
    }

    return 0;
}
