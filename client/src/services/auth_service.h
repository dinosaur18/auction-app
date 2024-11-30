#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <stdbool.h>

int handle_login(int sockfd, const char *username, const char *password);
int handle_register(const char *username, const char *password, const char *email, int socket_fd);

#endif // AUTH_SERVICE_H
