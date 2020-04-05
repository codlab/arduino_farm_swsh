#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED

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

typedef struct Context {
	State_t state;
	State_t next_state;
	int commandIndex;
	int endIndex;
	int durationCount;
	Bot bot;
    void (*set)(const char*);
} Context;

void set(const char* set);

#endif