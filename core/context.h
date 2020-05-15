#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED

#include "../Joystick.h"
#include "bot.h"

typedef enum {
	PROCESS,
	PROCESS_SETTINGS, //name to facilitate tracking specific settings actions
	PROCESS_CUSTOM_1,
	PROCESS_CUSTOM_2,
	PROCESS_CUSTOM_3,
	PROCESS_CUSTOM_4,
	PROCESS_CUSTOM_5,
	PROCESS_CUSTOM_6,
	PROCESS_CUSTOM_7,
	PROCESS_CUSTOM_8,
	PROCESS_CUSTOM_9,
	DONE
} State_t;

struct Context;


typedef Command* (*NextStep)(struct Context* context, USB_JoystickReport_Input_t* const ReportData);
typedef void (*Set)(const char*);

typedef struct Context {
	State_t state;
	State_t next_state;
	int commandIndex;
	int endIndex;
	int durationCount;
	Bot bot;
    BotState botState;
	unsigned long botSteps;

    NextStep next_step;
    Set set;

	int echo;
	int ECHOES;
} Context;

void configure(Context* context);

void set(const char* set);

#endif