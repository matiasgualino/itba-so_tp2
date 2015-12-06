
#ifndef SYSCALLS_HEADER
#define SYSCALLS_HEADER

#include <definitions.h>

#define SYSCALL_GET_TIME					1
#define SYSCALL_SET_TIME					2
#define SYSCALL_READ						3
#define SYSCALL_WRITE						4
#define SYSCALL_MALLOC						5
#define SYSCALL_CALLOC						6
#define SYSCALL_FREE						7
#define SYSCALL_KEYBOARD_REPLACE_BUFFER		8
#define SYSCALL_DELAY_SCREENSAVER			9
#define SYSCALL_SHOW_SCREENSAVER			10
#define SYSCALL_CLEAR_SCREEN				11
#define SYSCALL_EXIT						12

void sys_get_time(time_t* t);
void sys_set_time(time_t* t);
int sys_read(FILE_DESCRIPTOR fileDescriptor, char * string, int length);
void sys_write(FILE_DESCRIPTOR fileDescriptor, char * string, int length);
void* sys_malloc(int length);
void* sys_calloc(int length);
void sys_free(void* m);
void sys_keyboard_replace_buffer(char* string);
void sys_set_delay_screensaver(uint64_t t);
void sys_show_screensaver();
void sys_clear_screen();
void sys_exit();

#endif