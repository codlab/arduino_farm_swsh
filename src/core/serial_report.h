#ifndef SERIAL_REPORT_INCLUDED
#define SERIAL_REPORT_INCLUDED

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