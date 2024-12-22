#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "item.h"

// #define "data/items.txt" "data/items.txt"

// Hàm để lấy ID vật phẩm tiếp theo trong 1 room
int getNextItemIdInRoom(int room_id)
{
    int next_item_id = 1;  // ID mặc định cho vật phẩm đầu tiên trong phòng
    FILE *file = fopen("data/items.txt", "rb");
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
    FILE *file = fopen("data/items.txt", "ab");
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
    FILE *file = fopen("data/items.txt", "rb");
    if (file == NULL) {
        perror("Error opening file to delete item");
        return -1; // Trả về lỗi nếu không mở được file
    }

    Item items[MAX_ITEM];
    int count = 0;

    // Đọc tất cả các item vào mảng
    while (fread(&items[count], sizeof(Item), 1, file)) {
        count++;
    }

    // Kiểm tra nếu không đọc được dữ liệu từ file
    if (ferror(file)) {
        perror("Error reading file");
        fclose(file); // Đảm bảo đóng file khi có lỗi
        return -1;
    }

    fclose(file); // Đóng file sau khi đọc


    file = fopen("data/items.txt", "wb");
    if (file == NULL) {
        perror("Error reopening file to rewrite items");
        return -1;
    }

    int deleted = 0;
    for (int i = 0; i < count; i++) {
        if (items[i].item_id != item_id) {
            if (fwrite(&items[i], sizeof(Item), 1, file) != 1) {
                perror("Error writing item to file");
                fclose(file); 
                return -1; 
            }
        } else {
            deleted = 1;  // Đánh dấu đã xóa thành công
        }
    }

    // Đảm bảo đóng file sau khi ghi
    if (fclose(file) != 0) {
        perror("Error closing file");
        return -1; // Lỗi khi đóng file
    }

    // debug
    printf("Deleted: %d\n", deleted);

    return deleted ? 1 : 0;
}


// Hàm khởi tạo file nếu chưa tồn tại
void initItemFile() {
    FILE *file = fopen("data/items.txt", "rb");
    if (file == NULL) {
        file = fopen("data/items.txt", "wb");
        if (file) {
            fclose(file);
            printf("Initialized item file at %s\n","data/items.txt");
        } else {
            perror("Error initializing item file");
        }
    } else {
        fclose(file);
    }
}

// Lấy các vật phẩm trong một phòng đấu giá
int loadItems(int room_id, Item *items) {
    FILE *file = fopen("data/items.txt", "rb");
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

            // debug
            printf("[LOAD] item id: %d\n", items[count].item_id);

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
    FILE *file = fopen("data/items.txt", "rb");
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