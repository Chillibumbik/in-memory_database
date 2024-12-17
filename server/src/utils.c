// src/utils.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

// Функция для безопасного получения строки с ввода, удаляет символ новой строки
void get_input_string(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;  // Убираем символ новой строки
    }
}

// Функция для безопасного копирования строки
void safe_str_copy(char *dest, const char *src, int dest_size) {
    strncpy(dest, src, dest_size - 1);  // Копируем строку с ограничением на размер
    dest[dest_size - 1] = '\0';  // Обеспечиваем завершающий нулевой символ
}
