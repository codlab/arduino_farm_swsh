#ifndef SERIAL_REPORT_INCLUDED
#define SERIAL_REPORT_INCLUDED

#include "usb_device.h"

void fechCurrentMillis(void);
void checkReceived(Context* context);
void checkSend(Context* context);

/**
 * Initialize the USART communication
 * default is a 9600 baudrate
 */
void reportInit(void);

#endif