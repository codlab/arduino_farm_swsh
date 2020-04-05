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

#include "DaySkipper_EU_NoLimit.h"
#include "config_preprocess.h"
#include "config.h"
#include "../../core/calendar_skip.h"

static const Command PROGMEM sequences[] = {
	// Press A once to connect
	STEP_NOTHING(30),
	STEP_B(1, 30),
	STEP_B(1, 30),
	// Make sure cursor is on OK
	STEP_A(1, 1),
	STEP_RIGHT(40, 1),
	
	// Exit
	STEP_A(1, 4)
};

static const Command PROGMEM increment_day[] = {
	// Enter
	STEP_A(1, 4),
	
	// Move to day
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	
	// Increment day
	STEP_UP(1, 1),
	
	// Move to OK
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),

	// Exit
	STEP_A(1, 4)
};

static const Command PROGMEM exit[] = {
	STEP_HOME(1, 30),
	STEP_HOME(1, 30)
};

unsigned long day_to_skip_eu = 1;

void eu_nolimit_set(const char* buffer) {
	eu_dayToSkip = to_ulong(buffer);
}

void configureDaySkipperEUNoLimit(Context *context) {
	context->set = eu_nolimit_set;
}

// Prepare the next report for the host.
Command* daySkipperEUNoLimit(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS: {
			unsigned long months = eu_dayToSkip >= 30 ? eu_dayToSkip/30 : 0;
			unsigned long months_t_30 = months * 30;

			int mod = eu_dayToSkip - months_t_30;
			day_to_skip_eu = months * 31 + mod;

			context->bot = DaySkipperEUNoLimit;
			RETURN_NEW_SEQ(sequences, PROCESS_CUSTOM_1);
		}
		case PROCESS_CUSTOM_1:
			if(day_to_skip_eu > 0) {
				day_to_skip_eu--;
				RETURN_NEW_SEQ(increment_day, PROCESS_CUSTOM_1);
			} else {
				RETURN_NEW_SEQ(increment_day, PROCESS_CUSTOM_2);
			}
		case PROCESS_CUSTOM_2:
			RETURN_NEW_SEQ(exit, DONE);
		case DONE: return nullptr;
	}
	return nullptr;
}