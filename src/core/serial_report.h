#ifndef SERIAL_REPORT_INCLUDED
#define SERIAL_REPORT_INCLUDED

#include "usb_device.h"

typedef enum BotState {
	ON,
	PAUSE, //TODO add PAUSE
	OFF
} BotState;

BotState currentBotState(void);

void fechCurrentMillis(void);
void checkReceived(Context* context);
void checkSend(Context* context);

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