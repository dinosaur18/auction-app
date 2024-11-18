#include <gtk/gtk.h>
#include "../../utils/style_manager.h"

// Hàm đóng ứng dụng khi cửa sổ bị đóng
void on_main_window_destroy() {
    gtk_main_quit();
}

void run_auth_view() {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;

    // Khởi tạo GTK
    gtk_init(NULL, NULL);

    // Tạo GtkBuilder và tải tệp Glade
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "src/views/Auth/auth_view.glade", &error)) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    // Lấy đối tượng GtkWindow từ tệp Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_login"));
    g_signal_connect(window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);

    // Áp dụng CSS
    apply_css();

    // Hiển thị cửa sổ
    gtk_widget_show_all(window);

    // Vòng lặp chính của GTK
    gtk_main();

    // Giải phóng tài nguyên
    g_object_unref(builder);
}
