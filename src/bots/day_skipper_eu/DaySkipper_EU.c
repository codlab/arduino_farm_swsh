/*
Pokemon Sword & Shield AUTO Day Skipper - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

#include "DaySkipper_EU.h"
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

CalendarSkip _skip;

void daySkipperEUInit(Context* context) {
	_skip.day = calendarEU.day;
	_skip.dayToSkip = calendarEU.dayToSkip;
	_skip.month = calendarEU.month;
	_skip.year = calendarEU.year;
	context->commandIndex = 0;
	context->endIndex = 8;
	context->state = PROCESS;
}

// Prepare the next report for the host.
Command* daySkipperEU(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (context->commandIndex == -1) {
				if (_skip.dayToSkip > 0) {
					// Day = 0, Month = 1, Year = 2
					int passDayMonthYear = 0;
					
					if (_skip.month == 2) {
						bool isLeapYear = ((_skip.year) % 4 == 0);
						if (isLeapYear && (_skip.day) == 29)
						{
							passDayMonthYear = 1;
						}
						else if (!isLeapYear && (_skip.day) == 28)
						{
							passDayMonthYear = 1;
						}
					} else if ((_skip.month) == 12 && (_skip.day) == 31) {
						passDayMonthYear = 2;
					} else if ((_skip.month) == 4 || (_skip.month) == 6 || (_skip.month) == 9 || (_skip.month) == 11) {
						if ((_skip.day) == 30) {
							passDayMonthYear = 1;
						}
					} else { //if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10)
						if ((_skip.day) == 31) {
							passDayMonthYear = 1;
						}
					}
					
					if (passDayMonthYear == 0) {
						// Pass day
						(_skip.day)++;
						context->commandIndex = 9;
						context->endIndex = 34;
					} else if (passDayMonthYear == 1) {
						// Pass month
						(_skip.day) = 1;
						(_skip.month)++;
						context->commandIndex = 35;
						context->endIndex = 62;
					} else {
						// Pass year
						(_skip.day) = 1;
						(_skip.month) = 1;
						(_skip.year)++;
						context->commandIndex = 63;
						context->endIndex = 92;
					}
				} else if ((_skip.dayToSkip) == 0) {
					// Go back to game
					context->commandIndex = 93;
					context->endIndex = 96;
				} else {
					// Finish
					context->state = DONE;
					break;
				}
				
				(_skip.dayToSkip)--;
			}

			return &(sequences[context->commandIndex]);
		case DONE: return nullptr;
	}
	return nullptr;
}