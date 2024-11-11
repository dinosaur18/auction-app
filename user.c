#include <stdio.h>
#include <string.h>
#include "user.h"

int saveUser(User user) {
    if (checkUserExists(user.username)) {
        return 0; // Username đã tồn tại
    }

    FILE *file = fopen("users.dat", "ab");
    if (file == NULL) {
        return -1; 
    }

    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
    return 1;
}

int checkUserExists(const char *username) {
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL) {
        return 0;
    }

    User user;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0; 
}

int authenticateUser(User user) {
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL) {
        return -1;
    }

    User registeredUser;
    while (fread(&registeredUser, sizeof(User), 1, file)) {
        if (strcmp(registeredUser.username, user.username) == 0 && strcmp(registeredUser.password, user.password) == 0) {
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0; 
}
