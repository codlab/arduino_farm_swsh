/*
Pokemon Sword & Shield AUTO Day Skipper w/o Limit - Proof-of-Concept

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

#include "DaySkipper_US_NoLimit.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> You MUST set your current date to Janurary 1st or any month with 31 days, year doesn't matter
// -> It takes ~43 seconds per 30 days

// -> Set number of days you want to skip,can't be greater than 4294967295
unsigned long m_dayToSkip = 172;
/*------------------------------------------*/

static const Command sequences[] = {
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
	
	//----------Back to game [35,38]----------
	{HOME, 1},
	{NOTHING, 30},
	{HOME, 1},
	{NOTHING, 20}
};

// start and end index of "Setup"
int commandIndex = 0;
int m_endIndex = 8;
int m_day = 1; // [1,31]

// Prepare the next report for the host.
void daySkipperUSNoLimit(USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (commandIndex == -1) {
				if (m_endIndex == 38) {
					// Finish
					state = DONE;
					break;
				} else if (m_dayToSkip > 0) {
					// Pass day
					commandIndex = 9;
					m_endIndex = 34;
					
					if (m_day == 31) {
						// Rolling back, no day skipped
						m_day = 1;
					} else {
						// Roll foward by a day
						m_day++;
						m_dayToSkip--;
					}
				} else {
					// Go back to game
					commandIndex = 35;
					m_endIndex = 38;
				}
			}
		
			report_action(ReportData, &(sequences[commandIndex]));
			durationCount++;

			if (durationCount > sequences[commandIndex].duration) {
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
}