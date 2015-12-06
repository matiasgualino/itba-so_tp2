#include <stdarg.h>
#include <definitions.h>
#include <syscalls.h>
#include <libc.h>

void* calloc(int length) {
	return sys_calloc(length);
}

void* malloc(int length) {
	return sys_malloc(length);
}

void printf(char* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	sys_write(STDOUT, fmt, strlen(fmt));

	va_end(ap);

}

void putchar(char c) {

	sys_write(STDOUT, &c, 1);
}

int strcmp(char * str1, char * str2){
	int i;
	int length1 = strlen(str1);
	int lenght2 = strlen(str2);

	if(length1 != lenght2){
		return length1 - lenght2;
	}

	for(i = 0; i < length1; i++){
		if(str1[i] != str2[i]){
			return str1[i] > str2[i] ? 1 : -1;
		}
	}
	return 0;
}

int strlen(char * str){
	int i = 0;
	
	while (str[i]){
		i++;
	}
	return i;
}

int getchar() {

	static char buffer[2] = {0};

	char read = sys_read(STDOUT, buffer, 1);

	if (read == EOF) {
		return EOF;
	}

	return (int) buffer[0];
}

int scanf(char* c, int length) {

	char read = sys_read(STDOUT, c, length);

	return read;
}

char* itoc(int number) {

	int i = 0;
	int j = 0;
	int cnt = 0;

	char* c = malloc(10);

	if (number < 0) {
		number = -number;
		c[i++] = '-';
	}

	while (number >= 10 ) {
		int dig = number % 10;
		number /= 10;
		c[i++] = dig + '0';
		cnt++;
	}
	c[i] = number + '0';

	while (cnt >= j) {
		char aux;
		aux = c[cnt];
		c[cnt--] = c[j];
		c[j++] = aux;
	}

	return c;

}

bool string_numeric(char* str) {
	int len;
	len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (!is_numeric(str[i])) {
			return FALSE;
		}
	}
	return TRUE;
}

bool is_numeric(char c) {
	return (c >= '0' && c <= '9');
}

time_t* time() {
	time_t* t = (time_t*)calloc(sizeof(time_t));
	sys_get_time(t);
	return t;
}

void set_time(time_t * t) {
	sys_set_time(t);
}

void clear_screen() {
	sys_clear_screen();
}

void exit() {
	sys_exit();
}

int ctoi(char* c) {
	int ans;
	ans = 0;
	int len;
	len = strlen(c);
	int i;
	i = 0;
	while  (i < len) {
		ans = ans * 10 + c[i] - '0';
		i++;
	}
	return ans;
}