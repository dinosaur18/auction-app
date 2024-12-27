#include <gtk/gtk.h>
#include <sys/socket.h>
#include "style_manager.h"
#include "auction_view.h"
#include "auction_service.h"
#include "message_type.h"
#include "room.h"
#include "item.h"

int remaining_time = 10;
gboolean is_running = FALSE; // Trạng thái đồng hồ
gboolean auction_running = FALSE;

typedef struct
{
    int sockfd;
    int role;
    int room_id;
    int item_id;
    GtkWidget *home_window;
    GtkWidget *label_room_joiner;
    GtkWidget *auction_window;
    GtkStack *auction_stack;
    GtkWidget *label_waiting;
    GtkWidget *label_countdown;

    double current_item_startPrice;
    GtkWidget *current_user;
    GtkWidget *highest_price;
    GtkWidget *auction_item_name;
    GtkWidget *auction_time;
    GtkWidget *start_price;
    GtkWidget *current_price;
    GtkWidget *btn_bid1;
    GtkWidget *btn_bid2;
    GtkWidget *btn_bid3;
    GtkWidget *btn_bid4;

    GtkWidget *add_item_form;
    GtkWidget *item_name;
    GtkWidget *item_starting_price;
    GtkWidget *item_buynow_price;
    GtkWidget *item_auction_time;

    GtkWidget *add_item_msg;
    GtkWidget *item_card;
    GtkListBox *item_list;

} AuctionContext;

typedef struct
{
    int sockfd;
    int room_id;
    Item item;
    GtkLabel *status_label;
    AuctionContext *auctionContext;
} ItemContext;

void on_auction_window_destroy(GtkWidget *widget, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    if (handle_exit_room(context->sockfd, context->room_id))
    {
        gtk_widget_show(context->home_window);
    }
}

void on_buy_button_clicked(GtkWidget *widget, gpointer user_data)
{
    printf("Buy button clicked\n");
    ItemContext *context = (ItemContext *)user_data;

    printf("Item id: %d\n", context->item.item_id);

    if (handle_buy_now(context->sockfd, context->room_id, context->item.item_id) < 0)
    {
        printf("[BUYNOW] Cannot buy\n");
        return;
    }

    gtk_label_set_text(context->status_label, "Sold");
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

GtkWidget *add_item_card(Item item, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

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
    ItemContext *itemContext = g_malloc(sizeof(ItemContext));
    itemContext->sockfd = context->sockfd;
    itemContext->room_id = context->room_id;
    itemContext->item = item;
    itemContext->auctionContext = context;

    // gtk_builder_connect_signals(builder, &item);
    gtk_builder_connect_signals(builder, itemContext);

    card = GTK_WIDGET(gtk_builder_get_object(builder, "item_card"));

    GtkWidget *parent = gtk_widget_get_parent(card);
    if (parent != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(parent), card);
    }

    // Tìm và cập nhật nội dung trong card
    GtkWidget *item_name = GTK_WIDGET(gtk_builder_get_object(builder, "item_name"));
    GtkWidget *status = GTK_WIDGET(gtk_builder_get_object(builder, "status"));
    GtkWidget *buy_button = GTK_WIDGET(gtk_builder_get_object(builder, "buy_button"));
    GtkWidget *delete_button = GTK_WIDGET(gtk_builder_get_object(builder, "delete_button"));

    itemContext->status_label = (GtkLabel *)status;

    g_signal_connect(buy_button, "clicked", G_CALLBACK(on_buy_button_clicked), itemContext);

    if (GTK_IS_LABEL(item_name))
    {
        gtk_label_set_text(GTK_LABEL(item_name), item.item_name);
    }

    if (GTK_IS_LABEL(status))
    {
        char _item_status[64];
        if (strcmp(item.status, "Sold_Out_") == 0)
        {
            sprintf(_item_status, "Sold");
        }
        else
        {
            sprintf(_item_status, "%s", item.status);
        }

        gtk_label_set_text(GTK_LABEL(status), _item_status);
    }

    gtk_widget_show_all(card);

    if (context->role == 2)
    {
        gtk_widget_hide(delete_button);
    }
    if (context->role == 1)
    {
        gtk_widget_hide(buy_button);
    }

    return card;
}

void fetch_item(gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    // Tạo mảng Item để lưu danh sách item nhận được
    Item items[MAX_ITEM_IN_ROOM];
    int count = handle_fetch_items(context->sockfd, context->room_id, items);

    if (count < 0)
    {
        g_printerr("Failed to fetch item list from server\n");
        return;
    }

    // Duyệt qua danh sách item và thêm vào GTK_LIST_BOX
    for (int i = 0; i < count; i++)
    {
        GtkWidget *item_card = add_item_card(items[i], context);
        gtk_list_box_insert(context->item_list, item_card, -1);
    }

    // Hiển thị các widget vừa thêm
    // gtk_widget_show_all(GTK_WIDGET(context->item_list));
}

void on_delete_item(GtkWidget *button, gpointer user_data)
{
    // Item *item = (Item *)user_data;
    ItemContext *context = (ItemContext *)user_data;
    g_print("Delete button clicked\n");

    int result = handle_delete_item(context->sockfd, context->item.item_id);
    if (result > 0)
    {
        // Nếu xóa thành công, cập nhật lại danh sách item
        clear_item_children(GTK_LIST_BOX(context->auctionContext->item_list));
        fetch_item(context->auctionContext);
    }
}

////////////////// LIST ITEM /////////////////
void show_add_item_form(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->add_item_form);
    gtk_widget_show(form);
}

/// BUTTON
void on_add_item_ok(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->add_item_form);
    const gchar *item_name = gtk_entry_get_text(GTK_ENTRY(context->item_name));
    const gchar *item_starting_price_str = gtk_entry_get_text(GTK_ENTRY(context->item_starting_price));
    const gchar *item_buynow_price_str = gtk_entry_get_text(GTK_ENTRY(context->item_buynow_price));
    int item_buynow_price = atoi(item_buynow_price_str);
    int item_starting_price = atoi(item_starting_price_str);
    int item_id = handle_create_item(context->sockfd, item_name, item_starting_price, item_buynow_price, context->room_id);
    printf("%d\n", item_id);
    if (item_id > 0)
    {
        context->item_id = item_id;
        gtk_widget_hide(form);
        clear_item_children(GTK_LIST_BOX(context->item_list));
        fetch_item(context);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(context->add_item_msg), "Error");
    }
}

void on_add_item_cancel(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->add_item_form);
    gtk_widget_hide(form);
}

////////////////// AUCTION //////////////////

gboolean waiting_countdown(gpointer user_data)
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

gboolean auction_countdown(gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;
    if (remaining_time <= 0)
    {
        gtk_stack_set_visible_child_name(context->auction_stack, "waiting");
        auction_running = FALSE; // Kết thúc 1 lượt đấu giá
        return FALSE;            // Dừng việc gọi lại hàm này
    }

    // Cập nhật thời gian
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", remaining_time);
    gtk_label_set_text(GTK_LABEL(context->auction_time), buffer);

    // Giảm thời gian
    remaining_time--;
    return TRUE; // Tiếp tục gọi lại hàm sau 1 giây
}

void on_btn_start_clicked(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    if (handle_start_auction(context->sockfd, context->room_id))
    {
        gtk_label_set_text(GTK_LABEL(context->label_waiting), "Phiên đấu giá sẽ diên ra sau");
        if (!is_running)
        {
            is_running = TRUE;
            remaining_time = 10;
            g_timeout_add(1000, waiting_countdown, context); // Gọi lại hàm mỗi giây
        }
    }
}

// Hàm tính toán bước giá
void calculate_bid_steps(double item_price, double *step1, double *step2, double *step3, double *step4)
{
    double step = item_price * 0.02; // 2% của giá vật phẩm
    *step1 = step;
    *step2 = step * 2;
    *step3 = step * 3;
    *step4 = step * 4;
}

void update_bid_steps(double item_price, AuctionContext *context)
{
    double step1, step2, step3, step4;
    calculate_bid_steps(item_price, &step1, &step2, &step3, &step4);

    char label1[50], label2[50], label3[50], label4[50];
    sprintf(label1, "%.2f", step1);
    sprintf(label2, "%.2f", step2);
    sprintf(label3, "%.2f", step3);
    sprintf(label4, "%.2f", step4);

    gtk_button_set_label(GTK_BUTTON(context->btn_bid1), label1);
    gtk_button_set_label(GTK_BUTTON(context->btn_bid2), label2);
    gtk_button_set_label(GTK_BUTTON(context->btn_bid3), label3);
    gtk_button_set_label(GTK_BUTTON(context->btn_bid4), label4);
}

// Xử lý sự kiện click các bước giá
void on_bid_1(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;
    printf("You selected bid: %.2f\n", context->current_item_startPrice * 0.02);
}

void on_bid_2(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;
    printf("You selected bid: %.2f\n", context->current_item_startPrice * 0.02 * 2);
}

void on_bid_3(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;
    printf("You selected bid: %.2f\n", context->current_item_startPrice * 0.02 * 3);
}

void on_bid_4(GtkWidget *button, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;
    printf("You selected bid: %.2f\n", context->current_item_startPrice * 0.02 * 4);
}

// Hàm xử lý tín hiệu bắt đầu đấu giá
void start_auction_process(gpointer user_data)
{
    printf("START_AUCTION received\n");

    AuctionContext *context = (AuctionContext *)user_data;
    Item items[MAX_ITEM_IN_ROOM];
    int item_count = handle_fetch_items(context->sockfd, context->room_id, items);

    printf("Check %d\n", context->room_id);
    printf("Item count: %d\n", item_count);

    if (item_count > 0)
    {
        gtk_label_set_text(GTK_LABEL(context->label_waiting), "Phiên đấu giá sẽ diễn ra sau");
        if (!is_running)
        {
            is_running = TRUE;
            remaining_time = 10;
            g_timeout_add(1000, waiting_countdown, context); // Gọi lại hàm mỗi giây
        }
    }
    else
    {
        printf("No items available for auction.\n");
        gtk_label_set_text(GTK_LABEL(context->label_waiting), "No items available for auction.");
        return;
    }

    for (int i = 0; i < item_count; i++)
    {
        Item *current_item = &items[i];
        context->current_item_startPrice = current_item->startingPrice;
        remaining_time = 30; // Mỗi vật phẩm đấu giá bắt đầu với 30 giây

        update_bid_steps(current_item->startingPrice, context);

        printf("Starting auction for item: %s\n", current_item->item_name);
        gtk_label_set_text(GTK_LABEL(context->auction_item_name), current_item->item_name);
        char label[32];
        snprintf(label, sizeof(label), "%d", current_item->startingPrice);
        gtk_label_set_text(GTK_LABEL(context->start_price), label);

        auction_running = TRUE;

        // Đếm ngược cho mỗi vật phẩm
        while (auction_running)
        {
            g_timeout_add(1000, (GSourceFunc)auction_countdown, context);
            // bug: the auction_countdown function is never called so the remaining_time value is consistently 30

            // Giả lập xử lý sự kiện ra giá từ client
            gboolean bid_received = FALSE; // Hàm này nên kết nối với socket để nhận sự kiện thực tế
            if (bid_received)
            {
                remaining_time = 30; // Reset thời gian khi có người ra giá
            }

            if (remaining_time <= 0)
            {
                auction_running = FALSE;
                char label[100];
                sprintf(label, "Auction ended for item: %s - user:", current_item->item_name);
                gtk_label_set_text(GTK_LABEL(context->label_waiting), label);
                // Xử lý lưu kết quả đấu giá vào server
                // Ví dụ: handle_save_auction_result(context->sockfd, current_item->item_id, winner);
            }

            g_usleep(1000 * 1000); // Nghỉ 1 giây (đồng bộ với g_timeout_add)
        }

        // Nghỉ 10 giây trước khi đấu giá vật phẩm tiếp theo
        printf("Waiting 10 seconds before next item.\n");
        remaining_time = 10;
        gtk_label_set_text(GTK_LABEL(context->label_waiting), "Preparing next item...");
        while (remaining_time > 0)
        {
            g_timeout_add(1000, (GSourceFunc)waiting_countdown, context);
            g_usleep(1000 * 1000); // Nghỉ 1 giây
        }
    }

    printf("All items have been auctioned.\n");
    gtk_label_set_text(GTK_LABEL(context->label_waiting), "Auction session completed.");
}

////////////////// ////////////////// //////////////////

void reload_auction_view(gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    // Cập nhập số người tham gia phòng
    if (GTK_IS_LABEL(context->label_room_joiner))
    {
        Room room;

        int role = handle_join_room(context->sockfd, context->room_id, &room);

        if (role <= 0)
        {
            g_printerr("Failed to update room's joiner");
            return;
        }
        char joiner_count_text[32];
        snprintf(joiner_count_text, sizeof(joiner_count_text), "%d", room.numUsers);
        gtk_label_set_text(GTK_LABEL(context->label_room_joiner), joiner_count_text);
    }
    // Xóa nội dung cũ
    clear_item_children(context->item_list);

    // Nạp lại dữ liệu
    fetch_item(context);
}

gboolean on_socket_event_auction(GIOChannel *source, GIOCondition condition, gpointer user_data)
{
    AuctionContext *context = (AuctionContext *)user_data;

    if (context->auction_window && gtk_widget_get_visible(context->auction_window))
    {
        if (condition & G_IO_IN) // Có dữ liệu từ server
        {
            int sockfd = g_io_channel_unix_get_fd(source);
            char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);

            int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0)
            {
                printf("Server disconnected.\n");
                return FALSE; // Dừng theo dõi socket
            }

            // Xử lý thông điệp từ server
            char message_type = buffer[0];
            printf("test%d\n", buffer[0]);
            if (message_type == REFRESH)
            {
                printf("Server yêu cầu refresh dữ liệu.\n");
                reload_auction_view(context);
            }
            else if (message_type == START_AUCTION)
            {
                start_auction_process(context);
                // pthread_t thread;
                // pthread_create(&thread, NULL, (void*)start_auction_process, (void*)context);
            }
            else
                printf("Received unknown message type - auction: %d\n", message_type);
        }
    }
    return TRUE; // Tiếp tục theo dõi socket
}

////////////////// ////////////////// //////////////////

void init_auction_view(int sockfd, GtkWidget *home_window, Room room, int role)
{
    AuctionContext *auctionContext = g_malloc(sizeof(AuctionContext));
    auctionContext->sockfd = sockfd;
    auctionContext->home_window = home_window;
    auctionContext->role = role;

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
    g_signal_connect(window, "destroy", G_CALLBACK(on_auction_window_destroy), auctionContext);

    GtkWidget *label_room_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_name"));
    GtkWidget *label_room_owner = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_owner"));
    GtkWidget *add_button = GTK_WIDGET(gtk_builder_get_object(builder, "add_button"));
    GtkWidget *start_button = GTK_WIDGET(gtk_builder_get_object(builder, "start_button"));
    auctionContext->current_user = GTK_WIDGET(gtk_builder_get_object(builder, "current_user"));
    auctionContext->highest_price = GTK_WIDGET(gtk_builder_get_object(builder, "highest_price"));
    auctionContext->auction_item_name = GTK_WIDGET(gtk_builder_get_object(builder, "auction_item_name"));
    auctionContext->auction_time = GTK_WIDGET(gtk_builder_get_object(builder, "auction_time"));
    auctionContext->start_price = GTK_WIDGET(gtk_builder_get_object(builder, "start_price"));
    auctionContext->current_price = GTK_WIDGET(gtk_builder_get_object(builder, "current_price"));
    auctionContext->btn_bid1 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_bid1"));
    auctionContext->btn_bid2 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_bid2"));
    auctionContext->btn_bid3 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_bid3"));
    auctionContext->btn_bid4 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_bid4"));
    auctionContext->label_room_joiner = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_joiner"));
    auctionContext->auction_stack = GTK_STACK(gtk_builder_get_object(builder, "auction_stack"));
    auctionContext->label_waiting = GTK_WIDGET(gtk_builder_get_object(builder, "label_waiting"));
    auctionContext->label_countdown = GTK_WIDGET(gtk_builder_get_object(builder, "label_countdown"));

    if (GTK_IS_LABEL(label_room_name))
    {
        gtk_label_set_text(GTK_LABEL(label_room_name), room.roomName);
    }
    if (GTK_IS_LABEL(label_room_owner))
    {
        gtk_label_set_text(GTK_LABEL(label_room_owner), room.username);
    }
    if (GTK_IS_LABEL(auctionContext->label_room_joiner))
    {
        char joiner_count_text[32];
        snprintf(joiner_count_text, sizeof(joiner_count_text), "%d", room.numUsers);
        gtk_label_set_text(GTK_LABEL(auctionContext->label_room_joiner), joiner_count_text);
    }

    auctionContext->room_id = room.room_id;
    auctionContext->auction_window = window;

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

    if (role == 2)
    {
        gtk_widget_hide(add_button);
        gtk_widget_hide(start_button);
    }

    fetch_item(auctionContext);

    // Thêm watcher cho socket
    GIOChannel *io_channel = g_io_channel_unix_new(sockfd);
    g_io_add_watch(io_channel, G_IO_IN, on_socket_event_auction, auctionContext);

    g_object_unref(builder);
}
