
#ifndef LIBC_HEADER
#define LIBC_HEADER

#include <definitions.h>

#define MAX_PRINTF_LEN 1024



void printf(char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
void putchar(char c);
void* calloc(int length);
void* malloc(int length);
int strcmp(char* string1, char* string2);
void fprintf(FILE_DESCRIPTOR fd, char* fmt, ...) __attribute__ ((format (printf, 2, 3)));;
int strlen(char* string);
int getchar();
int scanf(char* c, int length);
char* itoc(int number);
void clear_screen();
void exit();
bool string_numeric(char* str);
bool is_numeric(char c);
time_t* time();
void set_time(time_t * t);
int ctoi(char* c);

#endif
