/*
Pokemon Sword & Shield Fast Berry Farmer - Proof-of-Concept

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

#include "BerryFarmer.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> You MUST have set text speed to FAST
// -> You MUST stand in front of a berry tree
// -> STAY AWAY from Greedent between Bridge Field and Stony Wilderness
// -> You should not be on a bike, but it would still work
// -> You have to start this program at the Change Grip/Order menu
// -> There is a very small chance that the game will crash, the program will save every 50 tries
// -> Note that if today's date is end of month for EU, or December for US, first loop will fail, this is normal
// -> It takes ~17 seconds per berry collection
/*------------------------------------------*/

static const Command PROGMEM sequences[] = {
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
	
	//----------Sync time and roll day(EU)/month(US)/year(JP) forward [9,56]----------
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
	
	// Sync and unsync time
	{A, 1},
	{NOTHING, 4},
	{A, 1},
	{NOTHING, 8},
	
	// To actually Date and Time
	{DOWN, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 7},
	
	// Plus one
	{UP, 1},
	{NOTHING, 1},
	{RIGHT, 28},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 4},
	
	// Back to game
	{HOME, 1},
	{NOTHING, 30},
	{HOME, 1},
	{NOTHING, 30},
	
	//----------Collect Berry [57,74]----------
	{A, 14},		// It's a Berry tree. Do you want to shake it?
	{NOTHING, 1},
	{A, 12},
	{NOTHING, 1},
	{A, 170},		// Yes
	{NOTHING, 1},
	{B, 30},		// X fell from the tree!
	{NOTHING, 1},
	{B, 65},		// There are X Berries on the ground. Quit
	{NOTHING, 1},
	{B, 16},		// You picked up the Betties that fell from the tree!
	{NOTHING, 1},
	{B, 16},
	{NOTHING, 1},
	{B, 16},
	{NOTHING, 1},
	{B, 16},
	{NOTHING, 14},
	
	//----------Save Game [75,80]----------
	{X, 1},	
	{NOTHING, 20},
	{R, 1},
	{NOTHING, 50},
	{A, 1},
	{NOTHING, 140}
};

// Saving
int berry_saveCount = 0;
int berry_saveAt = 50;

// Prepare the next report for the host.
Command* berryFarmer(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->commandIndex = 0;
			context->endIndex = 8;
			context->next_state = PROCESS_CUSTOM_1;
			return nullptr;
		case PROCESS_CUSTOM_1:
			// Get the next command sequence (new start and end)
			if (berry_saveCount == berry_saveAt) {
				context->commandIndex = 75;
				context->endIndex = 80;
				
				berry_saveCount = 0;
			} else {
				context->commandIndex = 9;
				context->endIndex = 74;
				
				berry_saveCount++;
			}

			return &sequences;
		case DONE: return nullptr;
	}
	return nullptr;
}