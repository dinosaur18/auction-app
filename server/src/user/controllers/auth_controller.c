#include <gtk/gtk.h>
#include <string.h>
#include "user.h"
#include "auth_view.h"

// Khai báo biến toàn cục cho các widget
GtkWidget *entry_username, *entry_password, *label_message;

// Hàm xử lý sự kiện khi nhấn nút "Login"
void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    printf("clicked!\n");
    // const char *username = gtk_entry_get_text(GTK_ENTRY(entry_username));
    // const char *password = gtk_entry_get_text(GTK_ENTRY(entry_password));

    // if (authenticate_user(username, password)) {
    //     gtk_label_set_text(GTK_LABEL(label_message), "Login Successful!");
    // } else {
    //     gtk_label_set_text(GTK_LABEL(label_message), "Invalid Credentials");
    // }
}

// Hàm khởi tạo Controller
void init_auth_controller(int argc, char *argv[]) {
    GtkBuilder *builder;
    init_auth_view(argc, argv, &builder);

    // // Lấy các widget từ builder
    // entry_username = GTK_WIDGET(gtk_builder_get_object(builder, "entry_username"));
    // entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password"));
    // label_message = GTK_WIDGET(gtk_builder_get_object(builder, "label_message"));

    // Kết nối sự kiện
    gtk_builder_connect_signals(builder, NULL);

    gtk_main(); // Bắt đầu vòng lặp GTK
}
