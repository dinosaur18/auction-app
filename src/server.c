#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "models/User/user.h"

#define PORT 8080
#define BUFFER_SIZE 100
#define MAX_USERS 100

void handle_register(int client_socket);
void handle_login(int client_socket);

int main()
{
    int sockfd, new_sock, client_socks[MAX_USERS];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    fd_set read_fds;
    struct timeval timeout;
    char buffer[BUFFER_SIZE];
    int max_fd, fd;

    for (int i = 0; i < MAX_USERS; i++)
    {
        client_socks[i] = 0;
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 10) < 0)
    {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        // Initialize the file descriptor set
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(sockfd, &read_fds);
        max_fd = sockfd;

        for (int i = 0; i < MAX_USERS; i++)
        {
            fd = client_socks[i];

            if (fd > 0)
            {
                FD_SET(fd, &read_fds);
            }

            // Xác định file descriptor có giá trị lớn nhất để dùng cho select
            if (fd > max_fd)
            {
                max_fd = fd;
            }
        }

        // Set the timeout to 5 seconds
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // Use select to wait for input or timeout
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (activity == -1)
        {
            perror("select");
            break;
        }

        // Kiểm tra nếu có kết nối mới trên server socket
        if (FD_ISSET(sockfd, &read_fds))
        {
            new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);
            if (new_sock < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            // Thêm client mới vào mảng client_socks
            for (int i = 0; i < MAX_USERS; i++)
            {
                if (client_socks[i] == 0)
                {
                    client_socks[i] = new_sock;
                    printf("Added new client to socket list at index %d\n", i);
                    break;
                }
            }

            read(new_sock, buffer, sizeof(buffer));
            int choice = atoi(buffer);
            printf("%d\n", choice);

            switch (choice)
                {
                case 1:
                    handle_register(new_sock);
                    break;
                case 2:
                    handle_login(new_sock);
                    break;
                default:
                    printf("Lựa chọn không hợp lệ.\n");
                }
            
        }

        // for (int i = 0; i < MAX_USERS; i++)
        // {
        //     fd = client_socks[i];

        //     // Check for data from the client socket
        //     if (FD_ISSET(fd, &read_fds))
        //     {
        //         int bytes_received = recv(fd, buffer, sizeof(buffer) - 1, 0);
        //         if (bytes_received <= 0)
        //         {
        //             // Client disconnected or error
        //             getpeername(fd, (struct sockaddr *)&client_addr, &addrlen);
        //             printf("Client disconnected: ip %s, port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        //             perror("recv");

        //             // Đóng socket và xóa khỏi mảng client_socks
        //             close(fd);
        //             client_socks[i] = 0;
        //             break;
        //         }
        //         else
        //         {
        //             buffer[bytes_received] = '\0'; // Null-terminate the received data
        //             printf("Received from client: %s", buffer);

        //             // Gửi tin nhắn đến tất cả các client khác
        //             for (int j = 0; j < MAX_USERS; j++)
        //             {
        //                 if (client_socks[j] != 0 && client_socks[j] != fd) // Không gửi lại cho chính client đã gửi
        //                 {
        //                     send(client_socks[j], buffer, bytes_received, 0);
        //                 }
        //             }
        //         }
        //     }
        // }
    }
    // Clean up
    close(sockfd);
    return 0;
}

void handle_register(int client_socket)
{
    User user;

    // Nhận dữ liệu từ client
    read(client_socket, &user, sizeof(User));
    printf("Nhận thông tin đăng ký từ client: %s\n", user.username);

    // Lưu thông tin người dùng
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
}

void handle_login(int client_socket)
{
    User user;

    // Nhận dữ liệu từ client
    read(client_socket, &user, sizeof(User));
    printf("Nhận thông tin đăng ký từ client: %s\n", user.username);

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
}
