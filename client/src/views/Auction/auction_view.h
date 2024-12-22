#ifndef AUCTION_VIEW_H
#define AUCTION_VIEW_H
#include <gtk/gtk.h>
#include "room.h"
#include "item.h"
#include "home_view.h"

// Khai báo hàm hoặc cấu trúc
void init_auction_view(int sockfd, AppContext *app_context, Room room, int role);

#endif