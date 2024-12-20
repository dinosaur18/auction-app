#ifndef ITEM_H
#define ITEM_H

#define MAX_LENGTH 20
#define MAX_ITEM 100
#define MAX_ITEM_IN_ROOM 10

typedef struct {
    int item_id;             // ID của vật phẩm
    char name[MAX_LENGTH];  // Tên vật phẩm
    int startingPrice;      // Giá khởi điểm
    int buyNowPrice;        // Giá bán ngay
    int auctionTime;        // Thời gian đấu giá (giây)
    int room_id;             // ID phòng chứa vật phẩm
} Item;

// Hàm khai báo
int saveItem(Item item);
int getItemById(int item_id, Item *item);
int loadItems(int room_id, Item *items);
int deleteItem(int item_id);

#endif
