#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#define BUFFER_SIZE 100

int handle_login(int client_socket, char buffer[BUFFER_SIZE]);
int handle_register(int client_socket, char buffer[BUFFER_SIZE]);
void handleCreateRoom(const char *roomName);
// void handleListRooms();
void handleDeleteRoom(int roomId);


#endif 
