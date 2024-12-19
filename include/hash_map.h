#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h> // Для size_t
#include <stdbool.h> // Для типа bool

// Структура для элемента хеш-таблицы (ключ-значение)
typedef struct HashMapNode {
    char *key;                // Ключ
    char *value;              // Значение
    struct HashMapNode *next; // Указатель на следующий элемент в случае коллизии
} HashMapNode;

// Структура для хеш-таблицы
typedef struct HashMap {
    HashMapNode **table;  // Массив указателей на списки
    size_t size;          // Количество элементов в хеш-таблице
} HashMap;

// Функции для работы с хеш-таблицей
HashMap* create_hash_map(size_t size);
void destroy_hash_map(HashMap *map);
bool insert(HashMap *map, const char *key, const char *value);
char* get(HashMap *map, const char *key);
bool delete_key(HashMap *map, const char *key);
void print_all(HashMap *map);

#endif 
