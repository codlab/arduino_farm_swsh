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

#include "AutoFossil.h"
#include "config_preprocess.h" //in case proper variables set, override
#include "config.h"

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

// Cara Liss talk
int m_talkSequence = 0;
int m_fossilCount = 0;

void autoFossilInit(Context* context) {
	context->commandIndex = 0;
	context->endIndex = 8;
	context->state = PROCESS;
}

// Prepare the next report for the host.
Command* autoFossil(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (context->commandIndex == -1) {
				if (m_fossilCount == m_timesBeforeSR) {
					if (m_autoSoftReset) {
						// Soft reset
						context->commandIndex = 35;
						context->endIndex = 46;
						
						m_fossilCount = 0;
					} else {
						if (m_talkSequence == 0) {
							// Goto HOME and tell player it's finished
							context->commandIndex = 35;
							context->endIndex = 36;
							
							m_talkSequence++;
						} else {
							// Finish
							context->state = DONE;
							break;
						}
					}
				} else {
					m_talkSequence++;
					
					if (m_talkSequence == 1) {
						// Start talking
						context->commandIndex = 9;
						context->endIndex = 12;
					} else if (m_talkSequence >= 4) {
						// Getting fossil
						context->commandIndex = 17;
						context->endIndex = 34;
						
						m_talkSequence = 0;
						m_fossilCount++;
					} else {
						bool topSlot = (m_talkSequence == 2) ? m_firstFossilTopSlot : m_secondFossilTopSlot;
						context->commandIndex = topSlot ? 15 : 13;
						context->endIndex = 16;
					}
				}
			}

			return &(sequences[context->commandIndex]);
		case DONE: return nullptr;
	}
	return nullptr;
}