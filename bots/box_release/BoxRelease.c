/*
Pokemon Sword & Shield Box Pokemon Release - Proof-of-Concept

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

#include "BoxRelease.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST not have any egges in the boxes
// -> You MUST completely filled all release boxes except for the last (rightmost) box
// -> If the last box is not full, pokemon must be arranged from left to right, top to bottom
// -> You MUST set the cursor mode to "Select"
// -> You MUST start the program on the top left Pokemon in a box
// -> Boxes to be released should be next to each other to the right side
// -> You have to start this program at the Change Grip/Order menu
// -> It takes ~1 minute 26 seconds to release 30 pokemon in a box

// Set how many boxes you want to release with m_boxCount
int m_boxCount = 2;
/*------------------------------------------*/

static const Command sequences[] = {
	//----------Setup [0,8]----------
	// Connect controller in Change Grip/Order
	{NOTHING, 30},
	{TRIGGERS, 1},
	{NOTHING, 30},
	{A, 1},
	{NOTHING, 40},
	{B, 1},
	{NOTHING, 40},
	{HOME, 1},
	{NOTHING, 60},
	
	//----------Release Pokemon [9,22]----------
	{A, 10},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{A, 30},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{A, 48},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 10},
	
	//----------Movement [23,56]----------
	// Next box [23-42]
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
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{R, 1},
	{NOTHING, 14},
	
	// Next Pokemon [43-44]
	{RIGHT, 1},
	{NOTHING, 1},
	
	// Next Row [45-56]
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
	{DOWN, 1},
	{NOTHING, 1},
};

// start and end index of "Setup"
int commandIndex = 0;
int m_endIndex = 8;

int m_column = 1;
int m_row = 1;
int m_box = 1;
bool m_released = false;

// Prepare the next report for the host.
void boxRelease(USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (commandIndex == -1) {
				if (m_endIndex == 6) {
					// Complete
					state = DONE;
					break;
				} else if (m_column > 6) {
					if (m_row == 5) {
						m_box++;
						if (m_box > m_boxCount) {
							// Press B to leave
							commandIndex = 5;
							m_endIndex = 6;
						} else {
							// Next box
							commandIndex = 23;
							m_endIndex = 42;
							
							m_row = 1;
						}
					} else {
						// Next row
						commandIndex = 45;
						m_endIndex = 56;
						
						m_row++;
					}
					
					m_column = 1;
					m_released = false;
				} else {
					if (!m_released) {
						// Release pokemon
						commandIndex = 9;
						m_endIndex = 22;
						
						m_column++;
						m_released = true;
					} else {
						// Next pokemon
						commandIndex = 43;
						m_endIndex = 44;
						
						m_released = false;
					}
				}
			}
		
			report_action(ReportData, &(sequences[commandIndex]));
			durationCount++;

			if (durationCount > sequences[commandIndex].duration) {
				commandIndex++;
				durationCount = 0;		

				// We reached the end of a command sequence
				if (commandIndex > m_endIndex)
				{
					commandIndex = -1;
				}		
			}

			break;

		case DONE: return;
	}
}