#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <stdbool.h>

#define MAX_CLIENTS 1024

typedef struct {
    int sockfd;
    int user_id;  // -1 nếu chưa đăng nhập
    char username[64];
} ClientSession;

void init_sessions();
void add_session(int sockfd);
ClientSession *find_session_by_socket(int sockfd);


#endif 
