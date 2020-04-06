/*
    Pokemon Sword/Shield bots using Arduino
    Copyright (C) 2020 codlab

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


	Contextually based on the LUFA library's Low-Level Joystick Demo
		(C) Dean Camera
	Contextually based on the HORI's Pokken Tournament Pro Pad design
		(C) HORI

	For more information, please see the README.md available at https://github.com/codlab/arduino_farm_swsh
*/

#include "TwitchPlays.h"

/*------------------------------------------*/
// INSTRUCTION
// -> Nothing
/*------------------------------------------*/

static const Command PROGMEM setup[] = {
	// Connect controller in Change Grip/Order
	STEP_NOTHING(30),
	STEP_B(1, 40),
	STEP_B(1, 40)
};

#define TWITCH_PLAYS(TYPE, WAIT_FOR) plays_##TYPE[] = {  {TYPE, WAIT_FOR} }

static const Command PROGMEM TWITCH_PLAYS(A, 1);
static const Command PROGMEM TWITCH_PLAYS(B, 1);
static const Command PROGMEM TWITCH_PLAYS(X, 1);
static const Command PROGMEM TWITCH_PLAYS(Y, 1);
static const Command PROGMEM TWITCH_PLAYS(L, 1);
static const Command PROGMEM TWITCH_PLAYS(R, 1);
static const Command PROGMEM TWITCH_PLAYS(UP, 1);
static const Command PROGMEM TWITCH_PLAYS(DOWN, 1);
static const Command PROGMEM TWITCH_PLAYS(LEFT, 1);
static const Command PROGMEM TWITCH_PLAYS(RIGHT, 1);
static const Command PROGMEM TWITCH_PLAYS(TRIGGERS, 1);
static const Command PROGMEM TWITCH_PLAYS(CIRCLE, 1);
static const Command PROGMEM TWITCH_PLAYS(HOME, 1);
static const Command PROGMEM TWITCH_PLAYS(PLUS, 1);
static const Command PROGMEM TWITCH_PLAYS(MINUS, 1);

int eq(const char* left, const char* right) {
    int index = 0;
	if(strlen(left) != strlen(right)) return false;

    while(index < strlen(left) && index < strlen(right)) {
        if(left[index] != right[index]) return false;
        index++;
    }
    return true;
}
Command* _next = nullptr;

void twitchPlaysSet(const char* str) {
	if(eq("A", str)) _next = (Command*) plays_A;
	else if(eq("B", str)) _next = (Command*) plays_B;
	else if(eq("X", str)) _next = (Command*) plays_X;
	else if(eq("Y", str)) _next = (Command*) plays_Y;
	else if(eq("L", str)) _next = (Command*) plays_L;
	else if(eq("R", str)) _next = (Command*) plays_R;
	else if(eq("UP", str)) _next = (Command*) plays_UP;
	else if(eq("DOWN", str)) _next = (Command*) plays_DOWN;
	else if(eq("LEFT", str)) _next = (Command*) plays_LEFT;
	else if(eq("RIGHT", str)) _next = (Command*) plays_RIGHT;
	else if(eq("TRIGGERS", str)) _next = (Command*) plays_TRIGGERS;
	else if(eq("CIRCLE", str)) _next = (Command*) plays_CIRCLE;
	else if(eq("HOME", str)) _next = (Command*) plays_HOME;
	else if(eq("PLUS", str)) _next = (Command*) plays_PLUS;
	else if(eq("MINUS", str)) _next = (Command*) plays_MINUS;
}

// Prepare the next report for the host.
Command* twitchPlays(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->bot = TurboA;
			context->set = twitchPlaysSet;
			RETURN_NEW_SEQ(setup, PROCESS_CUSTOM_1);
		case PROCESS_CUSTOM_1: {
			Command* ptr = _next;
			_next = nullptr;
			RETURN_NEW_SEQ(ptr, PROCESS_CUSTOM_1);
		}
		case DONE: default: return nullptr;
	}
	return nullptr;
}