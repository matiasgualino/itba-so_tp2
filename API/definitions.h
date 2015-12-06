/******
	General Definitions
******/

#ifndef DEFINITIONS_HEADER
#define DEFINITIONS_HEADER

#include <stdint.h>

#define byte	unsigned char
#define word	short int
#define dword	int

#define TRUE	1
#define FALSE	0
#define NULL	0

typedef char bool;

#define EOF 	-1

/* REAL TIME CLOCK */
typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} time_t;

/* File descriptors */
typedef enum {
	STDOUT,
	STDERR
} FILE_DESCRIPTOR;

/** VIDEO ATTRIBUTES **/
typedef uint8_t color_t;

#define PROMPT "\nsarasa$ "

#endif