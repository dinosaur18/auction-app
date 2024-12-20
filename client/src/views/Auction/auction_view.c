#include <gtk/gtk.h>
#include "style_manager.h"
#include "auction_view.h"
#include "auction_service.h"
#include "room.h"
#include "item.h"

typedef struct
{
    int sockfd;

    GtkWidget *create_room_form;
    GtkWidget *room_name;
    GtkWidget *create_room_msg;
} AuctionContext;

void on_auction_window_destroy(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *home_window = (GtkWidget *)user_data;

    // Hiển thị lại cửa sổ đăng nhập khi cửa sổ Home bị đóng
    gtk_widget_show(home_window);
}



////////////////// AUCTION PAGE //////////////////

////////////////// ////////////////// //////////////////

void init_auction_view(int sockfd, GtkWidget *home_window, Room room, int role)
{
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "client/src/views/Auction/auction_view.glade", &error))
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_auction"));
    g_signal_connect(window, "destroy", G_CALLBACK(on_auction_window_destroy), home_window);

    GtkWidget *label_room_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_name"));
    GtkWidget *label_room_owner = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_owner"));
    GtkWidget *label_room_joiner = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_joiner"));
    GtkWidget *add_button = GTK_WIDGET(gtk_builder_get_object(builder, "add_button"));

    if (GTK_IS_LABEL(label_room_name))
    {
        gtk_label_set_text(GTK_LABEL(label_room_name), room.roomName);
    }
    if (GTK_IS_LABEL(label_room_owner))
    {
        gtk_label_set_text(GTK_LABEL(label_room_owner), room.username);
    }
    if (GTK_IS_LABEL(label_room_joiner))
    {
        char joiner_count_text[32];
        snprintf(joiner_count_text, sizeof(joiner_count_text), "%d", room.numUsers);
        gtk_label_set_text(GTK_LABEL(label_room_joiner), joiner_count_text);
    }
    if (role == 2) {
        gtk_widget_hide(add_button);
    }


    AuctionContext *auctionContext = g_malloc(sizeof(AuctionContext));
    auctionContext->sockfd = sockfd;

    gtk_builder_connect_signals(builder, auctionContext);

    apply_css();
    gtk_widget_show_all(window);

    g_object_unref(builder);
}
