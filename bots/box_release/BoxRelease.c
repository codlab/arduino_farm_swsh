/*
Pokemon Sword & Shield Box Pokemon Release - Proof-of-Concept

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

#include "BoxRelease.h"
#include "config.h"

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
	
	//----------Release Pokemon [9,22]----------
	{A, 10},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{A, 30},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{A, 48},
	{NOTHING, 1},
	{A, 1},
	{NOTHING, 10},
	
	//----------Movement [23,56]----------
	// Next box [23-42]
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{UP, 1},
	{NOTHING, 1},
	{R, 1},
	{NOTHING, 14},
	
	// Next Pokemon [43-44]
	{RIGHT, 1},
	{NOTHING, 1},
	
	// Next Row [45-56]
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{LEFT, 1},
	{NOTHING, 1},
	{DOWN, 1},
	{NOTHING, 1},
};

int m_column = 1;
int m_row = 1;
int m_box = 1;
bool m_released = false;

void boxReleaseInit(Context* context) {
	context->commandIndex = 0;
	context->endIndex = 8;
	context->state = PROCESS;
}

// Prepare the next report for the host.
Command* boxRelease(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			// Get the next command sequence (new start and end)
			if (context->commandIndex == -1) {
				if (context->endIndex == 6) {
					// Complete
					context->state = DONE;
					break;
				} else if (m_column > 6) {
					if (m_row == 5) {
						m_box++;
						if (m_box > m_boxCount) {
							// Press B to leave
							context->commandIndex = 5;
							context->endIndex = 6;
						} else {
							// Next box
							context->commandIndex = 23;
							context->endIndex = 42;
							
							m_row = 1;
						}
					} else {
						// Next row
						context->commandIndex = 45;
						context->endIndex = 56;
						
						m_row++;
					}
					
					m_column = 1;
					m_released = false;
				} else {
					if (!m_released) {
						// Release pokemon
						context->commandIndex = 9;
						context->endIndex = 22;
						
						m_column++;
						m_released = true;
					} else {
						// Next pokemon
						context->commandIndex = 43;
						context->endIndex = 44;
						
						m_released = false;
					}
				}
			}

			return &(sequences[context->commandIndex]);
		case DONE: return nullptr;
	}
	return nullptr;
}