#include <stdio.h>
#include <string.h>
#include "room.h"

// Hàm để lấy ID phòng tiếp theo (tăng dần)
int getNextRoomId()
{
    int nextRoomId = 1;
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL)
    {
        return nextRoomId; // Nếu file chưa tồn tại, bắt đầu với ID 1
    }

    Room room;
    while (fread(&room, sizeof(Room), 1, file))
    {
        if (room.roomId >= nextRoomId)
        {
            nextRoomId = room.roomId + 1; // Tăng ID phòng nếu cần
        }
    }
    fclose(file);
    return nextRoomId;
}

// Hàm tạo phòng đấu giá
int createRoom(const char *roomName, const char *username)
{
    int roomId = getNextRoomId();
    Room room;
    room.roomId = roomId;
    strncpy(room.roomName, roomName, MAX_ROOM_NAME_LENGTH);
    strncpy(room.username, username, MAX_LENGTH);
    room.numUsers = 0; // Bắt đầu phòng đấu giá không có người dùng
    room.numItems = 0; // Bắt đầu phòng đấu giá không có vật phẩm

    if (checkRoomExists(room))
    {
        return 0;
    }

    FILE *file = fopen("data/rooms.dat", "ab");
    if (file == NULL)
    {
        return 0; // Lỗi khi mở file
    }

    fwrite(&room, sizeof(Room), 1, file);
    fclose(file);
    return 1; // Thành công
}

// Hàm kiểm tra phòng đấu giá đã tồn tại hay chưa
int checkRoomExists(Room room)
{
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL)
    {
        return 0; // Nếu không thể mở file, coi như không có phòng nào
    }

    Room existedRoom;
    while (fread(&existedRoom, sizeof(Room), 1, file))
    {
        if (strcmp(existedRoom.username, room.username) == 0 && strcmp(existedRoom.roomName, room.roomName) == 0)
        {
            fclose(file);
            return room.roomId;
        }
    }

    fclose(file);
    return 0; // Phòng không tồn tại
}

// Hàm tải danh sách phòng đấu giá
int loadRooms(Room *rooms, const char *username)
{
    FILE *file = fopen(ROOM_DATA, "rb");
    if (file == NULL)
    {
        perror("Error opening room file");
        return -1;
    }

    int room_count = 0;

    if (username != NULL)
    {
        Room existedRoom;
        while (fread(&existedRoom, sizeof(Room), 1, file))
        {
            if (strcmp(existedRoom.username, username) == 0)
            {
                memcpy(&rooms[room_count], &existedRoom, sizeof(Room));
                room_count++;
            }
        }
    }
    else
    {

        // Đọc dữ liệu từ file
        while (fread(&rooms[room_count], sizeof(Room), 1, file) == 1)
        {
            room_count++;
        }
    }

    fclose(file);
    return room_count;
}
// Hàm xóa phòng đấu giá
int deleteRoom(int roomId)
{
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL)
    {
        return 0; // Không thể mở file
    }

    Room rooms[100];
    int numRooms = 0;

    // Đọc tất cả phòng và lưu vào mảng trừ phòng cần xóa
    while (fread(&rooms[numRooms], sizeof(Room), 1, file))
    {
        if (rooms[numRooms].roomId != roomId)
        {
            numRooms++; // Không thêm phòng cần xóa vào mảng
        }
    }
    fclose(file);

    // Nếu không tìm thấy phòng để xóa
    if (numRooms == 0)
    {
        return 0;
    }

    // Ghi lại các phòng còn lại vào file mới
    file = fopen("data/rooms.dat", "wb");
    if (file == NULL)
    {
        return 0; // Lỗi khi mở file để ghi lại
    }

    for (int i = 0; i < numRooms; i++)
    {
        fwrite(&rooms[i], sizeof(Room), 1, file);
    }

    fclose(file);
    return 1; // Thành công
}
