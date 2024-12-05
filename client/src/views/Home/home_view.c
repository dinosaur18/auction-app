#include <gtk/gtk.h>
#include "style_manager.h"
#include "home_view.h"

void on_home_window_destroy(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *auth_window = (GtkWidget *)user_data;

    // Hiển thị lại cửa sổ đăng nhập khi cửa sổ Home bị đóng
    gtk_widget_show(auth_window);
}

void on_create_room_button_clicked(GtkWidget *button, gpointer user_data)
{
    printf("Clicked!\n");
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

    gtk_builder_connect_signals(builder, NULL);

    apply_css();
    gtk_widget_show_all(window);

    g_object_unref(builder);
}
