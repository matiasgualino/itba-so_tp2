#include "../include/definitions.h"
#include "../include/stdio.h"
#include "../include/syscalls.h"

#define ENTER			'\n'
#define TAB				'\t'
#define SPACE			' '
#define SCAN_ERROR		0

const char hexDigits [] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/* getc implementation */
int getc(int fileDescriptor){
	byte c;
	return (__read(fileDescriptor, &c, 1) == 1) ? (byte) c : EOF;
}

/* putc implementation */
int putc(int c, int fileDescriptor){
	int ret;
	byte aux = BLUE_TEXT;
	byte character = (byte) c;

	if(c == TAB){
		putc(' ', fileDescriptor);
		putc(' ', fileDescriptor);
		putc(' ', fileDescriptor);
	} else {
		ret = (__write(fileDescriptor, &character, 1) == 1) ? c : EOF;
		__write(fileDescriptor, &aux, 1);
	}
	return ret;
}

int isDigit(char c){
	if(c >= '0' && c <= '9'){
		return TRUE;
	}
	return FALSE;
}

int isSpace(char c){
	if(c == '\n' || c == '\t' || c == ' '){
		return TRUE;
	}
	return FALSE;
}

int stringToInt(char string, int length){
	int i, num = 0;

	for(i = 0; i < length; i++){
		num *= 10;
		num += (string[i] - '0');
	}
	return num;
}