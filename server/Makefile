# server/Makefile

CC = gcc
CFLAGS = -Wall -g
INCLUDES = -Iinclude
LIBS = -lws2_32
SRC = src/server.c src/utils.c src/database.c
OBJ = obj/server.o obj/utils.o obj/database.o
EXEC = server.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
