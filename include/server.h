#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include "db.h"

void start_server(int port);
void handle_client_connection(SOCKET client_socket, Database *db);

#endif // SERVER_H
