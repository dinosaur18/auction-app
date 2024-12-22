#ifndef HOME_VIEW_H
#define HOME_VIEW_H
#include <gtk/gtk.h>

#include "user.h"

// Khai báo hàm hoặc cấu trúc
void init_home_view(int sockfd, GtkWidget *auth_window, User user);

#endif