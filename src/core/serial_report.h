#ifndef SERIAL_REPORT_INCLUDED
#define SERIAL_REPORT_INCLUDED

typedef enum Bot {
	MissingNo, //a
	Auto3DaysSkipper, //b
	AutoFossil, //c
	AutoHost, //d
	AutoLoto, //e
	BerryFarmer, //f
	BoxRelease, //g
	CrashFreeEggDup, //h
	DaySKipperEU, //i
	DaySKipperEUNoLimit, //j
	DaySKipperJP, //k
	DaySKipperJPNoLimit, //l
	DaySKipperUS, //m
	DaySKipperUSNoLimit, //n
	TurboA, //o
	WattFarmer //p
} Bot;

void reportBot(Bot bot);

/**
 * Initialize the USART communication
 * default is a 9600 baudrate
 */
void reportInit(void);

/**
 * reportStep onto the USART TX
 */
void reportStep(unsigned long round);

#endif