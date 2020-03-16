/*
Pokemon Sword & Shield Auto Fossil - Proof-of-Concept

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
#include "./core/action.h"
#include "./core/usb_device.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have set text speed to FAST
// -> You MUST have the fossil pokemon register in the PokeDex
// -> You MUST turn off asking for nickname
// -> You have to start this program at the Change Grip/Order menu
// -> Save and talk to Cara Liss and check which slot of the fossil you wish to choose
// -> Make sure the number of both fossils is less than m_timesBeforeSR
// -> When music plays, keep an eye on the game to see if you got a shiny
// -> It takes ~18 seconds per fossil and ~34 seconds to soft reset

bool m_firstFossilTopSlot = true; 	// true (top), false (bottom)
bool m_secondFossilTopSlot = false; 	// true (top), false (bottom)
int m_timesBeforeSR = 150; 			// Number of fossil to get before restarting/stopping
bool m_autoSoftReset = false;		// true: Auto soft reset after complete, false: stop when done
/*------------------------------------------*/

static const Command autoFossil[] = {
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
	
	//----------Fossil Startup [9,12]----------
	{A, 40},		// Talk
	{NOTHING, 1},
	{A, 32},		// Yes
	{NOTHING, 1},
	
	//----------1st/2nd fossil [13,16]----------
	{DOWN, 1},		// 2nd fossil
	{NOTHING, 1},
	{A, 32},		// 1st fossil
	{NOTHING, 1},
	
	//----------Get fossil [17,34]----------
	{A, 20},		// Yes please
	{NOTHING, 1},
	{A, 160},		// OK. Restoration time...
	{NOTHING, 1},
	{A, 30},		// All right, I'll stick 'em together! ...
	{NOTHING, 1},
	{A, 20},		// Objective complete...
	{NOTHING, 1},
	{A, 16},		// Yes, I can see it in its eyes...
	{NOTHING, 1},
	{A, 180},		// Please take and care for this Pokemon, huh. (Music plays)
	{NOTHING, 1},
	{B, 80},		// You received XXX! (please check if this is a shiny here)
	{NOTHING, 1},
	{B, 24},		// Send to box
	{NOTHING, 1},
	{B, 60},		// XXX has been sent to box
	{NOTHING, 1},
	
	//----------Finish/Prepare SR [35,36]----------
	{HOME, 1},		// (We use this to tell player it is done, if m_autoSoftReset = false
	{NOTHING, 40},
	
	//----------Soft reset [37,46]----------
	{X, 1},			// Close game
	{NOTHING, 8},
	{A, 1},			// Comfirm close game
	{NOTHING, 120},
	{A, 1},			// Choose game
	{NOTHING, 50},
	{A, 1},			// Pick User
	{NOTHING, 720},
	{A, 1},			// Enter game
	{NOTHING, 460}
};

// start and end index of "Setup"
int commandIndex = 0;
int m_endIndex = 8;

// Cara Liss talk
int m_talkSequence = 0;
int m_fossilCount = 0;

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
	if (echoes > 0)
	{
		memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
		echoes--;
		return;
	}

	// States and moves management
	switch (state)
	{
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (commandIndex == -1) {
				if (m_fossilCount == m_timesBeforeSR) {
					if (m_autoSoftReset) {
						// Soft reset
						commandIndex = 35;
						m_endIndex = 46;
						
						m_fossilCount = 0;
					} else {
						if (m_talkSequence == 0) {
							// Goto HOME and tell player it's finished
							commandIndex = 35;
							m_endIndex = 36;
							
							m_talkSequence++;
						} else {
							// Finish
							state = DONE;
							break;
						}
					}
				}
				else
				{
					m_talkSequence++;
					
					if (m_talkSequence == 1) {
						// Start talking
						commandIndex = 9;
						m_endIndex = 12;
					} else if (m_talkSequence >= 4) {
						// Getting fossil
						commandIndex = 17;
						m_endIndex = 34;
						
						m_talkSequence = 0;
						m_fossilCount++;
					} else {
						bool topSlot = (m_talkSequence == 2) ? m_firstFossilTopSlot : m_secondFossilTopSlot;
						commandIndex = topSlot ? 15 : 13;
						m_endIndex = 16;
					}
				}
			}
		
			report_action(ReportData, &(autoFossil[commandIndex]));
			durationCount++;

			if (durationCount > autoFossil[commandIndex].duration) {
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