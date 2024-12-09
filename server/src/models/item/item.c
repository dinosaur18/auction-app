#include <stdio.h>
#include <string.h>
#include "item.h"

// Lưu vật phẩm vào file
int saveItem(Item item) {
    FILE *file = fopen("data/items.dat", "ab");
    if (file == NULL) {
        return -1;
    }

    fwrite(&item, sizeof(Item), 1, file);
    fclose(file);
    return 1;
}

// Lấy thông tin một vật phẩm theo ID
int getItemById(int itemId, Item *item) {
    FILE *file = fopen("data/items.dat", "rb");
    if (file == NULL) {
        return -1;
    }

    while (fread(item, sizeof(Item), 1, file)) {
        if (item->itemId == itemId) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0; // Không tìm thấy
}

// Liệt kê các vật phẩm trong phòng đấu giá
int listItems(int roomId, Item *items, int maxItems) {
    FILE *file = fopen("data/items.dat", "rb");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (fread(&items[count], sizeof(Item), 1, file)) {
        // Giả sử Item có trường roomId để phân loại vật phẩm trong các phòng đấu giá
        if (items[count].roomId == roomId) {
            count++;
            if (count >= maxItems) {
                break;
            }
        }
    }

    fclose(file);
    return count;
}

// Xóa vật phẩm theo ID
int deleteItem(int itemId) {
    FILE *file = fopen("data/items.dat", "rb");
    if (file == NULL) {
        return -1;
    }

    // Đọc tất cả vật phẩm vào bộ nhớ
    Item items[100]; // Giới hạn 100 vật phẩm
    int count = 0;
    while (fread(&items[count], sizeof(Item), 1, file)) {
        count++;
    }

    fclose(file);

    // Mở lại file để ghi lại các vật phẩm còn lại
    file = fopen("data/items.dat", "wb");
    if (file == NULL) {
        return -1;
    }

    // Ghi lại tất cả vật phẩm trừ vật phẩm cần xóa
    for (int i = 0; i < count; i++) {
        if (items[i].itemId != itemId) {
            fwrite(&items[i], sizeof(Item), 1, file);
        }
    }

    fclose(file);
    return 1; // Xóa thành công
}
