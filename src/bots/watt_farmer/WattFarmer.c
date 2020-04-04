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

#include "WattFarmer.h"
#include "../../core/serial_report.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> You MUST have set text speed to FAST
// -> You MUST stand in front of a wishing piece den
// -> There is a very small chance that the game will crash, the program will save every 50 tries
// -> Note that if today's date is end of month for EU, or December for US, first loop will fail, this is normal
// -> It takes ~10 seconds per watt collection
/*------------------------------------------*/

static const Command PROGMEM init_sequence[] = {
	STEP_NOTHING(100),
	STEP_B(1, 50),
};

static const Command PROGMEM settings_sequence[] = {
	STEP_HOME(1, 30),
	STEP_DOWN(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_A(1, 10),
	
	// To Date and Time
	STEP_DOWN(80, 1),
	STEP_A(1, 1),
	STEP_DOWN(1, 1),
	STEP_DOWN(1, 1),
	STEP_DOWN(1, 1),
	STEP_DOWN(1, 1),
	STEP_A(1, 8),
	
	// Sync and unsync time
	STEP_A(1, 4),
	STEP_A(1, 8),
	
	// To actually Date and Time
	STEP_DOWN(1, 1),
	STEP_DOWN(1, 1),
	STEP_A(1, 7),
	
	// Plus one
	STEP_UP(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_A(1, 4),
	
	// Back to game
	STEP_HOME(1, 30),
	STEP_HOME(1, 30)
};

static const Command PROGMEM collect_sequence[] = {
	STEP_A(12, 1),
	STEP_B(6, 1),
	STEP_B(30, 1),
	STEP_B(1, 50),
};

static const Command PROGMEM save_sequence[] = {
	STEP_X(1, 20),
	STEP_R(1, 50),
	STEP_A(1, 140)
};

// Saving
int m_saveCount = 0;
int m_saveAt = 50;

#define PROCESS_COLLECT PROCESS_CUSTOM_1
#define PROCESS_SAVE PROCESS_CUSTOM_2
#define PROCESS_CHECK_SAVE PROCESS_CUSTOM_3

static unsigned long watt_farmer_round = 0;

// Prepare the next report for the host.
Command* wattFarmer(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->bot = WattFarmer;
			RETURN_NEW_SEQ(init_sequence, PROCESS_SETTINGS);

		case PROCESS_SETTINGS:
			RETURN_NEW_SEQ(settings_sequence, PROCESS_COLLECT);

		case PROCESS_COLLECT:
			reportStep(watt_farmer_round);
			watt_farmer_round ++;

			RETURN_NEW_SEQ(collect_sequence, PROCESS_CHECK_SAVE);

		case PROCESS_SAVE:
			RETURN_NEW_SEQ(save_sequence, PROCESS_SETTINGS);

		case PROCESS_CHECK_SAVE:
			// Get the next command sequence (new start and end)
			if(m_saveCount < m_saveAt) {
				m_saveCount++;
				context->next_state = PROCESS_SETTINGS;
			} else {
				m_saveCount = 0;
				context->next_state = PROCESS_SAVE;
			}

		case DONE:
		default: { }
	}
	return nullptr;
}