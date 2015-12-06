#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER

#include <definitions.h>

#define KEYBOARD_BUFFER_SIZE 128

typedef struct {
	char scancode;
	char ascii;
	char specialCharacter;
} keyboardCode;

typedef struct {
	bool capsON;
	bool ctrlON;
	bool altON;
} specialKeysStatus;

int waitBuffer(int length);
char getCharFromBuffer();
void keyboardHandler(uint64_t string);
void replaceLastWritten(char* string);

#endif