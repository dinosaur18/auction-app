#ifndef ROOM_H
#define ROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOM_NAME_LENGTH 100
#define MAX_USERS_IN_ROOM 10
#define MAX_ITEMS_IN_ROOM 10

// Cấu trúc thông tin phòng đấu giá
typedef struct {
    int roomId;                    // ID của phòng
    char roomName[MAX_ROOM_NAME_LENGTH]; // Tên phòng đấu giá
    int numUsers;                  // Số người tham gia
    int numItems;                  // Số vật phẩm trong phòng
    char users[MAX_USERS_IN_ROOM][MAX_ROOM_NAME_LENGTH]; // Danh sách người dùng tham gia
    char items[MAX_ITEMS_IN_ROOM][MAX_ROOM_NAME_LENGTH]; // Danh sách vật phẩm trong phòng
    int userId;
} Room;

// Khai báo các hàm thao tác với phòng đấu giá
int saveRoom(Room room);
int checkRoomExists(int roomId);
int loadRooms(Room *rooms, int *numRooms);
int createRoom(const char *roomName);
int getNextRoomId();
int deleteRoom(int roomId);

#endif // ROOM_H
