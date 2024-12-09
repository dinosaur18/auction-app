#include <gtk/gtk.h>
#include "style_manager.h"
#include "auth_view.h"
#include "home_view.h"
#include "auth_service.h"

typedef struct
{
    int sockfd;
    GtkWidget *login_username;
    GtkWidget *login_password;
    GtkWidget *login_message;

    GtkWidget *register_username;
    GtkWidget *register_password;
    GtkWidget *register_message;
    GtkWidget *auth_window;
} AuthContext;

void on_auth_window_destroy(GtkWidget *widget, gpointer user_data)
{
    gtk_main_quit();
}

void on_register_button_clicked(GtkWidget *button, gpointer user_data)
{
    AuthContext *context = (AuthContext *)user_data;

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(context->register_username));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(context->register_password));

    int result = handle_register(context->sockfd, username, password);
    if (result > 0)
    {
        gtk_label_set_text(GTK_LABEL(context->register_message), "Register successful!");

        // Ẩn cửa sổ đăng nhập
        gtk_widget_hide(context->auth_window);

        // Chuyển sang giao diện Home
        init_home_view(context->sockfd, context->auth_window);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(context->register_message), "Username already exists.");
    }
}

void on_login_button_clicked(GtkWidget *button, gpointer user_data)
{
    AuthContext *context = (AuthContext *)user_data;

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(context->login_username));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(context->login_password));

    int result = handle_login(context->sockfd, username, password);
    printf("%d\n", result);
    if (result > 0)
    {
        gtk_label_set_text(GTK_LABEL(context->login_message), "Login successful!");

        // Ẩn cửa sổ đăng nhập
        gtk_widget_hide(context->auth_window);

        // Chuyển sang giao diện Home
        init_home_view(context->sockfd, context->auth_window);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(context->login_message), "Invalid username or password.");
    }
}

void init_auth_view(int sockfd)
{
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "client/src/views/Auth/auth_view.glade", &error))
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_auth"));
    g_signal_connect(window, "destroy", G_CALLBACK(on_auth_window_destroy), NULL);

    AuthContext *context = g_malloc(sizeof(AuthContext));
    context->sockfd = sockfd;
    context->auth_window = window;
    context->login_username = GTK_WIDGET(gtk_builder_get_object(builder, "login_username"));
    context->login_password = GTK_WIDGET(gtk_builder_get_object(builder, "login_password"));
    context->login_message = GTK_WIDGET(gtk_builder_get_object(builder, "login_msg"));
    context->register_username = GTK_WIDGET(gtk_builder_get_object(builder, "register_username"));
    context->register_password = GTK_WIDGET(gtk_builder_get_object(builder, "register_password"));
    context->register_message = GTK_WIDGET(gtk_builder_get_object(builder, "register_msg"));

    gtk_builder_connect_signals(builder, context);

    apply_css();
    gtk_widget_show_all(window);

    gtk_main();

    g_object_unref(builder);
    g_free(context);
}
