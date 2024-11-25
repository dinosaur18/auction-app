#include "auth_service.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include "message_type.h"
#include "user.h"

#define BUFFER_SIZE 1024

bool handle_login(int sockfd, const char *username, const char *password) {

    char buffer[BUFFER_SIZE];    
    User user;
    strncpy(user.username, username, sizeof(user.username));
    strncpy(user.password, password, sizeof(user.password));
    
    buffer[0] = LOGIN; 
    // buffer[1] = user;

    // Gửi dữ liệu qua socket
    if (send(sockfd, buffer, strlen(buffer), 0) < 0)
    {
        perror("send failed");
        return false;
    }

    // Đọc phản hồi từ server
    memset(buffer, 0, BUFFER_SIZE);
    
    recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (buffer[0] == LOGIN && buffer[1] == 1) return true;
    else return false;

    return false;
}

bool handle_register(const char *username, const char *password, const char *email, int socket_fd) {

    return true;
}
