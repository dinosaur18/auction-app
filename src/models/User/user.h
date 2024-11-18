#ifndef USER_H
#define USER_H

#define MAX_LENGTH 50

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;

// Khai báo hàm
int saveUser(User user);
int checkUserExists(const char *username);
int authenticateUser(User user);

#endif // USER_H
