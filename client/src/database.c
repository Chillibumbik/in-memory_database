// src/database.c
#include <stdio.h>
#include <string.h>
#include "database.h"
#include "utils.h"

// Максимальное количество записей в базе
#define MAX_DB_SIZE 100

// Структура базы данных
typedef struct {
    char key[100];
    char value[100];
} DataEntry;

// Массив базы данных
static DataEntry database[MAX_DB_SIZE];
static int db_size = 0;

// Функция для добавления записи в базу данных
void put_data(const char *key, const char *value) {
    if (db_size < MAX_DB_SIZE) {
        safe_str_copy(database[db_size].key, key, sizeof(database[db_size].key));
        safe_str_copy(database[db_size].value, value, sizeof(database[db_size].value));
        db_size++;
    } else {
        printf("Database is full!\n");
    }
}

// Функция для получения записи по ключу
const char *get_data(const char *key) {
    for (int i = 0; i < db_size; i++) {
        if (strcmp(database[i].key, key) == 0) {
            return database[i].value;
        }
    }
    return NULL;
}

// Функция для удаления записи по ключу
void delete_data(const char *key) {
    for (int i = 0; i < db_size; i++) {
        if (strcmp(database[i].key, key) == 0) {
            // Сдвигаем все элементы после удаленной записи
            for (int j = i; j < db_size - 1; j++) {
                database[j] = database[j + 1];
            }
            db_size--;
            return;
        }
    }
    printf("Key not found for deletion!\n");
}

// Функция для отображения всей базы данных
void show_database() {
    if (db_size == 0) {
        printf("Database is empty.\n");
        return;
    }

    printf("Database contents:\n");
    for (int i = 0; i < db_size; i++) {
        printf("Key: %s, Value: %s\n", database[i].key, database[i].value);
    }
}

// Функция для очистки базы данных
void clear_database() {
    db_size = 0;
    printf("Database cleared.\n");
}
