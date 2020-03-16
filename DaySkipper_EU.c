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

#include "Joystick.h"
#include "action.h"
#include "usb_device.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> Set your date info here, date must be between 1/1/2000 to 31/12/2060 (22280 days advance)
// -> Use this website to check your end date: https://www.timeanddate.com/date/dateadd.html
// -> MAKE SURE the end date is within range, if your seed is over 22280 skips, enter the game
// to register the time skip, set a new start date, update this program and start again!
// -> It takes ~43 seconds per 30 days

int m_day = 28;
int m_month = 11;
int m_year = 2020;
int m_dayToSkip = 40;
/*------------------------------------------*/

static const Command daySkipper[] = {
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

// start and end index of "Setup"
int commandIndex = 0;
int m_endIndex = 8;

// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {

	// Prepare an empty report
	memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;

	// Repeat ECHOES times the last report
	if (echoes > 0) {
		memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
		echoes--;
		return;
	}

	// States and moves management
	switch (state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (commandIndex == -1) {
				if (m_dayToSkip > 0) {
					// Day = 0, Month = 1, Year = 2
					int passDayMonthYear = 0;
					
					if (m_month == 2) {
						bool isLeapYear = (m_year % 4 == 0);
						if (isLeapYear && m_day == 29)
						{
							passDayMonthYear = 1;
						}
						else if (!isLeapYear && m_day == 28)
						{
							passDayMonthYear = 1;
						}
					} else if (m_month == 12 && m_day == 31) {
						passDayMonthYear = 2;
					} else if (m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11) {
						if (m_day == 30) {
							passDayMonthYear = 1;
						}
					} else { //if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10)
						if (m_day == 31) {
							passDayMonthYear = 1;
						}
					}
					
					if (passDayMonthYear == 0) {
						// Pass day
						m_day++;
						commandIndex = 9;
						m_endIndex = 34;
					} else if (passDayMonthYear == 1) {
						// Pass month
						m_day = 1;
						m_month++;
						commandIndex = 35;
						m_endIndex = 62;
					} else {
						// Pass year
						m_day = 1;
						m_month = 1;
						m_year++;
						commandIndex = 63;
						m_endIndex = 92;
					}
				} else if (m_dayToSkip == 0) {
					// Go back to game
					commandIndex = 93;
					m_endIndex = 96;
				} else {
					// Finish
					state = DONE;
					break;
				}
				
				m_dayToSkip--;
			}
		
			report_action(ReportData, &(daySkipper[commandIndex]));
			durationCount++;

			if (durationCount > daySkipper[commandIndex].duration) {
				commandIndex++;
				durationCount = 0;		

				// We reached the end of a command sequence
				if (commandIndex > m_endIndex) {
					commandIndex = -1;
				}		
			}

			break;

		case DONE: return;
	}

	// Prepare to echo this report
	memcpy(&last_report, ReportData, sizeof(USB_JoystickReport_Input_t));
	echoes = ECHOES;
}