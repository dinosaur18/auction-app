#include <gtk/gtk.h>
#include <sys/socket.h>
#include <string.h>
#include "user.h"
#include "message_type.h"

#define BUFFER_SIZE 100

int handle_client(int client_socket)
{

    char buffer[BUFFER_SIZE];
    recv(client_socket, buffer, BUFFER_SIZE, 0);

    switch (buffer[0])
    {
    case LOGIN:
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
        break;
    case REGISTER:
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
        break;
    default:
        break;
    }
}