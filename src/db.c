#include <stdlib.h>
#include <stdio.h>
#include "db.h"
#include "./config.h"

Database* create_database() {
    Database *db = (Database*)malloc(sizeof(Database));
    if (!db) {
        perror("Failed to allocate memory for database");
        exit(EXIT_FAILURE);
    }
    db->map = create_hash_map(HASH_MAP_SIZE);
    return db;
}

void destroy_database(Database *db) {
    destroy_hash_map(db->map);
    free(db);
}

bool add_data(Database *db, const char *key, const char *value) {
    return insert(db->map, key, value);
}

char* get_data(Database *db, const char *key) {
    return get(db->map, key);
}

bool delete_data(Database *db, const char *key) {
    return delete_key(db->map, key);
}


