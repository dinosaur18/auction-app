#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    gtk_init(&argc, &argv);

    // Nạp tệp .glade
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "interface.glade", &error)) {
        g_print("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Lấy tham chiếu tới widget window từ file .glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!window) {
        g_print("Unable to fetch window from .glade file\n");
        return 1;
    }

    // Kết nối tín hiệu (signals)
    gtk_builder_connect_signals(builder, NULL);

    // Hiển thị cửa sổ
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

// Hàm xử lý sự kiện đóng cửa sổ
void on_main_window_destroy() {
    gtk_main_quit();
}
