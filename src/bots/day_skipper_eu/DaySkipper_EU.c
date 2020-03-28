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

#include "DaySkipper_EU.h"
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
	
	//----------Skip month [35,62]----------
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
	{RIGHT, 1},
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

	// Exit
	{A, 1},
	{NOTHING, 4},
	
	//----------Skip year [63,92]----------
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
	{RIGHT, 1},
	{NOTHING, 1},

	// Set month back to "01"
	{UP, 1},
	{NOTHING, 1},
	
	// Move to year
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

	// Exit
	{A, 1},
	{NOTHING, 4},
	
	//----------Back to game [93,96]----------
	{HOME, 1},
	{NOTHING, 30},
	{HOME, 1},
	{NOTHING, 20}
};

// Prepare the next report for the host.
Command* daySkipperEU(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->commandIndex = 0;
			context->endIndex = 8;
			context->next_state = PROCESS_CUSTOM_1;
			return nullptr;
		case PROCESS_CUSTOM_1:
			// Get the next command sequence (new start and end)
			if (calendarEU.dayToSkip > 0) {
				// Day = 0, Month = 1, Year = 2
				int passDayMonthYear = 0;
				
				if (calendarEU.month == 2) {
					bool isLeapYear = ((calendarEU.year) % 4 == 0);
					if (isLeapYear && (calendarEU.day) == 29)
					{
						passDayMonthYear = 1;
					}
					else if (!isLeapYear && (calendarEU.day) == 28)
					{
						passDayMonthYear = 1;
					}
				} else if ((calendarEU.month) == 12 && (calendarEU.day) == 31) {
					passDayMonthYear = 2;
				} else if ((calendarEU.month) == 4 || (calendarEU.month) == 6 || (calendarEU.month) == 9 || (calendarEU.month) == 11) {
					if ((calendarEU.day) == 30) {
						passDayMonthYear = 1;
					}
				} else { //if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10)
					if ((calendarEU.day) == 31) {
						passDayMonthYear = 1;
					}
				}
				
				if (passDayMonthYear == 0) {
					// Pass day
					(calendarEU.day)++;
					context->commandIndex = 9;
					context->endIndex = 34;
				} else if (passDayMonthYear == 1) {
					// Pass month
					(calendarEU.day) = 1;
					(calendarEU.month)++;
					context->commandIndex = 35;
					context->endIndex = 62;
				} else {
					// Pass year
					(calendarEU.day) = 1;
					(calendarEU.month) = 1;
					(calendarEU.year)++;
					context->commandIndex = 63;
					context->endIndex = 92;
				}
			} else if ((calendarEU.dayToSkip) == 0) {
				// Go back to game
				context->commandIndex = 93;
				context->endIndex = 96;
			} else {
				// Finish
				context->next_state = DONE;
				break;
			}
			
			(calendarEU.dayToSkip)--;

			return &sequences;
		case DONE: return nullptr;
	}
	return nullptr;
}