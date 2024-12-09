#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "server_controller.h"
#include "message_type.h"
#include "session_manager.h"

#define PORT 8080
#define BUFFER_SIZE 100
#define MAX_USERS 100

int main()
{
    int server_sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    fd_set read_fds, master_fds;
    int max_fd;

    // Create a socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sockfd, 10) < 0)
    {
        perror("listen");
        close(server_sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Initialize the file descriptor set
    FD_ZERO(&master_fds);
    FD_SET(server_sockfd, &master_fds);
    max_fd = server_sockfd;

    init_sessions();

    while (1)
    {
        read_fds = master_fds;

        // Use select to wait for input or timeout
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
        {
            perror("select failed");
            break;
        }

        for (int fd = 0; fd <= max_fd; fd++)
        {
            if (FD_ISSET(fd, &read_fds))
            {
                if (fd == server_sockfd)
                {
                    // Chấp nhận kết nối mới
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &addrlen);
                    if (client_sockfd > 0)
                    {
                        FD_SET(client_sockfd, &master_fds);
                        if (client_sockfd > max_fd)
                        {
                            max_fd = client_sockfd;
                        }
                        add_session(client_sockfd);
                    }
                }
                else
                {
                    // Xử lý dữ liệu từ client
                    char buffer[1024];
                    int bytes_received = recv(fd, buffer, sizeof(buffer), 0);

                    if (bytes_received <= 0)
                    {
                        // Client đóng kết nối
                        close(fd);
                        FD_CLR(fd, &master_fds);
                        printf("Client disconnected: %d\n", fd);
                    }
                    else
                    {
                        // Xử lý thông điệp
                        switch (buffer[0])
                        {
                        case LOGIN:
                            handle_login(fd, buffer);
                            break;
                        case REGISTER:
                            handle_register(fd, buffer);
                            break;
                        case CREATE_ROOM:
                        {
                            // Assuming buffer contains room name starting from index 1
                            handleCreateRoom(fd, buffer + 1);
                            break;
                        }
                        case DELETE_ROOM:
                        {
                            // Assuming buffer contains room ID (an integer) starting from index 1
                            int roomId = atoi(buffer + 1);
                            handleDeleteRoom(fd, roomId); // Use correct handler for delete room
                            break;
                        }
                        // case LIST_ROOMS: {
                        //     handleListRooms(new_sock);
                        //     break;
                        // }
                        case CREATE_ITEM:
                        {
                            handleCreateItem(fd, buffer + 1);
                            break;
                        }
                        // case LIST_ITEMS: {
                        //     int roomId = atoi(buffer + 1);
                        //     handleListItems(new_sock, roomId);
                        //     break;
                        // }
                        case DELETE_ITEM:
                        {
                            int itemId = atoi(buffer + 1);
                            handleDeleteItem(fd, itemId);
                            break;
                        }
                        default:
                            printf("Unknown message type: %d\n", buffer[0]);
                        }
                    }
                }
            }
        }
    }

    // Clean up
    close(server_sockfd);
    return 0;
}
