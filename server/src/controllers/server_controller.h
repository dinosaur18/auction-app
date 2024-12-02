#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H
#define BUFFER_SIZE 100

int handle_login(int client_socket, char buffer[BUFFER_SIZE]);
int handle_register(int client_socket, char buffer[BUFFER_SIZE]);
void handleCreateRoom(int sockfd, const char *roomName);
// void handleListRooms();
void handleDeleteRoom(int sockfd, int roomId);

void handleCreateItem(int sockfd, const char *itemData);
// void handleListItems(int sockfd, int roomId);
void handleDeleteItem(int sockfd, int itemId);

#endif 
