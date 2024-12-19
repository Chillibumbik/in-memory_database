#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Утилитарные функции
char* str_dup(const char *str); // Дублирование строки
char* trim_whitespace(char *str); // Удаление пробелов с концов строки
void parse_command_line(int argc, char *argv[], int *port, char **server_address);

#endif 