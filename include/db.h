#ifndef DB_H
#define DB_H

#include <stdbool.h>
#include <stdio.h>
#include "hash_map.h"

typedef struct Database {
    HashMap *map;
} Database;

Database* create_database();
void destroy_database(Database *db);
bool add_data(Database *db, const char *key, const char *value);
char* get_data(Database *db, const char *key);
bool delete_data(Database *db, const char *key);

#endif 