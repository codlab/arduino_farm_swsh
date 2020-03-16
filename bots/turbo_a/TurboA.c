/*
Pokemon Sword & Shield Turbo A button - Proof-of-Concept

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

#include "TurboA.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You have to start this program at the Change Grip/Order menu
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
	
	//----------Turbo A [9,10]----------
	{A, 1},
	{NOTHING, 1}
};

// start and end index of "Setup"
int commandIndex = 0;
int m_endIndex = 8;

// Prepare the next report for the host.
void turboA(USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (state)
	{
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (commandIndex == -1)
			{
				commandIndex = 9;
				m_endIndex = 10;
			}
		
			report_action(ReportData, &(sequences[commandIndex]));
			durationCount++;

			if (durationCount > sequences[commandIndex].duration)
			{
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