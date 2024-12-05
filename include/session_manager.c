#include "session_manager.h"
#include <string.h>
#include <stdlib.h>

void init_sessions() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sessions[i].sockfd = -1; // Socket chưa được sử dụng
        sessions[i].user_id = -1; // Chưa đăng nhập
    }
}

void add_session(int sockfd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (sessions[i].sockfd == -1) {
            sessions[i].sockfd = sockfd;
            sessions[i].user_id = -1; // Chưa đăng nhập
            printf("Client added with socket: %d\n", sockfd);
            return;
        }
    }
    printf("Max clients reached. Cannot add new session.\n");
}

ClientSession *find_session_by_socket(int sockfd) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (sessions[i].sockfd == sockfd) {
            return &sessions[i];
        }
    }
    return NULL;
}

