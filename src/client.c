#include <stdio.h>
<<<<<<< HEAD

void run_auth_view();

int main() {
    printf("Chạy ứng dụng client...\n");

    // Gọi giao diện đăng nhập
    run_auth_view();

=======
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "user.h"

#define PORT 8080
#define BUFFER_SIZE 100

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    fd_set read_fds; // Tập hợp file descriptor cho select
    int max_fd;
    User user;

    // Tạo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Cấu hình địa chỉ server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Địa chỉ server

    // Kết nối đến server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");
    printf("\n--- MENU ---\n");
    printf("1. Đăng ký\n");
    printf("2. Đăng nhập\n");
    printf("Lựa chọn của bạn: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    send(sockfd, buffer, strlen(buffer), 0);

    printf("Nhập tên người dùng: ");
    scanf("%s", user.username);
    printf("Nhập mật khẩu: ");
    scanf("%s", user.password);

    send(sockfd, &user, sizeof(User), 0);

    while (1)
    {
        // Khởi tạo tập hợp file descriptor
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds); // Theo dõi input từ người dùng
        FD_SET(sockfd, &read_fds);       // Theo dõi phản hồi từ server

        // Xác định file descriptor lớn nhất
        max_fd = sockfd;

        // Sử dụng select để chờ input từ người dùng hoặc phản hồi từ server
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (activity < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Nếu có dữ liệu từ bàn phím (input từ người dùng)
        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            fgets(buffer, sizeof(buffer), stdin);
            send(sockfd, buffer, strlen(buffer), 0);
        }

        // Nếu có phản hồi từ server
        if (FD_ISSET(sockfd, &read_fds))
        {
            read(sockfd, buffer, BUFFER_SIZE);
            printf("Server: %s\n", buffer);
            // Nhận phản hồi từ server
            int response = read(sockfd, buffer, BUFFER_SIZE);
            if (response <= 0)
            {
                printf("Server disconnected.\n");
                break;
            }
            else
                printf("Server: %s\n", buffer);
        }
    }

    close(sockfd);
>>>>>>> 15cf158e34f2b38943ed086882fb2e03b0bbd531
    return 0;
}
