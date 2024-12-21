#include <gtk/gtk.h>
#include "style_manager.h"
#include "auction_view.h"
#include "auction_service.h"
#include "room.h"
#include "item.h"

int remaining_time = 10;     // Đếm ngược 10 giây trước khi bắt đầu phiên đấu giá
gboolean is_running = FALSE; // Trạng thái đồng hồ

typedef struct
{
    int sockfd;
    GtkStack *auction_stack;
    GtkStack *label_waiting;
    GtkStack *label_countdown;

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

////////////////// AUCTION BOX //////////////////

// Hàm cập nhật thời gian trên nhãn
gboolean update_countdown(gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;
    if (remaining_time <= 0)
    {
        gtk_stack_set_visible_child_name(context->auction_stack, "opening");
        is_running = FALSE; // Kết thúc đồng hồ
        return FALSE;       // Dừng việc gọi lại hàm này
    }

    // Cập nhật thời gian
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", remaining_time);
    gtk_label_set_text(GTK_LABEL(context->label_countdown), buffer);

    // Giảm thời gian
    remaining_time--;
    return TRUE; // Tiếp tục gọi lại hàm sau 1 giây
}

void on_btn_start_clicked(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    gtk_label_set_text(GTK_LABEL(context->label_waiting), "Phiên đấu giá sẽ diên ra sau");
    if (!is_running)
    {
        is_running = TRUE;
        remaining_time = 10; 
        g_timeout_add(1000, update_countdown, context); // Gọi lại hàm mỗi giây
    }
}

////////////////// ////////////////// //////////////////

void init_auction_view(int sockfd, GtkWidget *home_window, Room room, int role)
{
    AuctionContext *auctionContext = g_malloc(sizeof(AuctionContext));
    auctionContext->sockfd = sockfd;

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
    GtkWidget *start_button = GTK_WIDGET(gtk_builder_get_object(builder, "start_button"));
    auctionContext->auction_stack = GTK_STACK(gtk_builder_get_object(builder, "auction_stack"));
    auctionContext->label_waiting = GTK_STACK(gtk_builder_get_object(builder, "label_waiting"));
    auctionContext->label_countdown = GTK_STACK(gtk_builder_get_object(builder, "label_countdown"));

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

    gtk_builder_connect_signals(builder, auctionContext);

    apply_css();
    gtk_widget_show_all(window);
    if (role == 2)
    {
        gtk_widget_hide(add_button);
        gtk_widget_hide(start_button);
    }

    g_object_unref(builder);
}
