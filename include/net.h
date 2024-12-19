#ifndef NET_H
#define NET_H

// Функции для работы с сокетами
int create_server_socket(int port);
int create_client_socket(const char *server_address, int port);
void close_socket(int socket);

#endif 
