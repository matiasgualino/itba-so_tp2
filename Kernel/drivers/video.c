#include "video.h"

int video_current_row = 0;
int video_current_column = 0;
static color_t video_current_color = 0;

static screen_t screensaverBackup;

static void video_scroll();
static uint16_t video_get_full_char_from(int row, int col);
static void video_reset_color();
static void video_reset_current_row();
static void video_reset_current_column();
static void video_print_screensaver();

void video_init(){
	video_reset_color();
	video_clear_screen();
}

// COLOR
void video_reset_color(){
	video_current_color = CREATE_COLOR(COLOR_WHITE, COLOR_BLACK);
}

void video_update_screen_color() {
	for (int c = 1; c < SCREEN_WIDTH * SCREEN_HEIGHT * 2; c += 2) {
		((uint8_t*)SCREEN_START)[c] = video_current_color;
	}
}

void video_set_color(color_t color){
	video_current_color = color;
}

void video_set_font_background_color(video_color font, video_color background){
	video_current_color = CREATE_COLOR(font, background);
}

void video_clear_screen(){
	for(int j = 0; j < SCREEN_WIDTH; j++){
		for(int i = 0; i < SCREEN_HEIGHT; i++){
			video_printc(' ');
		}
	}
	video_reset_cursor();
	video_update_screen_color();
}

color_t video_get_color(){
	return video_current_color;
}

// PRINT
void video_printc(const char c){
	uint16_t char_16 = c;
	uint16_t color_16 = video_current_color;
	video_print_char(char_16 | (color_16 << 8));
}

void video_print_char(uint16_t c) {

	video_print_char_at(c, video_current_row, video_current_column);

	video_current_column++;

	/*	Chequeamos que no se pase del ancho de pantalla.
		En ese caso ponemos la columna en 0 y hacemos un salto de linea.
	*/
	if (video_current_column == SCREEN_WIDTH) {
		video_reset_current_column();
		video_current_row++;
	}

	/*	Chequeamos que no se pase del alto de la pantalla.
		En ese caso hay que hacer un scroll.
	*/
	if (video_current_row == SCREEN_HEIGHT) {
		video_scroll();
	}
}

void video_print_char_at(uint16_t c, int row, int col){
	SCREEN_START[row * SCREEN_WIDTH + col] = c;
}

void video_print_line(){
	for(int i = 0; video_current_column < SCREEN_WIDTH; i++){
		video_printc(' ');
		video_current_column = i;
	}
	video_reset_current_column();
	video_current_row++;
}

void video_print_new_line(){
	video_reset_current_column();
	video_current_row++;

	if(video_current_row == SCREEN_HEIGHT) {
		video_scroll();
	}

}

static uint16_t video_get_full_char_from(int row, int col) {
	return SCREEN_START[row * SCREEN_WIDTH + col];
}

void video_print_string(const char * string) {
	while (*string != 0) {
		switch (*string) {
			case '\n':
				video_print_new_line();
				break;

			case '\t':
				video_print_string("    ");
				break;

			default:
				video_printc(*string);
				break;
		}
		string++;
	}
	video_update_cursor();
}

void video_println(const char * string){
	if (video_current_column != 0) {
		video_print_new_line();
	}
	video_print_string(string);
	video_print_new_line();
}

// POSITION
static void video_reset_current_row(){
	video_current_row = 0;
}

static void video_reset_current_column(){
	video_current_column = 0;
}

void video_reset_cursor(){
	video_reset_current_row();
	video_reset_current_column();
}

static void video_scroll(){
	for (int row = 1; row <= SCREEN_HEIGHT; row++) {
		for (int column = 0; column < SCREEN_WIDTH; column++) {
			uint16_t c = video_get_full_char_from(row, column);
			video_print_char_at(c, row - 1, column);
		}

	}
	video_current_row--;
	video_reset_current_column();
	video_update_screen_color();
}

/*
	Moving the Cursor without the BIOS, using video hardware control.
	Reference: http://wiki.osdev.org/Text_Mode_Cursor
*/
void video_update_cursor() {
	unsigned short position = (video_current_row * 80) + video_current_column;
	// cursor LOW port to vga INDEX register
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position & 0xFF));
	// cursor HIGH port to vga INDEX register
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((position >> 8) & 0xFF));
}

void video_trigger_backup() {

	screensaverBackup.row = video_current_row;
	screensaverBackup.column = video_current_column;
	screensaverBackup.color = video_current_color;

	for (int i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH); i++) {
		screensaverBackup.screen[i] = SCREEN_START[i];
	}

	video_current_row = 0;
	video_current_column = 0;
	video_reset_color();

}

void video_trigger_restore() {

	video_current_row = screensaverBackup.row;
	video_current_column = screensaverBackup.column;
	video_current_color = screensaverBackup.color;

	for (int i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH); i++) {
		SCREEN_START[i] = screensaverBackup.screen[i];
	}

	video_update_cursor();
}

void video_trigger_screensaver() {

	video_trigger_backup();
	video_clear_screen();
	video_print_screensaver();
	video_update_screen_color();
}

static void video_print_screensaver(){
	video_set_font_background_color(COLOR_GREEN, COLOR_BLACK);
	video_print_string("agfurt- \t\t sa d k\t mdsa s vod k.pf \t\to  \tsd- \t\t sa d k\t a s vod k.pf \t\t ,lo30\n");
	video_print_string("- \t\t sa d k\t mdsa s vod k.pf  wqe.-,fhpr,w\t\t qwlorem\t  mdqd+'3059\t\n");
	video_print_string("1 furt- \tnn sa d k\t mlpotra s vod k.pf \t\t\t,ofwnwew0dfwe + .wefw0'r2 \t\n");
	video_print_string("o  \tsd- \t\t sa d k\t a s vod k.pf \t soidw oadsm `peto'57 \t 76'7o5`.`q+\t ksdas\t\n");
	
	video_print_string("\t\t\t\t\t\t ======================================\n");
	video_print_string("\n");
	video_print_string("k.pf \t soidw oadsm \t 53372 - Biancucci, Christian.\t\tmlpotra s nnd k\n");
	video_print_string("peto'57 \t 76'7o5\t\t53344 - Gualino, Matias.\t\t\t  orem\t  mdqd\n");
	video_print_string("\t\tofwnwew0dfwe + \t52477 - Rizzotto, Damian.\t\t\tod k.pf  wq\n");
	video_print_string("\n");
	video_print_string("\t\t\t\t\t\t ======================================\n");
	
	video_print_string("agfurt- \t\t sa d k\t mdsa s vod k.pf \t\to  \tsd- \t\t sa d k\t a s vod k.pf \t\t ,lo30\n");
	video_print_string("- \t\t sa d k\t mdsa s vod k.pf  wqe.-,fhpr,w\t\t qwlorem\t  mdqd+'3059\t\n");
	video_print_string("1 furt- \tnn sa d k\t mlpotra s vod k.pf \t\t\t,ofwnwew0dfwe + .wefw0'r2 \t\n");
	video_print_string("o  \tsd- \t\t sa d k\t a s vod k.pf \t soidw oadsm `peto'57 \t 76'7o5`.`q+\t ksdas\t\n");
}
