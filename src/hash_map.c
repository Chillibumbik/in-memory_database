#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash_map.h"

// Определение strdup, если оно отсутствует
#ifndef strdup
char* strdup(const char *str) {
    char *dup = malloc(strlen(str) + 1);
    if (dup) {
        strcpy(dup, str);
    }
    return dup;
}
#endif

// Вспомогательная функция для хеширования ключей
static size_t hash_function(const char *key, size_t size) {
    size_t hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % size;
}

// Создание хеш-таблицы
HashMap* create_hash_map(size_t size) {
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) {
        perror("Failed to allocate memory for hash map");
        exit(EXIT_FAILURE);
    }
    map->table = (HashMapNode**)calloc(size, sizeof(HashMapNode*));
    if (!map->table) {
        perror("Failed to allocate memory for hash map table");
        free(map);
        exit(EXIT_FAILURE);
    }
    map->size = size;
    return map;
}

// Уничтожение хеш-таблицы
void destroy_hash_map(HashMap *map) {
    for (size_t i = 0; i < map->size; i++) {
        HashMapNode *node = map->table[i];
        while (node) {
            HashMapNode *temp = node;
            node = node->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(map->table);
    free(map);
}

// Вставка элемента в хеш-таблицу
bool insert(HashMap *map, const char *key, const char *value) {
    size_t index = hash_function(key, map->size);
    HashMapNode *node = map->table[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            free(node->value);
            node->value = strdup(value); // Используем strdup для копирования строки
            return true;
        }
        node = node->next;
    }
    node = (HashMapNode*)malloc(sizeof(HashMapNode));
    if (!node) {
        perror("Failed to allocate memory for hash map node");
        return false;
    }
    node->key = strdup(key); // Используем strdup для копирования строки
    node->value = strdup(value);
    node->next = map->table[index];
    map->table[index] = node;
    return true;
}

// Получение значения по ключу
char* get(HashMap *map, const char *key) {
    size_t index = hash_function(key, map->size);
    HashMapNode *node = map->table[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

// Удаление элемента по ключу
bool delete_key(HashMap *map, const char *key) {
    size_t index = hash_function(key, map->size);
    HashMapNode *node = map->table[index];
    HashMapNode *prev = NULL;
    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                map->table[index] = node->next;
            }
            free(node->key);
            free(node->value);
            free(node);
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}

// Печать всех элементов хеш-таблицы
void print_all(HashMap *map) {
    for (size_t i = 0; i < map->size; i++) {
        HashMapNode *node = map->table[i];
        while (node) {
            printf("Key: %s, Value: %s\n", node->key, node->value);
            node = node->next;
        }
    }
}
