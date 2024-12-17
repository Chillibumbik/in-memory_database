// include/server.h
#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>

// Объявления функций
void handle_client(SOCKET client_socket);
void server_loop(int port);
void init_server_socket(SOCKET *server_socket, int port);

#endif
