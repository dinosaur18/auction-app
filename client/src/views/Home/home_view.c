#include <gtk/gtk.h>
#include "style_manager.h"
#include "home_view.h"
#include "auction_service.h"

typedef struct
{
    int sockfd;
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

void on_home_window_destroy(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *auth_window = (GtkWidget *)user_data;

    // Hiển thị lại cửa sổ đăng nhập khi cửa sổ Home bị đóng
    gtk_widget_show(auth_window);
}

////////////////// DASHBOARD PAGE //////////////////

GtkWidget *create_room_card(Room room, gpointer user_data)
{

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

    card = GTK_WIDGET(gtk_builder_get_object(builder, "room_card"));

    GtkWidget *parent = gtk_widget_get_parent(card);
    if (parent != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(parent), card);
    }

    // Tìm và cập nhật nội dung trong card
    GtkWidget *label_room_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_name"));
    // GtkWidget *label_room_owner = GTK_WIDGET(gtk_builder_get_object(builder, "label_room_owner"));
    // GtkWidget *label_item_count = GTK_WIDGET(gtk_builder_get_object(builder, "label_item_count"));

    if (GTK_IS_LABEL(label_room_name))
        gtk_label_set_text(GTK_LABEL(label_room_name), room.roomName);

    // if (GTK_IS_LABEL(label_room_owner))
    //     gtk_label_set_text(GTK_LABEL(label_room_name), room.username);

    // if (GTK_IS_LABEL(label_item_count))
    // {
    //     char item_count_text[32];
    //     snprintf(item_count_text, sizeof(item_count_text), "%d items", room.numItems);
    //     gtk_label_set_text(GTK_LABEL(label_item_count), item_count_text);
    // }

    return card;
}

void fetch_all_room(gpointer user_data)
{
    AppContext *context = (AppContext *)user_data;

    // Tạo mảng Room để lưu danh sách phòng nhận được
    Room rooms[MAX_ROOMS];
    int room_count = handle_fetch_all_rooms(context->sockfd, rooms);

    if (room_count < 0)
    {
        g_printerr("Failed to fetch room list from server\n");
        return;
    }

    // Duyệt qua danh sách phòng và thêm vào GTK_FLOW_BOX
    for (int i = 0; i < room_count; i++)
    {

        GtkWidget *room_card = create_room_card(rooms[i], context); // Hiển thị tên phòng
        gtk_flow_box_insert(context->room_list_all, room_card, -1);
    }

    // Hiển thị các widget vừa thêm
    gtk_widget_show_all(GTK_WIDGET(context->room_list_all));
}

////////////////// MY AUCTION PAGE //////////////////

void fetch_own_room(int sockfd, GtkFlowBox *room_list)
{
    // Tạo mảng Room để lưu danh sách phòng nhận được
    Room rooms[MAX_ROOMS];
    int room_count = handle_fetch_own_rooms(sockfd, rooms);

    if (room_count < 0)
    {
        g_printerr("Failed to fetch room list from server\n");
        return;
    }

    // Duyệt qua danh sách phòng và thêm vào GTK_FLOW_BOX
    for (int i = 0; i < room_count; i++)
    {
        printf("Phòng của toi: %s\n", rooms[i].roomName);
        printf("user: %s\n", rooms[i].username);
        printf("-----------------------------\n");
        GtkWidget *room_button = gtk_button_new_with_label(rooms[i].roomName); // Hiển thị tên phòng
        gtk_flow_box_insert(room_list, room_button, -1);
    }

    // Hiển thị các widget vừa thêm
    gtk_widget_show_all(GTK_WIDGET(room_list));
}

void show_create_room_form(GtkWidget *button, gpointer user_data)
{
    AppContext *context = (AppContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->create_room_form);
    gtk_widget_show(form);
}

void on_create_room_ok(GtkWidget *button, gpointer user_data)
{
    AppContext *context = (AppContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->create_room_form);
    GtkFlowBox *room_list = GTK_FLOW_BOX(context->room_list);
    const gchar *room_name = gtk_entry_get_text(GTK_ENTRY(context->room_name));

    printf("%s\n", room_name);

    int roomId = handle_create_room(context->sockfd, room_name);
    if (roomId > 0)
    {
        gtk_widget_hide(form);
        GtkWidget *room_card = gtk_button_new_with_label(room_name);
        gtk_flow_box_insert(room_list, room_card, -1);
        gtk_widget_show(room_card);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(context->create_room_msg), "This room's name already exists.");
    }
}

void on_create_room_cancel(GtkWidget *button, gpointer user_data)
{
    AppContext *context = (AppContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->create_room_form);
    gtk_widget_hide(form);
}

////////////////// ////////////////// //////////////////

void clear_all_children(GtkFlowBox *flowbox)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(flowbox));
    for (GList *iter = children; iter != NULL; iter = iter->next)
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void on_tab_switch(GtkStack *stack, GParamSpec *pspec, gpointer user_data)
{
    AppContext *context = (AppContext *)user_data;

    // Lấy widget hiện tại đang hiển thị
    GtkWidget *current_child = gtk_stack_get_visible_child(stack);

    // Kiểm tra nếu là tab All Rooms
    if (current_child == GTK_WIDGET(context->dashboard))
    {
        printf("Switched to Dashboard tab.\n");

        // // Xóa các widget hiện có trong flowbox trước khi fetch lại
        clear_all_children(context->room_list_all);

        // // Fetch lại danh sách tất cả các phòng
        fetch_all_room(context);
    }
    // Kiểm tra nếu là tab My Auctions
    else if (current_child == GTK_WIDGET(context->my_auction))
    {
        printf("Switched to My Auctions tab.\n");

        // // Xóa các widget hiện có trong flowbox trước khi fetch lại
        clear_all_children(context->room_list);

        // // Fetch lại danh sách các phòng của người dùng
        fetch_own_room(context->sockfd, context->room_list);
    }
}

void init_home_view(int sockfd, GtkWidget *auth_window)
{
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "client/src/views/Home/home_view.glade", &error))
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_home"));
    g_signal_connect(window, "destroy", G_CALLBACK(on_home_window_destroy), auth_window);

    AppContext *appContext = g_malloc(sizeof(AppContext));
    appContext->sockfd = sockfd;

    appContext->dashboard = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard"));
    appContext->room_list_all = GTK_FLOW_BOX(gtk_builder_get_object(builder, "room_list_all"));
    appContext->room_card = GTK_WIDGET(gtk_builder_get_object(builder, "room_card"));

    appContext->my_auction = GTK_WIDGET(gtk_builder_get_object(builder, "my_auction"));
    appContext->room_list = GTK_FLOW_BOX(gtk_builder_get_object(builder, "room_list"));
    appContext->create_room_form = GTK_WIDGET(gtk_builder_get_object(builder, "create_room_form"));
    appContext->room_name = GTK_WIDGET(gtk_builder_get_object(builder, "room_name"));
    appContext->create_room_msg = GTK_WIDGET(gtk_builder_get_object(builder, "create_room_msg"));

    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack_app"));
    g_signal_connect(stack, "notify::visible-child", G_CALLBACK(on_tab_switch), appContext);

    gtk_builder_connect_signals(builder, appContext);

    apply_css();
    gtk_widget_show_all(window);

    fetch_all_room(appContext);

    g_object_unref(builder);
}
