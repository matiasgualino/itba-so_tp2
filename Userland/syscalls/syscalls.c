#include <syscalls.h>
#include <definitions.h>
#include <stdint.h>

extern uint64_t syscall(uint64_t callid, ...);

void sys_exit() {
	syscall((uint64_t)SYSCALL_EXIT);
}

void sys_get_time(time_t* t) {
	syscall((uint64_t)SYSCALL_GET_TIME, (uint64_t)t);
}

void sys_set_time(time_t* t) {
	syscall((uint64_t)SYSCALL_SET_TIME, (uint64_t)t);
}

void sys_write(FILE_DESCRIPTOR fileDescriptor, char * string, int length){
	syscall((uint64_t)SYSCALL_WRITE, (uint64_t)fileDescriptor, (uint64_t)string, (uint64_t)length);
}

int sys_read(FILE_DESCRIPTOR fileDescriptor, char * string, int length){
	return syscall((uint64_t)SYSCALL_READ, (uint64_t)fileDescriptor, (uint64_t)string, (uint64_t)length);
}

void sys_clear_screen() {
	syscall((uint64_t)SYSCALL_CLEAR_SCREEN);
}

void* sys_malloc(int length) {
	return (void*)syscall((uint64_t)SYSCALL_MALLOC, (uint64_t)length);
}

void* sys_calloc(int length) {
	return (void*)syscall((uint64_t)SYSCALL_CALLOC, (uint64_t)length);
}

void sys_free(void* m) {
	syscall((uint64_t)SYSCALL_FREE, (uint64_t)m);
}

void sys_keyboard_replace_buffer(char* string) {
	syscall((uint64_t)SYSCALL_KEYBOARD_REPLACE_BUFFER, (uint64_t)string);
}

void sys_set_delay_screensaver(uint64_t t) {
	syscall((uint64_t)SYSCALL_DELAY_SCREENSAVER, (uint64_t)t);
}

void sys_show_screensaver() {
	syscall((uint64_t)SYSCALL_SHOW_SCREENSAVER);
}