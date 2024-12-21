#include <gtk/gtk.h>
#include "style_manager.h"
#include "auction_view.h"
#include "auction_service.h"
#include "room.h"
#include "item.h"

typedef struct
{
    int sockfd;
    int room_id;

    GtkWidget *add_item_form;
    GtkWidget *item_name;
    GtkWidget *item_starting_price;
    GtkWidget *item_buynow_price;
    GtkWidget *item_auction_time;

    GtkWidget *add_item_msg;
    GtkWidget *item_card;
    GtkListBox *item_list;

} AuctionContext;

void on_auction_window_destroy(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *home_window = (GtkWidget *)user_data;

    // Hiển thị lại cửa sổ đăng nhập khi cửa sổ Home bị đóng
    gtk_widget_show(home_window);
}

void clear_item_children(GtkListBox *listbox)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(listbox));
    for (GList *iter = children; iter != NULL; iter = iter->next)
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

////////////////// AUCTION PAGE //////////////////
GtkWidget *add_item_card(Item item, gpointer user_data)
{
    // AuctionContext *context = (AuctionContext *)user_data;

    GtkBuilder *builder;
    GtkWidget *card;
    GError *error = NULL;

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "client/src/views/Home/custom_card.glade", &error))
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return NULL;
    }

    gtk_builder_connect_signals(builder, NULL);

    card = GTK_WIDGET(gtk_builder_get_object(builder, "item_card"));

    GtkWidget *parent = gtk_widget_get_parent(card);
    if (parent != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(parent), card);
    }

    // Tìm và cập nhật nội dung trong card
    GtkWidget *item_name = GTK_WIDGET(gtk_builder_get_object(builder, "item_name"));

    if (GTK_IS_LABEL(item_name))
    {
        g_print("abc");
        gtk_label_set_text(GTK_LABEL(item_name), item.name);
    }

    return card;
}

////////////////// LIST ITEM /////////////////
void fetch_item(gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    // Tạo mảng Item để lưu danh sách item nhận được
    Item items[MAX_ITEM];
    int count = handle_fetch_items(context->sockfd,context-> room_id, items);

    if (count < 0)
    {
        g_printerr("Failed to fetch item list from server\n");
        return;
    }

    // Duyệt qua danh sách item và thêm vào GTK_LIST_BOX
    for (int i = 0; i < count; i++)
    {
        g_print ("abc");
        GtkWidget *item_card = add_item_card(items[i], context); 
        gtk_list_box_insert(context->item_list, item_card, -1);
    }

    // Hiển thị các widget vừa thêm
    gtk_widget_show_all(GTK_WIDGET(context->item_list));
}

void show_add_item_form(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->add_item_form);
    gtk_widget_show(form);
}


///BUTTON
void on_add_item_ok(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->add_item_form);
    const gchar *item_name = gtk_entry_get_text(GTK_ENTRY(context->item_name));
    const gchar *item_starting_price_str = gtk_entry_get_text(GTK_ENTRY(context->item_starting_price));
    const gchar *item_buynow_price_str = gtk_entry_get_text(GTK_ENTRY(context->item_buynow_price));
    const gchar *item_auction_time_str = gtk_entry_get_text(GTK_ENTRY(context->item_auction_time));
    g_print("Item Name: %s\n", item_name); // In ra tên vật phẩm
    g_print("Starting Price: %s\n", item_starting_price_str);
    g_print("Buy Now Price: %s\n", item_buynow_price_str);
    g_print("Auction Time: %s\n", item_auction_time_str);
    int item_buynow_price = atoi(item_buynow_price_str);
    int item_starting_price = atoi(item_starting_price_str);
    int item_auction_time = atoi(item_auction_time_str);
    int itemId = handle_create_item(context->sockfd, context->room_id, item_name, item_starting_price, item_buynow_price, item_auction_time);
    if (itemId > 0)
    {
        gtk_widget_hide(form);
        clear_item_children(GTK_LIST_BOX(context->item_list));
        fetch_item(context);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(context->add_item_msg), "This item's name already exists.");
    }

}


void on_add_item_cancel(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->add_item_form);
    gtk_widget_hide(form);
}

////////////////// ////////////////// //////////////////

void init_auction_view(int sockfd, GtkWidget *home_window, Room room, Item item, int role)
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



    AuctionContext *auctionContext = g_malloc(sizeof(AuctionContext));
    auctionContext->sockfd = sockfd;
    auctionContext->room_id = room.room_id;

    auctionContext->item_list = GTK_LIST_BOX(gtk_builder_get_object(builder, "item_list"));
    auctionContext->item_name = GTK_WIDGET(gtk_builder_get_object(builder, "item_name"));
    auctionContext->item_starting_price = GTK_WIDGET(gtk_builder_get_object(builder, "item_starting_price"));
    auctionContext->item_buynow_price = GTK_WIDGET(gtk_builder_get_object(builder, "item_buynow_price"));
    auctionContext->item_auction_time = GTK_WIDGET(gtk_builder_get_object(builder, "item_auction_time"));
    auctionContext->item_card = GTK_WIDGET(gtk_builder_get_object(builder, "item_card"));
    auctionContext->add_item_form = GTK_WIDGET(gtk_builder_get_object(builder, "add_item_form"));
    auctionContext->add_item_msg = GTK_WIDGET(gtk_builder_get_object(builder, "add_item_msg"));



    gtk_builder_connect_signals(builder, auctionContext);

    apply_css();
    gtk_widget_show_all(window);
    if (role == 2) {
        gtk_widget_hide(add_button);
    }
    fetch_item(auctionContext);

    g_object_unref(builder);
}





