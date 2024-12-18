#ifndef ROOM_H
#define ROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOM_NAME_LENGTH 100
#define MAX_LENGTH 50

#define ROOM_DATA "data/rooms.dat"
#define MAX_ROOMS 100

// Cấu trúc thông tin phòng đấu giá
typedef struct {
    int roomId;                    // ID của phòng
    char roomName[MAX_ROOM_NAME_LENGTH]; // Tên phòng đấu giá
    int numUsers;                  // Số người tham gia
    int numItems;                  // Số vật phẩm trong phòng
    char username[MAX_LENGTH];
} Room;

// Khai báo các hàm thao tác với phòng đấu giá
int checkRoomExists(Room room);
int loadRooms(Room *rooms, const char *username);
int createRoom(const char *roomName, const char *username);
int getNextRoomId();
int deleteRoom(int roomId);

#endif // ROOM_H
