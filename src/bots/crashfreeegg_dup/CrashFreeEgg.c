/*
    Pokemon Sword/Shield bots using Arduino
    Copyright (C) 2020  brianuuuSonic / codlab

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

#include "CrashFreeEgg.h"

/*------------------------------------------*/
// INSTRUCTION
// -> Nothing
/*------------------------------------------*/

static const Command PROGMEM setup[] = {
	STEP_NOTHING(200),
	STEP_TRIGGERS(5, 150),
	STEP_TRIGGERS(5, 150),
	STEP_B(5, 200),
};

static const Command PROGMEM wake_up_hang[] = {
	STEP_TRIGGERS(5, 250)
};

static const Command PROGMEM move[] = {
	STEP_CIRCLE(200, 1), //in the original script the NOTHIN step did not exist
	STEP_B(1, 1),
	STEP_CIRCLE(200, 1), //in the original script the NOTHIN step did not exist
	STEP_B(1, 1),
	STEP_CIRCLE(200, 1), //in the original script the NOTHIN step did not exist
};


static const Command PROGMEM go_to_lady[] = {
	STEP_X(2, 50),
	STEP_LEFT(100, 1),
	STEP_DOWN(100, 1),
	STEP_A(2, 90),
	STEP_A(2, 42),
	STEP_A(2, 90),
	STEP_L(2, 20),
	STEP_PLUS(2, 1),
	STEP_LEFT(40, 1),
	STEP_DOWN(35, 20)
};

static const Command PROGMEM talk_to_lady[] = {
	STEP_A(2, 50),
	STEP_A(2, 50),
	STEP_B(2, 50),
	STEP_B(2, 50),
	STEP_A(2, 100),
	STEP_A(2, 50),
	STEP_B(2, 75),
	STEP_A(2, 100),
	STEP_B(2, 50),
	STEP_B(2, 75),
	STEP_B(2, 50),
	STEP_B(2, 25)
};

static const Command PROGMEM go_to_position[] = {
	STEP_PLUS(2, 20),
	STEP_RIGHT(50, 20),
	STEP_UP(50, 1), //in the original script the nothing step did not exist
	STEP_UP(50, 1), //in the original script the nothing step did not exist
	STEP_UP(50, 25)
};

#define LADY PROCESS_CUSTOM_1
#define TALK PROCESS_CUSTOM_2
#define POSITION PROCESS_CUSTOM_3
#define MOVE PROCESS_CUSTOM_4
#define INCR PROCESS_CUSTOM_5

bool has_finish_one_round = false;

// Prepare the next report for the duplication to occur
Command* crashFreeEgg(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->bot = CrashFreeEggDup;
			RETURN_NEW_SEQ(setup, LADY);
		case LADY:
			if(has_finish_one_round) (context->botSteps)++;
			RETURN_NEW_SEQ(go_to_lady, TALK);
		case TALK:
			RETURN_NEW_SEQ(talk_to_lady, POSITION);
		case POSITION:
			RETURN_NEW_SEQ(go_to_position, MOVE);
		case MOVE:
			RETURN_NEW_SEQ(move, INCR);
		case INCR:
			has_finish_one_round = true;
			RETURN_NULL_SEQ(LADY);
		case DONE: default: return nullptr;
	}
	return nullptr;
}