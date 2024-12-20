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
int getItemById(int item_id, Item *item) {
    FILE *file = fopen("data/items.dat", "rb");
    if (file == NULL) {
        return -1;
    }

    while (fread(item, sizeof(Item), 1, file)) {
        if (item->item_id == item_id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0; // Không tìm thấy
}

// Lấy các vật phẩm trong phòng đấu giá
int loadItems(int room_id, Item *items) {
    FILE *file = fopen("data/items.dat", "rb");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    Item existedItem;
        while (fread(&existedItem, sizeof(Item), 1, file))
        {
            if (existedItem.room_id == room_id)
            {
                memcpy(&items[count], &existedItem, sizeof(Item));
                count++;
            }
        }

    fclose(file);
    return count;
}

// Xóa vật phẩm theo ID
int deleteItem(int item_id) {
    FILE *file = fopen("data/items.dat", "rb");
    if (file == NULL) {
        return -1;
    }

    // Đọc tất cả vật phẩm vào bộ nhớ
    Item items[100]; 
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
        if (items[i].item_id != item_id) {
            fwrite(&items[i], sizeof(Item), 1, file);
        }
    }

    fclose(file);
    return 1; // Xóa thành công
}
