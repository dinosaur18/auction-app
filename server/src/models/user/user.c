#include <stdio.h>
#include <string.h>
#include "user.h"

// Hàm để lấy ID người dùng tiếp theo (tăng dần)
int getNextuser_id() {
    int nextuser_id = 1;
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL) {
        return nextuser_id; // Nếu file chưa tồn tại, bắt đầu với ID 1
    }

    User user;
    while (fscanf(file, "%d %s %s %d\n", &user.user_id, user.username, user.password, &user.money) == 4) {
        if (user.user_id >= nextuser_id) {
            nextuser_id = user.user_id + 1;
        }
    }
    fclose(file);
    return nextuser_id;
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

    user.user_id = getNextuser_id();
    user.money = 10000;
    fprintf(file, "%d %s %s %d\n", user.user_id, user.username, user.password, user.money);
    fclose(file);
    return user.user_id;
}

// Hàm kiểm tra xem username đã tồn tại chưa
int checkUserExists(const char *username) {
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL) {
        return 0;
    }

    User user;
    while (fscanf(file, "%d %s %s %d\n", &user.user_id, user.username, user.password, &user.money) == 4) {
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
    while (fscanf(file, "%d %s %s %d\n", &registeredUser.user_id, registeredUser.username, registeredUser.password, &registeredUser.money) == 4) {
        if (strcmp(registeredUser.username, user.username) == 0 && strcmp(registeredUser.password, user.password) == 0) {
            fclose(file);
            return registeredUser.user_id; 
        }
    }
    fclose(file);
    return 0; 
}

// Hàm lấy thông tin người dùng theo id
int getUserById(int user_id, User *user)
{
    FILE *file = fopen("data/users.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    User existedUser;
    while (fscanf(file, "%d %s %s %d\n", &existedUser.user_id, existedUser.username, existedUser.password, &existedUser.money) == 4)
    {
        if (existedUser.user_id == user_id)
        {
            *user = existedUser;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
