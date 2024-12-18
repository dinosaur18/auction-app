#include <gtk/gtk.h>
#include <sys/socket.h>
#include <string.h>
#include "user.h"
#include "message_type.h"
#include "session_manager.h"
#include "server_controller.h"
#include "room.h"
#include "item.h"

void handle_login(int client_socket, char buffer[BUFFER_SIZE])
{
    User user;
    memcpy(&user, &buffer[1], sizeof(user)); // Deserialize dữ liệu từ buffer

    printf("Nhận thông tin đăng nhập từ client: %s - %s\n", user.username, user.password);

    // Lưu thông tin người dùng
    int user_id = authenticateUser(user);
    ClientSession *session = find_session_by_socket(client_socket);

    if (user_id > 0 && session != NULL)
    {
        session->user_id = user_id;
        strncpy(session->username, user.username, sizeof(session->username));
        printf("User %s logged in with user_id %d\n", user.username, user_id);

        int response = 1; // Đăng nhập thành công
        send(client_socket, &response, 1, 0);
    }
    else
    {
        int response = 0; // Đăng nhập thất bại
        send(client_socket, &response, 1, 0);
    }
}

void handle_register(int client_socket, char buffer[BUFFER_SIZE])
{
    User user;
    memcpy(&user, &buffer[1], sizeof(user)); // Deserialize dữ liệu từ buffer

    printf("Nhận thông tin đăng ký từ client: %s - %s\n", user.username, user.password);

    // Lưu thông tin người dùng
    int user_id = saveUser(user);
    ClientSession *session = find_session_by_socket(client_socket);

    if (user_id > 0 && session != NULL)
    {
        session->user_id = user_id;
        strncpy(session->username, user.username, sizeof(session->username));
        printf("User %s register with user_id %d\n", user.username, user_id);

        int response = 1; // Đăng ký thành công
        send(client_socket, &response, 1, 0);
    }
    else
    {
        int response = 0; // Đăng ký thất bại
        send(client_socket, &response, 1, 0);
    }
}

void handleCreateRoom(int client_socket, const char *roomName)
{
    ClientSession *session = find_session_by_socket(client_socket);

    if (session != NULL)
    {
        int response = createRoom(roomName, session->username);
        send(client_socket, &response, 1, 0);
    }
    else
    {
        int response = 0;
        send(client_socket, &response, 1, 0);
    }
}

void handleFetchAllRoom(int client_socket)
{
    char buffer[BUFFER_SIZE];
    Room rooms[MAX_ROOMS];
    int room_count = loadRooms(rooms, NULL);

    if (room_count < 0)
    {
        int response = 0;
        send(client_socket, &response, 1, 0);
        return;
    }

    memcpy(&buffer[0], &room_count, 1);
    memcpy(&buffer[1], &rooms, room_count * sizeof(Room));

    if (send(client_socket, buffer, (room_count * sizeof(Room)) + 1, 0) < 0)
    {
        perror("Error sending room data");
        return;
    }
}

void handleFetchOwnRoom(int client_socket)
{
    ClientSession *session = find_session_by_socket(client_socket);

    if (session != NULL)
    {
        char buffer[BUFFER_SIZE];
        Room rooms[MAX_ROOMS];
        int room_count = loadRooms(rooms, session->username);

        if (room_count < 0)
        {
            int response = 0;
            send(client_socket, &response, 1, 0);
            return;
        }

        memcpy(&buffer[0], &room_count, 1);
        memcpy(&buffer[1], &rooms, room_count * sizeof(Room));

        if (send(client_socket, buffer, (room_count * sizeof(Room)) + 1, 0) < 0)
        {
            perror("Error sending room data");
            return;
        }
    }
    else
    {
        int response = 0;
        send(client_socket, &response, 1, 0);
    }
}

// Hàm xóa phòng đấu giá
void handleDeleteRoom(int sockfd, int roomId)
{
    // Xử lý việc xóa phòng
    if (roomId >= 0)
    {
        deleteRoom(roomId); // Gọi hàm deleteRoom để xóa phòng
        send(sockfd, "The auction room has been deleted.", 28, 0);
    }
    else
    {
        send(sockfd, "Invalid room ID.", 24, 0);
    }
}

// Hàm xử lý tạo vật phẩm
void handleCreateItem(int sockfd, const char *itemData)
{
    // Giả sử itemData chứa thông tin vật phẩm dưới dạng chuỗi, cần phân tích để tạo Item
    Item newItem;
    sscanf(itemData, "%d %s %d %d %d", &newItem.itemId, newItem.name, &newItem.startingPrice, &newItem.buyNowPrice, &newItem.auctionTime);

    if (saveItem(newItem) > 0)
    {
        send(sockfd, "The item has been successfully created.", 30, 0);
    }
    else
    {
        send(sockfd, "Error creating item.", 20, 0);
    }
}

// Hàm xử lý liệt kê các vật phẩm trong phòng
// void handleListItems(int sockfd, int roomId) {
//     Item items[10];  // Giới hạn 10 vật phẩm trong một lần yêu cầu
//     int count = listItems(roomId, items, 10);

//     if (count > 0) {
//         for (int i = 0; i < count; i++) {
//             char buffer[200];
//             sprintf(buffer, "ID: %d, Tên: %s, Giá khởi điểm: %d, Giá bán ngay: %d, Thời gian: %d\n",
//                     items[i].itemId, items[i].name, items[i].startingPrice, items[i].buyNowPrice, items[i].auctionTime);
//             send(sockfd, buffer, strlen(buffer), 0);
//         }
//     } else {
//         send(sockfd, "Không có vật phẩm nào trong phòng đấu giá.", 40, 0);
//     }
// }

// Hàm xử lý xóa vật phẩm
void handleDeleteItem(int sockfd, int itemId)
{
    if (deleteItem(itemId) > 0)
    {
        send(sockfd, "The item has been successfully deleted.", 30, 0);
    }
    else
    {
        send(sockfd, "Error deleting item.", 20, 0);
    }
}