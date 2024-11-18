# Compiler và cờ biên dịch
CC = gcc
CFLAGS = -Wall 
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)

# Thư mục chứa mã nguồn
SRC_DIR = src
VIEWS_DIR = $(SRC_DIR)/views
UTILS_DIR = $(SRC_DIR)/utils

# Tên file thực thi
APP_EXEC = app

# Các tệp nguồn cho app
APP_SRC = $(SRC_DIR)/client.c $(VIEWS_DIR)/Auth/auth_view.c $(UTILS_DIR)/style_manager.c

<<<<<<< HEAD
# Các tệp .o tương ứng
APP_OBJ = $(APP_SRC:.c=.o)

# Đường dẫn tới file header
INCLUDES = -I$(VIEWS_DIR)/Auth -I$(UTILS_DIR)

# Mục tiêu mặc định
all: $(APP_EXEC)

# Luật biên dịch app với GTK+
$(APP_EXEC): $(APP_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(GTK_FLAGS)

# Quy tắc biên dịch các file .c thành .o (Thêm $(GTK_FLAGS) để tìm thấy gtk/gtk.h)
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(GTK_FLAGS) -c $< -o $@

# Dọn dẹp các file object và file thực thi
clean:
	rm -f $(APP_OBJ) $(APP_EXEC)
