#ifndef HOME_VIEW_H
#define HOME_VIEW_H
#include <gtk/gtk.h>

#include "user.h"

typedef struct
{
    int sockfd;

    GtkWidget *home_window;
    GtkWidget *dashboard;
    GtkWidget *room_item;
    GtkFlowBox *room_list_all;

    GtkWidget *my_auction;
    GtkWidget *create_room_form;
    GtkWidget *room_name;
    GtkWidget *create_room_msg;
    GtkFlowBox *room_list;
    GtkWidget *room_card;
} AppContext;

// Khai báo hàm hoặc cấu trúc
void init_home_view(int sockfd, GtkWidget *auth_window, User user);
void reload_home_view(gpointer user_data);

#endif