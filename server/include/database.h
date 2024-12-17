// include/database.h
#ifndef DATABASE_H
#define DATABASE_H

// Объявления функций
void put_data(const char *key, const char *value);      // Добавить данные
const char *get_data(const char *key);                  // Получить данные по ключу
void delete_data(const char *key);                      // Удалить данные по ключу
void show_database();                                   // Показать все данные
void clear_database();                                  // Очистить базу данных

#endif
