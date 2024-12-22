#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <stdbool.h>
#include "user.h"

#define BUFFER_SIZE 100000

int handle_login(int sockfd, const char *username, const char *password, User *user_data);
int handle_register(int sockfd, const char *username, const char *password, User *user_data);

#endif // AUTH_SERVICE_H
