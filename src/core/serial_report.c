#include <LUFA/Drivers/Peripheral/Serial.h>
#include "serial_report.h"

static char buffer[12];

void reportInit(void) {
    Serial_Init(9600, false);
}

void prepareBuffer(void) {
	int i = 0;
	for(; i < sizeof(buffer); i++) buffer[i] = ' ';
	buffer[sizeof(buffer) - 2] = '0';
	buffer[sizeof(buffer) - 1] = 0;
}

void reportStep(unsigned long round) {
	unsigned long tmp = round;
	prepareBuffer();

	int index = sizeof(buffer) - 2;
	while(tmp > 0 && index >= 0) {
		int digit = tmp % 10;
		buffer[index] = '0' + digit;
		tmp /= 10;
		index--;
	}
    //TODO make the Serial_SendByte at the position of the first non space character
	index = 0;
	while(index < sizeof(buffer)) {
		if(0 != buffer[index]) Serial_SendByte(buffer[index]);
		index++;
	}
}

