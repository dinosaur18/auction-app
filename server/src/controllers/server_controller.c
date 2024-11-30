#include <gtk/gtk.h>
#include <sys/socket.h>
#include <string.h>
#include "user.h"
#include "message_type.h"

#define BUFFER_SIZE 1024

int handle_login(int client_socket, char buffer[BUFFER_SIZE])
{
    User user;
    memcpy(&user, &buffer[1], sizeof(user)); // Deserialize dữ liệu từ buffer

    printf("Nhận thông tin đăng nhập từ client: %s - %s\n", user.username, user.password);

    // Lưu thông tin người dùng
    int result = authenticateUser(user);
    if (result == 1)
    {
        buffer[0] = 0x01;
        send(client_socket, buffer, 1, 0);
    }
    else
    {
        buffer[0] = 0x00;
        send(client_socket, buffer, 1, 0);
    }

    return 0;
}

int handle_register(int client_socket, char buffer[BUFFER_SIZE])
{
    User user;
    memcpy(&user, &buffer[1], sizeof(user)); // Deserialize dữ liệu từ buffer

    printf("Nhận thông tin đăng ký từ client: %s - %s\n", user.username, user.password);
    int result = saveUser(user);

    if (result)
    {
        buffer[0] = 0x01;
        send(client_socket, buffer, strlen(buffer), 0);
    }
    else
    {
        buffer[0] = 0x00;
        send(client_socket, buffer, strlen(buffer), 0);
    }
    return 0;
}
