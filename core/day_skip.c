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

#include "day_skip.h"
#include "lang.h"

static const Command PROGMEM sequences[] = {
	// Press A once to connect
	STEP_NOTHING(90),
	STEP_X(3, 90),
	STEP_X(3, 90),
	// Make sure cursor is on OK
	STEP_X(3, 1),
	STEP_RIGHT(120, 3),
	
	// Exit
	STEP_RIGHT(120, 3),
};

static const Command PROGMEM exit[] = {
	STEP_HOME(3, 90),
	STEP_HOME(3, 90)
};

static const Command PROGMEM increment_day_us[] = {
	// Enter
	STEP_A(5, 14),
	
	// Move to day
	STEP(JOYSTICK_1_LEFT, 4),
	STEP(JOYSTICK_2_LEFT, 4),
	STEP(JOYSTICK_1_LEFT, 4),
	STEP(JOYSTICK_2_LEFT, 4),

	// Increment day
	STEP_UP_A(4, 2),

	STEP(JOYSTICK_2_RIGHT, 4),
	STEP(JOYSTICK_1_RIGHT, 4),
	STEP(JOYSTICK_2_LEFT, 4),
	STEP(JOYSTICK_1_RIGHT, 4),

	STEP_DOUBLE_RIGHT(5, 14), //it has an A to validate
};

static const Command PROGMEM increment_day_jp[] = {
	// Enter
	STEP_A(4, 14),

	// Move to day
	STEP(JOYSTICK_1_LEFT, 4),
	STEP(JOYSTICK_2_LEFT, 4),
	STEP(JOYSTICK_1_LEFT, 4),

	// Increment day
	STEP_UP_A(4, 2),

	STEP(JOYSTICK_2_RIGHT, 4),

	// Exit
	STEP_DOUBLE_RIGHT(5, 14)
};

static const Command PROGMEM increment_day_eu[] = {
	// Enter
	STEP_A(5, 14),
	
	// Move to day
	STEP(JOYSTICK_1_LEFT, 4),
	STEP(JOYSTICK_2_LEFT, 4),
	STEP(JOYSTICK_1_LEFT, 4),
	STEP(JOYSTICK_2_LEFT, 4),
	STEP(JOYSTICK_1_LEFT, 4),
	
	// Increment day
	STEP_UP_A(4, 2),

	STEP(JOYSTICK_2_RIGHT, 4),
	STEP(JOYSTICK_1_RIGHT, 4),
	STEP(JOYSTICK_2_LEFT, 4),

	STEP_DOUBLE_RIGHT(5, 14), //it has an A to validate
};

//local value to hold the calculated number of skips to do
static unsigned long day_to_skip = 0;

// Prepare the next report for the host.
Command* daySkipperNoLimit(Bot mode, unsigned long total_skips, Context* context, USB_JoystickReport_Input_t* const ReportData, Region region) {
	// States and moves management
	switch (context->state) {
		case PROCESS: {

			unsigned long months = total_skips / 30;
			unsigned long months_t_30 = months * 30;

			int mod = total_skips - months_t_30;
			day_to_skip = months * 31 + mod;

			context->bot = mode;

			//override the number of ECHOES
			context->ECHOES = 0;

			RETURN_NEW_SEQ(sequences, PROCESS_CUSTOM_1);
		}
		case PROCESS_CUSTOM_1:
			if(day_to_skip > 0) {
				(context->botSteps) ++;
				day_to_skip--;
				switch(region) {
					case EU: { RETURN_NEW_SEQ(increment_day_eu, PROCESS_CUSTOM_1); }
					case US: { RETURN_NEW_SEQ(increment_day_us, PROCESS_CUSTOM_1); }
					default: { RETURN_NEW_SEQ(increment_day_jp, PROCESS_CUSTOM_1); }
				}
			} else {
				switch(region) {
					case EU: { RETURN_NEW_SEQ(increment_day_eu, PROCESS_CUSTOM_2); }
					case US: { RETURN_NEW_SEQ(increment_day_us, PROCESS_CUSTOM_2); }
					default: { RETURN_NEW_SEQ(increment_day_jp, PROCESS_CUSTOM_2); }
				}
			}
		case PROCESS_CUSTOM_2:
			RETURN_NEW_SEQ(exit, DONE);
		case DONE: return nullptr;
	}
	return nullptr;
}