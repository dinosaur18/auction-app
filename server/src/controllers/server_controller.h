#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H
#define BUFFER_SIZE 100000

void handle_login(int client_socket, char buffer[BUFFER_SIZE]);
void handle_register(int client_socket, char buffer[BUFFER_SIZE]);
void handleCreateRoom(int client_socket, char buffer[BUFFER_SIZE]);
void handleDeleteRoom(int sockfd, int room_id);
void handleFetchAllRoom(int client_socket);
void handleFetchOwnRoom(int client_socket);
void handleJoinRoom(int client_socket, int room_id);
void handleFetchItems(int client_socket, int room_id);
void handleCreateItem(int sockfd, const char *itemData);
void handleDeleteItem(int sockfd, int itemId);

#endif 
