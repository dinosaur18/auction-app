CC = gcc
CFLAGS = -Wall
GTK_FLAGS = `pkg-config --cflags --libs gtk+-3.0`

# Thư mục chứa mã nguồn
SRC_DIR = src
VIEWS_DIR = $(SRC_DIR)/views
UTILS_DIR = $(SRC_DIR)/utils

all: server client app

server: server.c user.c
	$(CC) $(CFLAGS) server.c user.c -o server

client: client.c user.c
	$(CC) $(CFLAGS) client.c user.c -o client

app: login.c
	$(CC) gui_client.c -o app $(CFLAGS) $(GTK_FLAGS)

clean:
	rm -f server client app
