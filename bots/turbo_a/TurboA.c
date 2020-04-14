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

#include "TurboA.h"

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
	
static const Command PROGMEM gggoooooo[] = {
	STEP_A(1, 1)
};

// Prepare the next report for the host.
Command* turboA(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->bot = TurboA;
			RETURN_NEW_SEQ(setup, PROCESS_CUSTOM_1);
		case PROCESS_CUSTOM_1:
			RETURN_NEW_SEQ(gggoooooo, PROCESS_CUSTOM_1);
		case DONE: default: return nullptr;
	}
	return nullptr;
}