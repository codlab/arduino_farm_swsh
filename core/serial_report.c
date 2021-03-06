#include <LUFA/Drivers/Peripheral/Serial.h>
#include "context.h"
#include "serial_report.h"
#include "millis.h"

//#define DEBUG true

static char buffer[25];
static BotState current_bot_state = ON;

#define RECV_SIZE 25
static char recv_buffer[RECV_SIZE + 1];
static int recv_buffer_index = 0;
static unsigned long recv_buffer_first_at = 0;

static unsigned long last_send_frame = 0;

static unsigned long current_millis = 0;
void fechCurrentMillis(void) {
    current_millis = millis();
}

void reportInit(void) {
    Serial_Init(9600, false);
}

void prepareBuffer(void) {
	int i = 0;
	for(; i < sizeof(buffer); i++) buffer[i] = ' ';
	buffer[sizeof(buffer) - 2] = '0';
	buffer[sizeof(buffer) - 1] = 0;
}

int equals(const char * str) {
    int index = 0;
    while(index < RECV_SIZE && index < strlen(str)) {
        if(recv_buffer[index] != str[index]) return false;
        index++;
    }
    return true;
}

#define SET_NUMBER_IN_BUFFER(NUMBER, BUFFER, INDEX) \
	while(NUMBER > 0 && INDEX >= 0) { \
		BUFFER[INDEX] = '0' + (NUMBER % 10); \
		NUMBER /= 10; \
		INDEX --; \
	}

void reportTrySendState(Context* context) {
    
	unsigned long tmp = 0;
	int index = sizeof(buffer) - 2;

    #ifdef DEBUG
    for(index = 0; index < recv_buffer_index ; index ++) {
        Serial_SendByte(recv_buffer[index]);
    }
    Serial_SendByte(equals("PAUSE") ? 0x99 : 0x66);
    #else
	prepareBuffer();

    tmp = current_millis/1000; //get the number of seconds
    SET_NUMBER_IN_BUFFER(tmp, buffer, index);

    if(index > 1) {
        index --; //we let one ' '

        tmp = context->botSteps;
        SET_NUMBER_IN_BUFFER(tmp, buffer, index);
    }

    if(index > 1) {
        index --; //we let one ' '
        buffer[index] = '0' + current_bot_state;
        index --;
    }

    if(index > 1) {
        index --; //we let one ' '
        if(context->bot >= 'A' && context->bot <= 'Z') {
            buffer[index] = 'A' + context->bot;
        } else {
            buffer[index] = 'a' + context->bot;
        }
        index --;
    }

    //TODO make the Serial_SendByte at the position of the first non space character
	index = 0;
    for(; index < sizeof(buffer) - 2 && buffer[index] == ' ' ; index++) { };

	while(index < sizeof(buffer)) {
		if(0 != buffer[index]) Serial_SendByte(buffer[index]);
		index ++;
	}
    #endif
}

void checkSend(Context* context) {
    if(last_send_frame == 0) {
        last_send_frame = current_millis;
    }

    unsigned long diff = current_millis - last_send_frame;

    if(diff >= 1000) {
        reportTrySendState(context);
        last_send_frame = current_millis;
    }
}

void resetContext(Context* context) {
	context->state = PROCESS;
	context->next_state = PROCESS;
	context->state = PROCESS;
	context->commandIndex = 0;
	context->endIndex = 0;
	context->durationCount = 0;
    context->set = nullptr;
    context->next_step = nullptr;
    context->botSteps = 0;
}

void checkReceived(Context* context) {
    int16_t b = Serial_ReceiveByte();
    if(-1 != b && recv_buffer_index < RECV_SIZE) {
        if(recv_buffer_first_at == 0) {
            recv_buffer_first_at = current_millis;
        }
        recv_buffer[recv_buffer_index] = (b & 0xff);
        recv_buffer_index ++;
        recv_buffer[recv_buffer_index] = 0; //nullptr
    }

    if(recv_buffer_first_at > 0) {
        unsigned long diff = current_millis - recv_buffer_first_at;
        if(diff >= 60) {
            if(equals("PAUSE")) {
                context->botState = PAUSE;
            } else if(equals("ON")) {
                context->botState = ON;
            } else if(equals("OFF")) {
                context->botState = OFF;
            } else if(equals("BOT=") && recv_buffer_index >= 4) {
                int new_bot = recv_buffer[4] - 'a';
                if(new_bot >= 0 && new_bot <= WattFarmer) {
                    resetContext(context);
                    context->bot = new_bot;
                    configure(context);
                }
            } else if(equals("SET=") && recv_buffer_index >= 4) {
                if(nullptr != context->set) {
                    const char* set = (const char*) &(recv_buffer[4]);
                    context->set(set);
                }
            } else if(equals("RESET")) {
                resetContext(context);
            }

            //reset buffer
            recv_buffer_first_at = 0;
            recv_buffer_index = 0;
        }
    }
}
