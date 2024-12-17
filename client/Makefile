# client/Makefile

CC = gcc
CFLAGS = -Wall -g
INCLUDES = -Iinclude
LIBS = -lws2_32
SRC = src/client.c src/utils.c src/database.c
OBJ = obj/client.o obj/utils.o obj/database.o
EXEC = client.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
