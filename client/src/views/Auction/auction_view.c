#include <gtk/gtk.h>
#include "style_manager.h"
#include "auction_view.h"
#include "auction_service.h"

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

void init_auction_view(int sockfd, GtkWidget *home_window)
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

    AuctionContext *auctionContext = g_malloc(sizeof(AuctionContext));
    auctionContext->sockfd = sockfd;
    // auctionContext->create_item_form = GTK_WIDGET(gtk_builder_get_object(builder, "add_item_form"));
    // auctionContext->room_name = GTK_WIDGET(gtk_builder_get_object(builder, "room_name"));
    // auctionContext->create_room_msg = GTK_WIDGET(gtk_builder_get_object(builder, "create_room_msg"));

    gtk_builder_connect_signals(builder, auctionContext);

    apply_css();
    gtk_widget_show_all(window);

    g_object_unref(builder);
}
