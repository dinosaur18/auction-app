#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H
#define BUFFER_SIZE 1024

void handle_login(int client_socket, char buffer[BUFFER_SIZE]);
void handle_register(int client_socket, char buffer[BUFFER_SIZE]);
void handleCreateRoom(int client_socket, const char *roomName);
void handleFetchAllRoom(int client_socket);
void handleFetchOwnRoom(int client_socket);
// void handleListRooms();
void handleDeleteRoom(int sockfd, int roomId);

void handleCreateItem(int sockfd, const char *itemData);
// void handleListItems(int sockfd, int roomId);
void handleDeleteItem(int sockfd, int itemId);

#endif 
