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

#include "AutoLoto.h"
#include "config_preprocess.h" //if have the preprocess
#include "config.h"

static const Command PROGMEM sequences_in_settings[] = {
	STEP_B(1, 1),
	STEP_B(1, 1),
	STEP_HOME(1, 30),
	STEP_DOWN(1, 1),
	STEP_DOWN(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_RIGHT(1, 1),
	STEP_A(1, 1),
	STEP_DOWN(80, 1), //To Console parameters
	STEP_A(1, 1),
	STEP_DOWN(1, 1), // To Date and Time
	STEP_DOWN(1, 1),
	STEP_DOWN(1, 1),
	STEP_DOWN(1, 1),
	STEP_A(1, 8),
	STEP_DOWN(1, 1), // To actually Date and Time
	STEP_DOWN(1, 1),
	STEP_A(1, 7),
	STEP_DOWN(1, 1), // Minus one
	STEP_RIGHT(28, 1),
	STEP_A(1, 4),
	STEP_A(1, 5), // Plus one
	STEP_LEFT(28, 1),
	STEP_UP(1, 1),
	STEP_RIGHT(28, 1),
	STEP_A(1, 4),
	STEP_HOME(1, 30), // Back to game
	STEP_HOME(1, 30)
};

static const Command PROGMEM sequences_in_game[] = {
	//STEP_UP(1, 1),		//make sure face top and then left
	//STEP_LEFT(1, 1), -> those 2 were because of an issue with A/B after // now A/A

	STEP_A(12, 1),		// Greetingzzz Trainer! What can I help you with? Zzzrt?
	STEP_A(12, 1),
	STEP_DOWN(1, 1),
	STEP_A(31, 1),		// I've now connected to the serverzzz at the Loto-ID Center!
	STEP_B(22, 1),		// We'll draw a number, and if it'zzz a match for the ID No. of any of your Pokemon, you could
	STEP_B(10, 1),		// win fabulous prizzzes!
	STEP_B(24, 1),		// Ready to save your progress and try your luck?
	STEP_A(45, 1),		// Yes
	STEP_B(19, 1),		// Then here we go... Bezzzt of luck to you!
	STEP_B(24, 1),		// ... ... ...
	STEP_B(7, 1),			// Here'zzz your number: xxxxx!
	STEP_B(10, 1),		// Let's see if it matches any Pokemon's ID number!
	STEP_B(96, 1),		// Oh! Congratulationszzz!
	STEP_B(22, 1),		// That'zzz amazing! The number matches up with the ID No. of ??? in your Boxes!
	STEP_B(22, 1),		// Oh! The last xxx digitzzz matched! (Longest: Moomoo Milk <- fuck you)
	STEP_B(26, 1),		// (Award dialog)
	STEP_B(95, 1),		// You obtained a xxx!
	STEP_B(20, 1),		// You put the xxx in your Bag's xxx pocket.
	STEP_B(16, 1),		// Looking forward to your nexzzzt attempt!
	STEP_B(30, 8),		// spam a lil more to be sure
	STEP_B(30, 8),
	STEP_B(30, 8),
	STEP_B(30, 8),
	STEP_B(30, 8),
	STEP_B(30, 8)
};

// optional day skip limit
unsigned int m_skip = 0;

#define STEP_SETTINGS 0
#define STEP_GAME 1
int loto_step = STEP_SETTINGS;


//GOTO requires context variable to be defined in the bloc
void goTo(Context* context, int step) {
	loto_step = step;
	context->commandIndex = 0;
	context->endIndex = ( step == STEP_SETTINGS ? SIZE(sequences_in_settings) : SIZE(sequences_in_game) ) - 1;
}

Command* get(Context* context) {
	if (loto_step == STEP_SETTINGS) return &(sequences_in_settings[context->commandIndex]);
	return &(sequences_in_game[context->commandIndex]);
}

void autoLotoInit(Context* context) {
	loto_step = STEP_SETTINGS;
	context->commandIndex = 0; 
	context->endIndex = SIZE(sequences_in_settings) - 1;
	context->state = PROCESS;
}

// Prepare the next report for the host.
Command* autoLoto(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (context->commandIndex == -1) {
				if(m_dayToSkip > 0 && m_skip == m_dayToSkip) {
					context->state = DONE;
				} else if (loto_step == STEP_SETTINGS) {
					goTo(context, STEP_GAME);
				} else {
					goTo(context, STEP_SETTINGS);
					
					m_skip++;
				}
			}

			return get(context);
		case DONE: return nullptr;
	}
	return nullptr;
}