#include <realTimeClock.h>
#include <io.h>

#define bcdToInt(bcd) ((((bcd) / 16) * 10) + ((bcd) & 0xF))
#define intToBcd(in) 	((((in) / 10) * 16) | (in % 10))

#define isBcd(regb)	((regb & 0x4) == 0)
#define twelveFormatClock(regb) ((regb & 0x02) == 0)
#define toPM(h)	(0x80 | (h))
#define isPM(h)	(0x80 & (h))

static uint8_t RTCRead(uint8_t reg);
static void RTCWrite(uint8_t reg, uint8_t val);
static uint8_t RTCUpdate();

void getTime(time_t* t) {

	uint8_t regb;

	regb = inb(0xB);

	while (RTCUpdate());

	t->hour = RTCRead(HOUR);
	t->minute = RTCRead(MINUTE);
	t->second = RTCRead(SECOND);

	if (isBcd(regb)) {
		t->hour = bcdToInt(t->hour);
		t->minute = bcdToInt(t->minute);
		t->second = bcdToInt(t->second);
	}

	if (twelveFormatClock(regb) && isPM(t->hour)) {
		// turn off pm. Add 12 hours. % 24 for midnight
		t->hour = ((t->hour & 0x7F) + 12) % 24;
	}
}

void setTime(time_t* t) {

	uint8_t regb;
	regb = inb(0xB);

	while (RTCUpdate());

	if (!twelveFormatClock(regb) && t->hour > 12) {
		t->hour = toPM(t->hour - 12);
	}

	if (isBcd(regb)) {
		t->hour = intToBcd(t->hour);
		t->minute = intToBcd(t->minute);
		t->second = intToBcd(t->second);
	}

	RTCWrite(SECOND, t->second);
	RTCWrite(MINUTE, t->minute);
	RTCWrite(HOUR, t->hour);

}

static uint8_t RTCUpdate() {
	return (RTCRead(0xA) & 0x8);
}

// The following functions uses io.asm rutines for read and write the values for RTC registers

static uint8_t RTCRead(uint8_t reg) {
	outb(PORT_POS, reg);
	return inb(PORT_VALUE);
}

static void RTCWrite(uint8_t reg, uint8_t val) {
	outb(PORT_POS, reg);
	outb(PORT_VALUE, val);
}
