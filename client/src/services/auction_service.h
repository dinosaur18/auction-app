#ifndef AUCTION_SERVICE_H
#define AUCTION_SERVICE_H

#include <stdbool.h>
#include "room.h"
#include "item.h"

int handle_create_room(int sockfd, const char *room_name);
int handle_delete_room(int sockfd, int roomId);
int handle_create_item(int sockfd, int roomId, const char *name, int startingPrice, int buyNowPrice, int auctionTime);
int handle_delete_item(int sockfd, int itemId);
int handle_fetch_all_rooms(int sockfd, Room *rooms);
int handle_fetch_own_rooms(int sockfd, Room *rooms);
int handle_fetch_bidding_items(int sockfd, Item *items);
int handle_fetch_items(int sockfd, int room_id, Item *items);
int handle_join_room(int sockfd, int room_id);

#endif 
