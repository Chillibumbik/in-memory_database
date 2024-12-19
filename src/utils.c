#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "config.h"

// Дублирование строки
char* str_dup(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char *copy = (char*)malloc(len);
    if (!copy) {
        perror("Failed to allocate memory for string duplication");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, str, len);
    return copy;
}

// Удаление пробелов с концов строки
char* trim_whitespace(char *str) {
    if (!str) return NULL;
    char *end;

    // Удаляем пробелы в начале строки
    while (*str == ' ' || *str == '\t' || *str == '\n') str++;

    // Если строка состоит только из пробелов
    if (*str == '\0') return str;

    // Удаляем пробелы в конце строки
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n')) end--;

    // Ставим терминальный ноль
    *(end + 1) = '\0';
    return str;
}

// Парсинг аргументов командной строки
void parse_command_line(int argc, char *argv[], int *port, char **server_address) {
    *port = DEFAULT_PORT; // Значение по умолчанию
    *server_address = "127.0.0.1"; // Локальный хост по умолчанию

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            *port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            *server_address = argv[++i];
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            fprintf(stderr, "Usage: %s [-p port] [-h server_address]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}
