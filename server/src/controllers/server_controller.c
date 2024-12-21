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
    memcpy(&user, &buffer[1], sizeof(user)); 

    printf("Nhận thông tin đăng ký từ client: %s - %s\n", user.username, user.password);

    user.money = 10000;
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

void handleCreateRoom(int client_socket, char buffer[BUFFER_SIZE])
{
    char roomName[MAX_LENGTH];
    memcpy(roomName, &buffer[1], MAX_LENGTH);
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
void handleDeleteRoom(int sockfd, int room_id)
{
    // Xử lý việc xóa phòng
    if (room_id >= 0)
    {
        deleteRoom(room_id); // Gọi hàm deleteRoom để xóa phòng
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
    sscanf(itemData, "%d %s %d %d %d", &newItem.item_id, newItem.name, &newItem.startingPrice, &newItem.buyNowPrice, &newItem.auctionTime);

    if (saveItem(newItem) > 0)
    {
        send(sockfd, "The item has been successfully created.", 30, 0);
    }
    else
    {
        send(sockfd, "Error creating item.", 20, 0);
    }
}

// Hàm xử lý xóa vật phẩm
void handleDeleteItem(int sockfd, int item_id)
{
    if (deleteItem(item_id) > 0)
    {
        send(sockfd, "The item has been successfully deleted.", 30, 0);
    }
    else
    {
        send(sockfd, "Error deleting item.", 20, 0);
    }
}

void handleJoinRoom(int client_socket, int room_id)
{

    char buffer[BUFFER_SIZE];
    ClientSession *session = find_session_by_socket(client_socket);
    Room room;
    int result = getRoomById(room_id, &room);
    printf("%d %s \n", room.room_id, session->username);

    if (result == 0 || session == NULL)
    {
        int response = 0; // Error
        send(client_socket, &response, 1, 0);
        return;
    }

    // Kiểm tra user
    int role = 2;
    if (strcmp(room.username, session->username) == 0)
    {
        role = 1; // Owner
    }
    else
    {
        role = 2; // Joiner
    }

    // Đóng gói dữ liệu
    memcpy(&buffer[0], &role, 1);
    memcpy(&buffer[1], &room, sizeof(Room));

    // Gửi dữ liệu cho client
    if (send(client_socket, buffer, sizeof(Room) + 1, 0) < 0)
    {
        perror("Error sending room data");
        return;
    }
}

void handleFetchItems(int client_socket, int room_id) {
    char buffer[BUFFER_SIZE];
    Item items[MAX_ITEM_IN_ROOM];
    int item_count = loadItems(room_id, items);

    memcpy(&buffer[0], &item_count, 1);
    memcpy(&buffer[1], &items, item_count * sizeof(Item));

    if (send(client_socket, buffer, (item_count * sizeof(Item)) + 1, 0) < 0)
    {
        perror("Error sending room data");
        return;
    }
}