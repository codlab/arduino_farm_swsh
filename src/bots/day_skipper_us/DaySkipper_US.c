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

#include "DaySkipper_US.h"
#include "config_preprocess.h"
#include "config.h"

static const Command PROGMEM sequences[] = {
	//----------Setup [0,8]----------
	// Press A once to connect
	{NOTHING, 30},
	{A, 1},
	{NOTHING, 1},
	
	// Make sure cursor is on OK
	{A, 1},
	{NOTHING, 1},
	{RIGHT, 40},
	{NOTHING, 1},
	
	// Exit
	{A, 1},
	{NOTHING, 4},
	
	//----------Skip day [9,34]----------
	// Enter
	{A, 1},
	{NOTHING, 5},
	
	// Move to day
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	
	// Increment day
	{UP, 1},
	{NOTHING, 1},
	
	// Move to OK
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},

	// Exit
	{A, 1},
	{NOTHING, 4},
	
	//----------Skip month [35,66]----------
	// Enter
	{A, 1},
	{NOTHING, 5},
	
	// Move to day
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	
	// Set day back to "01"
	{UP, 1},
	{NOTHING, 1},
	
	// Move to month
	{LEFT, 1},
	{NOTHING, 1},

	// Increment month
	{UP, 1},
	{NOTHING, 1},
	
	// Move to OK
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},

	// Exit
	{A, 1},
	{NOTHING, 4},
	
	//----------Skip year [67,100]----------
	// Enter
	{A, 1},
	{NOTHING, 5},
	
	// Move to day
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	
	// Set day back to "01"
	{UP, 1},
	{NOTHING, 1},
	
	// Move to month
	{LEFT, 1},
	{NOTHING, 1},

	// Set month back to "01"
	{UP, 1},
	{NOTHING, 1},
	
	// Move to year
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},

	// Increment year
	{UP, 1},
	{NOTHING, 1},
	
	// Move to OK
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},

	// Exit
	{A, 1},
	{NOTHING, 4},
	
	//----------Back to game [101,104]----------
	{HOME, 1},
	{NOTHING, 30},
	{HOME, 1},
	{NOTHING, 20}
};

// Prepare the next report for the host.
Command* daySkipperUS(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->bot = DaySkipperUS;
			context->commandIndex = 0;
			context->endIndex = 8;
			context->next_state = PROCESS_CUSTOM_1;
			return nullptr;
		case PROCESS_CUSTOM_1:
			// Get the next command sequence (new start and end)
			if (calendarUS.dayToSkip > 0) {
				// Day = 0, Month = 1, Year = 2
				int passDayMonthYear = 0;
				
				if (calendarUS.month == 2) {
					bool isLeapYear = (calendarUS.year % 4 == 0);
					if (isLeapYear && calendarUS.day == 29) {
						passDayMonthYear = 1;
					} else if (!isLeapYear && calendarUS.day == 28) {
						passDayMonthYear = 1;
					}
				} else if (calendarUS.month == 12 && calendarUS.day == 31) {
					passDayMonthYear = 2;
				} else if (calendarUS.month == 4 || calendarUS.month == 6 || calendarUS.month == 9 || calendarUS.month == 11) {
					if (calendarUS.day == 30) {
						passDayMonthYear = 1;
					}
				} else { //if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10)
					if (calendarUS.day == 31) {
						passDayMonthYear = 1;
					}
				}
				
				if (passDayMonthYear == 0) {
					// Pass day
					calendarUS.day++;
					context->commandIndex = 9;
					context->endIndex = 34;
				} else if (passDayMonthYear == 1) {
					// Pass month
					calendarUS.day = 1;
					calendarUS.month++;
					context->commandIndex = 35;
					context->endIndex = 66;
				} else {
					// Pass year
					calendarUS.day = 1;
					calendarUS.month = 1;
					calendarUS.year++;
					context->commandIndex = 67;
					context->endIndex = 100;
				}
			} else if (calendarUS.dayToSkip == 0) {
				// Go back to game
				context->commandIndex = 101;
				context->endIndex = 104;
			} else {
				// Finish
				context->next_state = DONE;
				return nullptr;
			}
			
			calendarUS.dayToSkip--;

			return &sequences;
		case DONE: return nullptr;
	}
	return nullptr;
}