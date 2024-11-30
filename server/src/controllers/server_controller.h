#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#define BUFFER_SIZE 100

int handle_login(int client_socket, char buffer[BUFFER_SIZE]);
int handle_register(int client_socket, char buffer[BUFFER_SIZE]);
// int handle_createRoom(int client_socket);
// int handle_deleteRoom(int client_socket);
// int handle_createItem(int client_socket);
// int handle_deleteItem(int client_socket);

#endif 
