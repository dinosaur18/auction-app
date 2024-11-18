#include "style_manager.h"

void apply_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    // Tải CSS từ file
    if (!gtk_css_provider_load_from_path(provider, "assets/style.css", NULL)) {
        g_warning("Không thể tải file CSS: assets/style.css");
    }
    
    // Áp dụng CSS cho toàn bộ màn hình
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);
}
