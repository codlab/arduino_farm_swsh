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

static const Command PROGMEM setup[] = {
	// Connect controller in Change Grip/Order
	STEP_NOTHING(30),
	STEP_TRIGGERS(1, 30),
	STEP_A(1, 40),
	STEP_B(1, 40),
	STEP_HOME(1, 60)
}
	
static const Command PROGMEM gggoooooo[] = {
	STEP_A(1, 1)
};

// Prepare the next report for the host.
Command* turboA(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->next_state = PROCESS_CUSTOM_1;
			return nullptr;
		case PROCESS_CUSTOM_1:
			context->endIndex = SIZE(setup);
			context->next_state = PROCESS_CUSTOM_2;

			return &setup;
		case PROCESS_CUSTOM_2:
			context->endIndex = SIZE(gggoooooo);
			return &gggoooooo;
		case DONE: return nullptr;
	}
	return nullptr;
}