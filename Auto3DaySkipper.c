/*
Pokemon Sword & Shield AUTO 3 Day Skipper - Proof-of-Concept

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
// -> You MUST have system time unsynced
// -> You MUST have set text speed to FAST
// -> You MUST stand in front of a wishing piece den
// -> Your date MUST have the following constrain:
//    US: Set month between 1 to 9
//    EU: Set day between 1 to 25
//    JP: Set year between 2000 to 2057
// -> You are adviced to ride on the bike (It rings the bell when 3 day skip is over!)
// -> You have to start this program at the Change Grip/Order menu
// -> It takes ~54 seconds to reach the 4th day and ~40 seconds to soft reset
/*------------------------------------------*/

static const Command auto3DaySkipper[] = {
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
	
	//----------Collect Watts [9,16]----------
	{A, 20},		// Talk
	{NOTHING, 1},
	{A, 6},			// There's energy pouring out from the den!
	{NOTHING, 1},
	{A, 30},		// You gained 2,000W!
	{NOTHING, 1},
	{A, 120},		// WAITING on local communication
	{NOTHING, 1},
	
	//----------Goto Date and Time [17,48]----------
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

	//----------Roll 3 days(EU)/months(US)/years(JP) backward [49,60]----------
	// Minus 3
	{A, 1},
	{NOTHING, 7},
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{RIGHT, 28},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 4},
	
	//----------Roll 1 day(EU)/month(US)/year(JP) forward [61,68]----------
	// Plus one
	{A, 1},
	{NOTHING, 7},
	{UP, 1},
	{NOTHING, 1},
	{RIGHT, 28},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 4},
	
	//----------Back to game [69,76]----------
	// Back to game
	{HOME, 1},
	{NOTHING, 30},
	{HOME, 1},
	{NOTHING, 30},
	
	// Quit the raid
	{B, 32},
	{NOTHING, 1},
	{A, 200},		// WAITING on local communication
	{NOTHING, 1},
	
	//----------Ring bike like mad, collect watts then delay [77,102]----------
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{LCLICK, 1},
	{NOTHING, 10},
	{A, 12},		// Talk
	{NOTHING, 1},
	{A, 6},			// There's energy pouring out from the den!
	{NOTHING, 1},
	{A, 30},		// You gained 2,000W! (Delay longer for player to check)
	{NOTHING, 300},
	
	//----------Finish/Prepare SR [103,114]----------
	{HOME, 1},
	{NOTHING, 40},
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
int m_sequence = 0;

// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {
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
				m_sequence++;
				if (m_sequence == 13) {
					// Done skipping 3 days, user should check the pokemon
					commandIndex = 77;
					m_endIndex = 102;
				} else if (m_sequence == 15) {
					// Roll 3 days backward
					commandIndex = 49;
					m_endIndex = 60;
				} else if (m_sequence == 16) {
					// SR
					commandIndex = 103;
					m_endIndex = 114;
					
					m_sequence = 0;
				} else if (m_sequence % 4 == 1)	{ // 1,5,9
					// Collect watts and invite others
					commandIndex = 9;
					m_endIndex = 16;
				} else if (m_sequence % 4 == 2) { // 2,6,10,14
					// Goto date and time
					commandIndex = 17;
					m_endIndex = 48;
				} else if (m_sequence % 4 == 3)	{ // 3,7,11
					// Roll one day forward
					commandIndex = 61;
					m_endIndex = 68;
				} else if (m_sequence % 4 == 0)	{ // 4,8,12
					// Back to game
					commandIndex = 69;
					m_endIndex = 76;
				}
			}

			report_action(ReportData, &(auto3DaySkipper[commandIndex]));
			durationCount++;

			if (durationCount > auto3DaySkipper[commandIndex].duration) {
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