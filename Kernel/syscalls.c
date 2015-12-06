
#include <syscalls.h>
#include <definitions.h>
#include <video.h>
#include <realTimeClock.h>
#include <keyboard.h>
#include <lib.h>

extern uint64_t screensaverWaitTime;
extern bool screensaverActive;

void sys_get_time(time_t* t) {
	getTime(t);
}

void sys_set_time(time_t* t) {
	setTime(t);
}

int sys_read(FILE_DESCRIPTOR fileDescriptor, char * string, int length){
	int read = 0;

	int i = 0;

	read = waitBuffer(length);

	while (i < read) {
		string[i] = getCharFromBuffer();

		i++;

	}

	string[i] = 0;

	return read;
}

void sys_write(FILE_DESCRIPTOR fileDescriptor, char * string, int length){
	switch(fileDescriptor) {
		case STDOUT:
			video_set_font_background_color(COLOR_WHITE, COLOR_BLACK);
			break;
		case STDERR:
			video_set_font_background_color(COLOR_RED, COLOR_BLACK);
			break;
	}
	video_print_string(string);
}

void* sys_malloc(int lenght) {

	return malloc(lenght);

}

void* sys_calloc(int lenght) {

	return calloc(lenght);

}

void sys_free(void* m) {
	free(m);
}

void sys_keyboard_replace_buffer(char* string) {
	replaceLastWritten(string);
}

void sys_clear_screen() {
	video_clear_screen();
}

void sys_set_delay_screensaver(uint64_t t) {
	screensaverWaitTime = t;
	screensaverResetTimer();
}

void sys_show_screensaver() {
	activeScreensaver();
	while(screensaverActive);
}