#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 20
#define MAX_ITEM 100
#define MAX_ITEM_IN_ROOM 10
#define MAX_LINE_LENGTH 1024

typedef struct {
    int item_id;             // ID của vật phẩm
    char item_name[MAX_LENGTH];  // Tên vật phẩm
    int startingPrice;      // Giá khởi điểm
    int buyNowPrice;        // Giá bán ngay
    int room_id;             // ID phòng chứa vật phẩm
    char status[MAX_LENGTH];       // Trạng thái
} Item;

int _replaceLineInFile(const char *fileName, int lineToReplace, const char *newLineContent) {
    FILE *file = fopen(fileName, "r+");
    if (file == NULL)
    {
        perror("Error when opening file");
        return -1;
    }

    char temp[MAX_LINE_LENGTH];
    int currentLine = 1;
    
    while (fgets(temp, MAX_LINE_LENGTH, file) != NULL)
    {
        if (currentLine == lineToReplace)
        {
            fseek(file, -strlen(temp), SEEK_CUR);
            fputs(newLineContent, file);
            
            break; 
        }
        currentLine++;
    }

    fclose(file);
    return 0;  // Success
}

int setItemStatus(int itemId, const char *status)
{
    Item item;
    int line = 0;

    FILE *file = fopen("data/items.txt", "r");
    if (file == NULL)
    {
        perror("Failed to open items.txt");
        return 0; // Lỗi khi mở file
    }
    
    while (fscanf(file, "%d %s %d %d %d %s", &item.item_id, item.item_name, &item.startingPrice, &item.buyNowPrice, &item.room_id, item.status) == 6)
    {
        line++;
        if (item.item_id == itemId && strcmp(item.status, "Available") == 0)
        {
            char newInfo[1024];
            sprintf(newInfo, "%d %s %d %d %d %s\n", item.item_id, item.item_name, item.startingPrice, item.buyNowPrice, item.room_id, status);
            _replaceLineInFile("data/items.txt", line, newInfo);
            
            return 1;
        }
        fgetc(file);
    }
    return -1;
}

int main()
{
    if (setItemStatus(1, "Sold_Out_") < 0)
    {
        perror("Error occured when updating item status");
    }
    return 0;
}