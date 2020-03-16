/*
Pokemon Sword & Shield AUTO Raid Hosting - Proof-of-Concept

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

#define USE_SRAND 1
#include "AutoHost.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST stand in front of an active den (with watts already collected)
// -> Be careful if time is synced and past mid-night, you will start able to collect watts
// -> You MUST not have connected to the internet at the beginning
// -> You are adviced to have wired connection for stable and fast internet
// -> If you use WiFi you probably need to adjust your connection time below in code
// -> This program relies on clients to be ready before 1 minute mark, otherwise the raid fails
// -> Program will close the game when raid starts, and it may freeze on client's side for 10-15 seconds
// -> You have to start this program at the Change Grip/Order menu
// -> It takes ~3 minutes 25 seconds to host a raid

// -> Use optional link code or not? (true/false)
bool m_useLinkCode = true;

// -> Set optional link code here (if m_useLinkCode = true, m_useRandomCode = false)
// -> e.g 4501: m_linkCode[] = {4,5,0,1};
// -> e.g 0389: m_linkCode[] = {0,3,8,9};
uint8_t m_linkCode[] = {1,6,4,9};
/*------------------------------------------*/

static const Command sequences[] = {
	//----------Setup [0,5]----------
	// Connect controller in Change Grip/Order
	{NOTHING, 30},
	{TRIGGERS, 30},
	{A, 40},
	{B, 40},
	{HOME, 1},
	{NOTHING, 60},
	
	//----------Connect Internet [6,12]----------
	{Y, 50},
	{PLUS, 400},		// Internet connection time (400 ~= 9 seconds wait)
	{B, 1},
	{NOTHING, 6}, 
	{B, 140},			// Allow time to load other players
	{A, 1},				// Interact with den
	{NOTHING, 200}, 	// Communicating... standing by
	
	//----------Confirm link code [13,15]----------
	{PLUS, 51},
	{A, 1},	
	{NOTHING, 30},
	
	//----------Start raid [16,23]----------
	{A, 5200},			// (5200 ~= until 1 min, 2600 ~= until 2 min)
	{UP, 2},	
	{A, 1},	
	{NOTHING, 20},
	{A, 50},	
	{NOTHING, 1},
	{A, 1},				
	{NOTHING, 800},	// Wait until game starting showing abilities (if any has one)
	
	//----------Finish/Prepare SR [24,34]----------
	{HOME, 1},
	{NOTHING, 40},
	{X, 9},			// Close game
	{A, 1},			// Comfirm close game
	{NOTHING, 120},
	{A, 1},			// Choose game
	{NOTHING, 50},
	{A, 1},			// Pick User
	{NOTHING, 720},
	{A, 1},			// Enter game
	{NOTHING, 460},
	
	//----------Set Link Code [35,59]----------
	// Init
	{PLUS, 35},
	
	// Reset to 0
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	
	// Press A
	{A, 1},
	{NOTHING, 1},
	
	// 1,4,7,2,5,8 [44-51]
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	
	// 3,6,9 [52-59]
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{RIGHT, 1},
	{NOTHING, 1},
};

// start and end index of "Setup"
int8_t commandIndex = 0;
int8_t m_endIndex = 5;
uint8_t m_sequence = 0;

// Prepare the next report for the host.
void autoHost(USB_JoystickReport_Input_t* const ReportData) {
	// Get the next command sequence (new start and end)
	if (commandIndex == -1) {
		m_sequence++;
		
		if (m_sequence == 1) {
			// Connect internet and enter raid
			commandIndex = 6;	// 6 = go online, 11 = local only
			m_endIndex = 12;
		} else if (m_sequence == 2) {					
			if (!m_useLinkCode) {
				// Skip to start raid, invite, SR
				commandIndex = 16;
				m_endIndex = 34;
				
				m_sequence = 0;
			} else {
				// Prepare link code, goto 0
				commandIndex = 35;
				m_endIndex = 41;
			}
		} else if (m_sequence == 14) {
			// Finish setting link code, invite others, SR
			commandIndex = 13;
			m_endIndex = 34;
			
			m_sequence = 0;
		} else { // if (m_sequence <= 13)
			// Entering link code
			if (m_sequence % 3 == 0) { // 3,6,9,12
				uint8_t number = m_useRandomCode ? (rand() % 10) : m_linkCode[m_sequence / 3 - 1];
				
				if (number == 0) {
					return;
					
					// Just press A for 0
					commandIndex = 42;
					m_endIndex = 43;
					
					// Skip going down
					m_sequence += 2;
				} else if (number % 3 == 0) { // 3,6,9
					commandIndex = 52 + (number / 3 - 1) * 2;
					m_endIndex = 59;
				} else { // 1,4,7,2,5,8
					commandIndex = 44 + (number / 3) * 2;
					m_endIndex = (number % 3 == 1) ? 51 : 49;
				}
			} else if (m_sequence % 3 == 1) { // 4,7,10,13
				// Press A
				commandIndex = 42;
				m_endIndex = 43;
			} else { // 5,8,11
				// Reset to 0
				commandIndex = 36;
				m_endIndex = 41;
			}
		}
	}

	report_action(ReportData, &(sequences[commandIndex]));

	goto_next(&durationCount, &commandIndex, m_endIndex, &(sequences[commandIndex]));
}