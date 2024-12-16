#include <gtk/gtk.h>
#include "style_manager.h"
#include "home_view.h"

void on_home_window_destroy(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *auth_window = (GtkWidget *)user_data;

    // Hiển thị lại cửa sổ đăng nhập khi cửa sổ Home bị đóng
    gtk_widget_show(auth_window);
}

////////////////// CREATE ROOM FORM //////////////////

typedef struct
{
    int sockfd;
    GtkWidget *create_room_form;
    GtkWidget *room_name;
    GtkWidget *create_room_msg;
    GtkFlowBox *room_list;
    GtkWidget *room_card;
} RoomListContext;

void show_create_room_form(GtkWidget *button, gpointer user_data)
{
    RoomListContext *context = (RoomListContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->create_room_form);
    gtk_widget_show(form);
}

void on_create_room_ok(GtkWidget *button, gpointer user_data)
{
    RoomListContext *context = (RoomListContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->create_room_form);
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
    RoomListContext *context = (RoomListContext *)user_data;

    GtkWidget *form = GTK_WIDGET(context->create_room_form);
    gtk_widget_hide(form);
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

    RoomListContext *roomListContext = g_malloc(sizeof(RoomListContext));
    roomListContext->sockfd = sockfd;
    roomListContext->create_room_form = GTK_WIDGET(gtk_builder_get_object(builder, "create_room_form"));
    roomListContext->room_name = GTK_WIDGET(gtk_builder_get_object(builder, "room_name"));
    roomListContext->create_room_msg = GTK_WIDGET(gtk_builder_get_object(builder, "create_room_msg"));
    roomListContext->room_list = GTK_FLOW_BOX(gtk_builder_get_object(builder, "room_list"));

    gtk_builder_connect_signals(builder, roomListContext);

    apply_css();
    gtk_widget_show_all(window);

    g_object_unref(builder);
}
