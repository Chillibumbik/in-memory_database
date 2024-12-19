# Название исполняемых файлов
SERVER_EXEC = server.exe
CLIENT_EXEC = client.exe

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -I.
LDFLAGS = -lws2_32 -Wl,-subsystem,console

# Директории
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Файлы исходного кода
SERVER_SRC = $(SRC_DIR)/server.c $(SRC_DIR)/db.c $(SRC_DIR)/hash_map.c $(SRC_DIR)/net.c
CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/net.c
UTIL_SRC = $(SRC_DIR)/utils.c

# Объектные файлы
SERVER_OBJ = $(SERVER_SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o) $(UTIL_SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
CLIENT_OBJ = $(CLIENT_SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o) $(UTIL_SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

# Правило по умолчанию
all: $(BIN_DIR) $(SERVER_EXEC) $(CLIENT_EXEC)

# Создание серверного исполняемого файла
$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Создание клиентского исполняемого файла
$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Компиляция исходных файлов в объектные файлы
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Создание директории для объектных файлов
$(BIN_DIR):
	mkdir $(BIN_DIR)

# Очистка
clean:
	del /q $(BIN_DIR)\*.o $(SERVER_EXEC) $(CLIENT_EXEC)
	rmdir $(BIN_DIR)

# Очистка и повторная сборка
rebuild: clean all
