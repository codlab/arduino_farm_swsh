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

#include "DaySkipper_JP_NoLimit.h"
#include "config_preprocess.h"
#include "config.h"

static const Command PROGMEM increment_day[] = {
	//----------Skip day [9,26]----------
	// Enter
	STEP_A(1, 4),
	
	// Move to day
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	
	// Increment day
	STEP_UP(1, 1),
	
	// Move to OK
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),

	// Exit
	STEP_A(1, 4)
};

void jp_nolimit_set(const char* buffer) {
	jp_daySkip.dayToSkip = to_ulong(buffer);
}

void configureDaySkipperJPNoLimit(Context *context) {
	context->set = jp_nolimit_set;
}

// Prepare the next report for the host.
Command* daySkipperJPNoLimit(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	return daySkipperNoLimit(DaySkipperJPNoLimit, &jp_daySkip, context, ReportData, increment_day);
}