#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "server_controller.h"
#include "message_type.h"

#define PORT 8080
#define BUFFER_SIZE 100
#define MAX_USERS 100

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

            recv(new_sock, buffer, BUFFER_SIZE, 0);
            switch (buffer[0])
                {
                case LOGIN:
                    handle_login(new_sock, buffer);
                    break;
                case REGISTER:
                    handle_register(new_sock, buffer);
                    break;
                case CREATE_ROOM: {
                    // Assuming buffer contains room name starting from index 1
                    handleCreateRoom(new_sock, buffer + 1);
                    break;
                }
                case DELETE_ROOM: {
                    // Assuming buffer contains room ID (an integer) starting from index 1
                    int roomId = atoi(buffer + 1);
                    handleDeleteRoom(new_sock, roomId);  // Use correct handler for delete room
                    break;
                }
                // case LIST_ROOMS: {
                //     handleListRooms(new_sock);
                //     break;
                // }
                case CREATE_ITEM: {
                    handleCreateItem(new_sock, buffer + 1);
                    break;
                }
                // case LIST_ITEMS: {
                //     int roomId = atoi(buffer + 1);
                //     handleListItems(new_sock, roomId);
                //     break;
                // }
                case DELETE_ITEM: {
                    int itemId = atoi(buffer + 1);
                    handleDeleteItem(new_sock, itemId);
                    break;
                }

                default:
                    printf("Lựa chọn không hợp lệ.\n");

                }
        }
    }
     
    
    // Clean up
    close(sockfd);
    return 0;
}


