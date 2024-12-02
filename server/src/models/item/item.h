#ifndef ITEM_H
#define ITEM_H

#define MAX_LENGTH_ITEM 100

typedef struct {
    int itemId;             // ID của vật phẩm
    char name[MAX_LENGTH_ITEM];  // Tên vật phẩm
    int startingPrice;      // Giá khởi điểm
    int buyNowPrice;        // Giá bán ngay
    int auctionTime;        // Thời gian đấu giá (giây)
    int roomId;             // ID phòng chứa vật phẩm
} Item;

// Hàm khai báo
int saveItem(Item item);
int getItemById(int itemId, Item *item);
int listItems(int roomId, Item *items, int maxItems);
int deleteItem(int itemId);

#endif
