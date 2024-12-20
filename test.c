#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_LENGTH 50

// Định nghĩa cấu trúc người dùng
typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} User;

#define MAX_ROOM_NAME_LENGTH 100

#define MAX_ROOMS 100

// Cấu trúc thông tin phòng đấu giá
typedef struct {
    int room_id;                    // ID của phòng
    char roomName[MAX_ROOM_NAME_LENGTH]; // Tên phòng đấu giá
    int numUsers;                  // Số người tham gia
    int numItems;                  // Số vật phẩm trong phòng
    char username[MAX_LENGTH];
} Room;

// Hàm đăng ký người dùng
void registerUser() {
    FILE *file = fopen("data/users.dat", "ab");
    if (file == NULL) {
        printf("Không thể mở file.\n");
        return;
    }
    
    User user;
    printf("Nhập tên người dùng: ");
    scanf("%s", user.username);
    printf("Nhập mật khẩu: ");
    scanf("%s", user.password);
    
    // Lưu thông tin người dùng vào file
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
    
    printf("Đăng ký thành công!\n");
}

// Hàm hiển thị tất cả người dùng
void displayUsers() {
    FILE *file = fopen("data/users.dat", "rb");
    if (file == NULL) {
        printf("Không có thông tin người dùng.\n");
        return;
    }
    
    User user;
    printf("\nDanh sách người dùng đã đăng ký:\n");
    printf("---------------------------------\n");
    
    while (fread(&user, sizeof(User), 1, file)) {
        printf("Tên người dùng: %s\n", user.username);
        printf("Password: %s\n", user.password);
        printf("-----------------------------\n");
    }
    fclose(file);
}

void displayRooms() {
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL) {
        printf("Không có thông tin người dùng.\n");
        return;
    }
    
    Room room;
    printf("\nDanh sách room:\n");
    printf("---------------------------------\n");
    
    while (fread(&room, sizeof(Room), 1, file)) {
        printf("id: %d\n", room.room_id);
        printf("Tên phòng: %s\n", room.roomName);
        printf("user: %s\n", room.username);
        printf("-----------------------------\n");
    }
    fclose(file);
}

// Hàm đăng nhập người dùng
void loginUser() {
    FILE *file = fopen("data/users.dat", "rb");
    if (file == NULL) {
        printf("Không có thông tin người dùng.\n");
        return;
    }
    
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    User user;
    int isAuthenticated = 0;

    printf("Nhập tên người dùng: ");
    scanf("%s", username);
    printf("Nhập mật khẩu: ");
    scanf("%s", password);

    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            isAuthenticated = 1;
            break;
        }
    }
    fclose(file);

    if (isAuthenticated) {
        printf("Đăng nhập thành công!\n");
    } else {
        printf("Tên người dùng hoặc mật khẩu không đúng.\n");
    }
}

int main() {
    int choice;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Đăng ký\n");
        printf("2. Đăng nhập\n");
        printf("3. Hiển thị tất cả người dùng\n");
        printf("4. Thoát\n");
        printf("Lựa chọn của bạn: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                displayUsers();
                break;
            case 4:
                displayRooms();
                break;
            default:
                printf("Lựa chọn không hợp lệ.\n");
        }
    } while (choice != 4);

    return 0;
}
