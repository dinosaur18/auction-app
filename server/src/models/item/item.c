#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "item.h"

#define ITEM_FILE_PATH "data/items.dat"

// Hàm để lấy ID vật phẩm tiếp theo trong 1 room
int getNextItemIdInRoom(int room_id)
{
    int next_item_id = 1;  // ID mặc định cho vật phẩm đầu tiên trong phòng
    FILE *file = fopen("data/items.dat", "rb");
    if (file == NULL)
    {
        return next_item_id; // Nếu file chưa tồn tại, bắt đầu với ID 1
    }

    Item item;
    while (fread(&item, sizeof(Item), 1, file))
    {
        if (item.room_id == room_id)  // Kiểm tra vật phẩm có thuộc phòng này không
        {
            if (item.item_id >= next_item_id)
            {
                next_item_id = item.item_id + 1; // Tăng ID nếu vật phẩm hiện tại có ID lớn hơn hoặc bằng next_item_id
            }
        }
    }
    fclose(file);
    return next_item_id;
}

// Hàm tạo và lưu một vật phẩm mới
int createItem(int item_id, const char *name, int startingPrice, int buyNowPrice, int auctionTime, int room_id) {
    // Khởi tạo vật phẩm
    Item item;
    item.item_id = item_id;
    strncpy(item.name, name, MAX_LENGTH - 1);
    item.name[MAX_LENGTH - 1] = '\0'; // Đảm bảo chuỗi kết thúc
    item.startingPrice = startingPrice;
    item.buyNowPrice = buyNowPrice;
    item.auctionTime = auctionTime;
    item.room_id = room_id;

    // Lưu vật phẩm vào file
    FILE *file = fopen(ITEM_FILE_PATH, "ab");
    if (file == NULL) {
        perror("Error opening file to save item");
        return -1;
    }

    fwrite(&item, sizeof(Item), 1, file);
    fclose(file);
    return 1;
}

// Xóa vật phẩm theo ID
int deleteItem(int item_id) {
    FILE *file = fopen(ITEM_FILE_PATH, "rb");
    if (file == NULL) {
        perror("Error opening file to delete item");
        return -1;
    }

    Item items[MAX_ITEM];
    int count = 0;

    while (fread(&items[count], sizeof(Item), 1, file)) {
        count++;
    }

    fclose(file);

    file = fopen(ITEM_FILE_PATH, "wb");
    if (file == NULL) {
        perror("Error reopening file to rewrite items");
        return -1;
    }

    int deleted = 0;
    for (int i = 0; i < count; i++) {
        if (items[i].item_id != item_id) {
            fwrite(&items[i], sizeof(Item), 1, file);
        } else {
            deleted = 1;
        }
    }

    fclose(file);
    return deleted ? 1 : 0; // Trả về 1 nếu xóa thành công, 0 nếu không tìm thấy
}

// Hàm khởi tạo file nếu chưa tồn tại
void initItemFile() {
    FILE *file = fopen(ITEM_FILE_PATH, "rb");
    if (file == NULL) {
        file = fopen(ITEM_FILE_PATH, "wb");
        if (file) {
            fclose(file);
            printf("Initialized item file at %s\n", ITEM_FILE_PATH);
        } else {
            perror("Error initializing item file");
        }
    } else {
        fclose(file);
    }
}

// Lấy các vật phẩm trong một phòng đấu giá
int loadItems(int room_id, Item *items) {
    FILE *file = fopen(ITEM_FILE_PATH, "rb");
    if (file == NULL) {
        perror("Error opening file to load items");
        return 0;
    }

    int count = 0;
    Item existedItem;

    while (fread(&existedItem, sizeof(Item), 1, file)) {
        if (existedItem.room_id == room_id) {
            memcpy(&items[count], &existedItem, sizeof(Item));
            count++;

            // Đảm bảo không vượt quá giới hạn
            if (count >= MAX_ITEM_IN_ROOM) {
                break;
            }
        }
    }

    fclose(file);
    return count; // Trả về số lượng item tìm thấy
}

// Lấy thông tin một vật phẩm theo ID
int getItemById(int item_id, Item *item) {
    FILE *file = fopen(ITEM_FILE_PATH, "rb");
    if (file == NULL) {
        perror("Error opening file to get item by ID");
        return -1;
    }

    while (fread(item, sizeof(Item), 1, file)) {
        if (item->item_id == item_id) {
            fclose(file);
            return 1; // Thành công
        }
    }

    fclose(file);
    return 0; // Không tìm thấy
}