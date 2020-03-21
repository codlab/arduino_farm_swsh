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
	
	//----------Turbo A [9,10]----------
	{A, 1},
	{NOTHING, 1}
};

void turboAInit(Context* context) {
	context->commandIndex = 0;
	context->endIndex = 8;
	context->state = PROCESS;
}

// Prepare the next report for the host.
Command* turboA(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (context->commandIndex == -1) {
				context->commandIndex = 9;
				context->endIndex = 10;
			}

			return &(sequences[context->commandIndex]);
		case DONE: return nullptr;
	}
	return nullptr;
}