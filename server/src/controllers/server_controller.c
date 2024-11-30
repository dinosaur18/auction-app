#include <gtk/gtk.h>
#include <sys/socket.h>
#include <string.h>
#include "user.h"
#include "message_type.h"
#include "room.h"

#define BUFFER_SIZE 100

int handle_login(int client_socket, char buffer[BUFFER_SIZE])
{
    User user;
    memcpy(&user, &buffer[1], sizeof(user)); // Deserialize dữ liệu từ buffer

    printf("Nhận thông tin đăng nhập từ client: %s\n", user.username);

    // Lưu thông tin người dùng
    int result = authenticateUser(user);

    if (result)
    {
        char *success = "Đăng nhập thành công!";
        send(client_socket, success, strlen(success), 0);
    }
    else
    {
        char *error = "Sai thông tin đăng nhập!";
        send(client_socket, error, strlen(error), 0);
    }
    return 0;
}

int handle_register(int client_socket, char buffer[BUFFER_SIZE])
{
    User user;
    memcpy(&user, &buffer[1], sizeof(user)); // Deserialize dữ liệu từ buffer

    printf("Nhận thông tin đăng ký từ client: %s\n", user.username);
    int result = saveUser(user);

    if (result == 1)
    {
        char *success = "Đăng ký thành công!";
        send(client_socket, success, strlen(success), 0);
    }
    else if (result == 0)
    {
        char *exists = "Người dùng đã tồn tại!";
        send(client_socket, exists, strlen(exists), 0);
    }
    else
    {
        char *error = "Lỗi lưu thông tin!";
        send(client_socket, error, strlen(error), 0);
    }
    return 0;
}

void handleCreateRoom(const char *roomName) {
    if (createRoom(roomName) > 0) {
        printf("Phòng '%s' đã được tạo thành công!\n", roomName);
    } else {
        printf("Lỗi khi tạo phòng '%s'.\n", roomName);
    }
}

// // Hàm liệt kê các phòng đã tồn tại
// void handleListRooms() {
//     Room rooms[100];
//     int numRooms = 0;

//     if (loadRooms(rooms, &numRooms)) {
//         if (numRooms == 0) {
//             printf("Không có phòng đấu giá nào.\n");
//         } else {
//             printf("Danh sách các phòng đấu giá:\n");
//             for (int i = 0; i < numRooms; i++) {
//                 printf("ID: %d, Tên: %s\n", rooms[i].roomId, rooms[i].roomName);
//             }
//         }
//     } else {
//         printf("Không thể tải phòng đấu giá.\n");
//     }
// }

// Hàm xóa phòng đấu giá
void handleDeleteRoom(int roomId) {
    if (deleteRoom(roomId)) {
        printf("Phòng đấu giá với ID %d đã được xóa thành công!\n", roomId);
    } else {
        printf("Không thể xóa phòng với ID %d.\n", roomId);
    }
}

