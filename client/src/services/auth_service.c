#include "auth_service.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include "message_type.h"
#include "user.h"

#define BUFFER_SIZE 1024

int handle_login(int sockfd, const char *username, const char *password) {

    char buffer[BUFFER_SIZE];    
    User user;
    strncpy(user.username, username, sizeof(user.username));
    strncpy(user.password, password, sizeof(user.password));
    
    buffer[0] = LOGIN; 
    memcpy(&buffer[1], &user, sizeof(user));

    // Gửi dữ liệu qua socket
    if (send(sockfd, buffer, strlen(buffer), 0) < 0)
    {
        perror("send failed");
        return 0;
    }

    // Đọc phản hồi từ server
    memset(buffer, 0, BUFFER_SIZE);
    
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (buffer[0] == LOGIN && buffer[1] == 1) return 1;
    else return 0;

    return 0;
}

int handle_register(const char *username, const char *password, const char *email, int socket_fd) {

    return 0;
}
