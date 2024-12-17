// include/client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>

// Объявление функции отправки запроса, теперь с сокетом
void send_request(SOCKET client_socket, const char *request);
void receive_response(SOCKET client_socket, char *buffer, int buffer_size);
void connect_to_server(SOCKET *client_socket, const char *server_ip, int port);

#endif
