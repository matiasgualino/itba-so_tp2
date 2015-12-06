#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <syscalls.h>
#include <video.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static uint64_t pitTimer = 0;
uint64_t screensaverWaitTime = 10;
uint64_t screensaverTimer = 0;
bool screensaverActive = FALSE;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{	
	video_init();

	screensaverResetTimer();

	video_set_font_background_color(4, 0);
                                                                               
	video_print_string("\t\t\t\t\t   _____ ___    ____  ___   _____ ___ \n");
	video_print_string("\t\t\t\t\t  / ___//   |  / __ \\/   | / ___//   |\n");
	video_print_string("\t\t\t\t\t  \\__ \\/ /| | / /_/ / /| | \\__ \\/ /| |\n");
	video_print_string("\t\t\t\t\t ___/ / ___ |/ _, _/ ___ |___/ / ___ |\n");
	video_print_string("\t\t\t\t\t/____/_/  |_/_/ |_/_/  |_/____/_/  |_|\n");
	
	video_set_font_background_color(15, 0);
	video_println(" ");
	video_print_string("\t\tMarcelo uso la primera computadora (y tambien nuestro Kernel)\n");

	((EntryPoint)sampleCodeModuleAddress)();
	return 0;
}

void irq0_handler() {

	pitTimer++;
	screensaverTimer--;

	if (screensaverTimer == 0 && !screensaverActive) {
		activeScreensaver();
	}

}

bool screensaverResetTimer() {
	bool ret = FALSE;
	if (screensaverActive) {
		ret = TRUE;
		screensaverActive = FALSE;
		video_trigger_restore();
	}
	screensaverTimer = 18 * screensaverWaitTime;
	return ret;
}

void activeScreensaver() {
	screensaverActive = TRUE;
	video_trigger_screensaver();
}
