#include <LUFA/Drivers/Peripheral/Serial.h>
#include "serial_report.h"

static char buffer[50];
static Bot current_bot;
static BotState current_bot_state = ON;
static unsigned long current_bot_round = 0;

#define RECV_SIZE 50
static char recv_buffer[RECV_SIZE + 1];
static int recv_buffer_index = 0;
static int recv_buffer_first_at = -1;

void reportInit(void) {
    Serial_Init(9600, false);
}

void prepareBuffer(void) {
	int i = 0;
	for(; i < sizeof(buffer); i++) buffer[i] = ' ';
	buffer[sizeof(buffer) - 2] = '0';
	buffer[sizeof(buffer) - 1] = 0;
}

BotState currentBotState() {
    return current_bot_state;
}

void reportBot(Bot bot) {
    current_bot = bot;
}

Bot current() {
    return current_bot;
}

void reportStep(unsigned long round) {
    current_bot_round = round;
    reportTrySendState();
}


int equals(const char * str) {
    int index = 0;
    while(index < RECV_SIZE && index < strlen(str)) {
        if(recv_buffer[index] != str[index]) return false;
        index++;
    }
    return true;
}

void checkReceived() {
    int16_t b = Serial_ReceiveByte();
    if(-1 != b && recv_buffer_index < RECV_SIZE) {
        if(recv_buffer_first_at == -1) {
            recv_buffer_first_at = millis();
        }
        recv_buffer[recv_buffer_index] = (b & 0xff);
        recv_buffer_index ++;
        recv_buffer[recv_buffer_index] = 0; //nullptr
    }

    if(recv_buffer_first_at > -1) {
        unsigned long diff = millis() - recv_buffer_first_at;
        if(diff >= 1000) {
            if(equals("PAUSE")) {
                current_bot_state = PAUSE;
            } else if(equals("ON")) {
                current_bot_state = ON;
            } else if(equals("OFF")) {
                current_bot_state = OFF;
            }

            //reset buffer
            recv_buffer_first_at = -1;
            recv_buffer_index = 0;
        }
    }
}

void reportTrySendState() {
	unsigned long tmp = current_bot_round;
	int index = sizeof(buffer) - 2;

	prepareBuffer();

	while(tmp > 0 && index >= 0) {
		int digit = tmp % 10;
		buffer[index] = '0' + digit;
		tmp /= 10;
		index --;
	}

    if(index > 1) {
        index --; //we let one ' '
        buffer[index] = '0' + current_bot_state;
        index --;
    }

    if(index > 1) {
        index --; //we let one ' '
        buffer[index] = 'a' + current_bot;
        index --;
    }

    //TODO make the Serial_SendByte at the position of the first non space character
	index = 0;
    for(; index < sizeof(buffer) - 2 && buffer[index] == ' ' ; index++) { };

	while(index < sizeof(buffer)) {
		if(0 != buffer[index]) Serial_SendByte(buffer[index]);
		index ++;
	}
}

