# Compiler và cờ biên dịch
CC = gcc
CFLAGS = -Wall -rdynamic
GTK_FLAGS = `pkg-config --cflags --libs gtk+-3.0`

# Thư mục chính
CLIENT_DIR = client/src
AUTH_DIR = $(CLIENT_DIR)/views/Auth
UTILS_DIR = $(CLIENT_DIR)/utils
SERVICES_DIR = $(CLIENT_DIR)/services

SERVER_DIR = server/src
USER_DIR = $(SERVER_DIR)/user
INCLUDE_DIR = include

# Tệp nguồn và đầu ra
CLIENT_SOURCES = 	$(CLIENT_DIR)/main.c \
					$(AUTH_DIR)/auth_view.c \
					$(UTILS_DIR)/style_manager.c \
					$(SERVICES_DIR)/auth_service.c \
					$(USER_DIR)/models/user.c \

SERVER_SOURCES = 	$(SERVER_DIR)/main.c \
					$(USER_DIR)/models/user.c \

CLIENT = client_exec
SERVER = server_exec

INCLUDES = 	-I$(AUTH_DIR) -I$(UTILS_DIR) -I$(SERVICES_DIR) \
			-I$(USER_DIR)/models \
			-I$(INCLUDE_DIR)

# Quy tắc biên dịch
all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT_SOURCES) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(CLIENT) $(CLIENT_SOURCES) $(GTK_FLAGS)

$(SERVER): $(SERVER_SOURCES) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(SERVER) $(SERVER_SOURCES) $(GTK_FLAGS)

# Quy tắc dọn dẹp
clean:
	rm -f $(CLIENT) $(SERVER)