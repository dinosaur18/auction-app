#include <stdio.h>
#include <string.h>
#include "user.h"

// Hàm để lấy ID người dùng tiếp theo (tăng dần)
int getNextUserId() {
    int nextUserId = 1;
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL) {
        return nextUserId; // Nếu file chưa tồn tại, bắt đầu với ID 1
    }

    User user;
    while (fscanf(file, "%d %s %s\n", &user.userId, user.username, user.password) == 3) {
        if (user.userId >= nextUserId) {
            nextUserId = user.userId + 1;
        }
    }
    fclose(file);
    return nextUserId;
}

// Hàm lưu thông tin người dùng vào file .txt
int saveUser(User user) {
    if (checkUserExists(user.username)) {
        return 0; // Username đã tồn tại
    }

    FILE *file = fopen("data/users.txt", "a");
    if (file == NULL) {
        return -1; 
    }

    user.userId = getNextUserId();
    fprintf(file, "%d %s %s\n", user.userId, user.username, user.password);
    fclose(file);
    return user.userId;
}

// Hàm kiểm tra xem username đã tồn tại chưa
int checkUserExists(const char *username) {
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL) {
        return 0;
    }

    User user;
    while (fscanf(file, "%d %s %s\n", &user.userId, user.username, user.password) == 3) {
        if (strcmp(user.username, username) == 0) {
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0; 
}

// Hàm xác thực thông tin người dùng (username và password)
int authenticateUser(User user) {
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL) {
        return -1;
    }
    User registeredUser;
    while (fscanf(file, "%d %s %s\n", &registeredUser.userId, registeredUser.username, registeredUser.password) == 3) {
        if (strcmp(registeredUser.username, user.username) == 0 && strcmp(registeredUser.password, user.password) == 0) {
            fclose(file);
            return registeredUser.userId; 
        }
    }
    fclose(file);
    return 0; 
}