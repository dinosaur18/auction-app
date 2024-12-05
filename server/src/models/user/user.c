#include <stdio.h>
#include <string.h>
#include "user.h"

// Hàm để lấy ID người dùng tiếp theo (tăng dần)
int getNextUserId() {
    int nextUserId = 1;
    FILE *file = fopen("data/users.dat", "rb");
    if (file == NULL) {
        return nextUserId; // Nếu file chưa tồn tại, bắt đầu với ID 1
    }

    User user;
    while (fread(&user, sizeof(User), 1, file)) {
        if (user.userId >= nextUserId) {
            nextUserId ++;
        }
    }
    fclose(file);
    return nextUserId;
}

int saveUser(User user) {
    if (checkUserExists(user.username)) {
        return 0; // Username đã tồn tại
    }

    FILE *file = fopen("data/users.dat", "ab");
    if (file == NULL) {
        return -1; 
    }

    user.userId = getNextUserId();

    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
    return 1;
}

int checkUserExists(const char *username) {
    FILE *file = fopen("data/users.dat", "rb");
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
    FILE *file = fopen("data/users.dat", "rb");
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
