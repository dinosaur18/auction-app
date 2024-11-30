#include <stdio.h>
#include <string.h>
#include "room.h"

// Hàm để lấy ID phòng tiếp theo (tăng dần)
int getNextRoomId() {
    int nextRoomId = 1;
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL) {
        return nextRoomId; // Nếu file chưa tồn tại, bắt đầu với ID 1
    }

    Room room;
    while (fread(&room, sizeof(Room), 1, file)) {
        if (room.roomId >= nextRoomId) {
            nextRoomId = room.roomId + 1; // Tăng ID phòng nếu cần
        }
    }
    fclose(file);
    return nextRoomId;
}

// Hàm tạo phòng đấu giá
int createRoom(const char *roomName) {
    int roomId = getNextRoomId();
    Room room;
    room.roomId = roomId;
    strncpy(room.roomName, roomName, MAX_ROOM_NAME_LENGTH);
    room.numUsers = 0; // Bắt đầu phòng đấu giá không có người dùng
    room.numItems = 0; // Bắt đầu phòng đấu giá không có vật phẩm

    return saveRoom(room);
}

// Hàm lưu phòng đấu giá vào tệp
int saveRoom(Room room) {
    FILE *file = fopen("data/rooms.dat", "ab");
    if (file == NULL) {
        return -1; // Lỗi khi mở file
    }

    fwrite(&room, sizeof(Room), 1, file);
    fclose(file);
    return 1; // Thành công
}

// Hàm kiểm tra phòng đấu giá đã tồn tại hay chưa
int checkRoomExists(int roomId) {
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL) {
        return 0; // Nếu không thể mở file, coi như không có phòng nào
    }

    Room room;
    while (fread(&room, sizeof(Room), 1, file)) {
        if (room.roomId == roomId) {
            fclose(file);
            return 1; // Phòng đã tồn tại
        }
    }
    fclose(file);
    return 0; // Phòng không tồn tại
}

// Hàm tải danh sách phòng đấu giá
int loadRooms(Room *rooms, int *numRooms) {
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL) {
        return 0; // Không có phòng nào
    }

    *numRooms = 0;
    while (fread(&rooms[*numRooms], sizeof(Room), 1, file)) {
        (*numRooms)++;
    }
}
// Hàm xóa phòng đấu giá
int deleteRoom(int roomId) {
    FILE *file = fopen("data/rooms.dat", "rb");
    if (file == NULL) {
        return 0; // Không thể mở file
    }

    Room rooms[100];
    int numRooms = 0;

    // Đọc tất cả phòng và lưu vào mảng trừ phòng cần xóa
    while (fread(&rooms[numRooms], sizeof(Room), 1, file)) {
        if (rooms[numRooms].roomId != roomId) {
            numRooms++; // Không thêm phòng cần xóa vào mảng
        }
    }
    fclose(file);

    // Nếu không tìm thấy phòng để xóa
    if (numRooms == 0) {
        return 0;
    }

    // Ghi lại các phòng còn lại vào file mới
    file = fopen("data/rooms.dat", "wb");
    if (file == NULL) {
        return 0; // Lỗi khi mở file để ghi lại
    }

    for (int i = 0; i < numRooms; i++) {
        fwrite(&rooms[i], sizeof(Room), 1, file);
    }


    fclose(file);
    return 1; // Thành công
}
