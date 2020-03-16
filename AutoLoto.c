/*
Pokemon Sword & Shield AUTO Lottery - Proof-of-Concept

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
// -> You MUST have y-comm glitch active and system time unsynced
// -> You MUST have set text speed to FAST
// -> You have to start this program at the Change Grip/Order menu
// -> It takes ~23 seconds per loto

// -> Set optional day skip limit here, 0 = unlimited, max is 65535 (17.4 days to finish)
unsigned int m_dayToSkip = 0;
/*------------------------------------------*/

static const Command autoLoto[] = {
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
	
	//----------Roll day(EU)/month(US)/year(JP) back and forward [9,62]----------
	// To System Settings
	{HOME, 1},
	{NOTHING, 30},
	{DOWN, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 1},
	
	// To Date and Time
	{DOWN, 80},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 8},
	
	// To actually Date and Time
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 7},
	
	// Minus one
	{DOWN, 1},
	{NOTHING, 1},
	{RIGHT, 28},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 4},
	
	// Plus one
	{A, 1},
	{NOTHING, 5},
	{LEFT, 28},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{RIGHT, 28},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 4},
	
	// Back to game
	{HOME, 1},
	{NOTHING, 30},
	{HOME, 1},			// Use this to stop the program if limited days
	{NOTHING, 30},
	
	//----------Loto [63,102]----------
	{A, 12},			// Greetingzzz Trainer! What can I help you with? Zzzrt?
	{NOTHING, 1},
	{B, 12},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{A, 31},		// I've now connected to the serverzzz at the Loto-ID Center!
	{NOTHING, 1},
	{B, 22},		// We'll draw a number, and if it'zzz a match for the ID No. of any of your Pokemon, you could
	{NOTHING, 1},
	{B, 10},		// win fabulous prizzzes!
	{NOTHING, 1},
	{B, 24},		// Ready to save your progress and try your luck?
	{NOTHING, 1},
	{A, 45},		// Yes
	{NOTHING, 1},
	{B, 19},		// Then here we go... Bezzzt of luck to you!
	{NOTHING, 1},
	{B, 24},		// ... ... ...
	{NOTHING, 1},
	{B, 7},			// Here'zzz your number: xxxxx!
	{NOTHING, 1},
	{B, 10},		// Let's see if it matches any Pokemon's ID number!
	{NOTHING, 1},
	{B, 96},		// Oh! Congratulationszzz!
	{NOTHING, 1},
	{B, 22},		// That'zzz amazing! The number matches up with the ID No. of ??? in your Boxes!
	{NOTHING, 1},
	{B, 22},		// Oh! The last xxx digitzzz matched! (Longest: Moomoo Milk <- fuck you)
	{NOTHING, 1},
	{B, 26},		// (Award dialog)
	{NOTHING, 1},
	{B, 95},		// You obtained a xxx!
	{NOTHING, 1},
	{B, 20},		// You put the xxx in your Bag's xxx pocket.
	{NOTHING, 1},
	{B, 16},		// Looking forward to your nexzzzt attempt!
	{NOTHING, 1},
	{B, 1},
	{NOTHING, 8}
};

// start and end index of "Setup"
int commandIndex = 0;
int m_endIndex = 8;

// optional day skip limit
unsigned int m_skip = 0;

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
				if (m_dayToSkip > 0 && m_skip == m_dayToSkip) {
					if (m_endIndex == 62) {
						// Stop the program
						state = DONE;
						break;
					} else {
						// Go to home, reached day to skip
						commandIndex = 61;
						m_endIndex = 62;
					}
				} else {
					commandIndex = 9;
					m_endIndex = 102;
					
					m_skip++;
				}
			}
		
			report_action(ReportData, &(autoLoto[commandIndex]));
			durationCount++;

			if (durationCount > autoLoto[commandIndex].duration) {
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