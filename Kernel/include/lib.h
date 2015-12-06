#ifndef LIB_HEADER
#define LIB_HEADER

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

void* malloc(int length);
void* calloc(int length);
void free(void* m);

#endif