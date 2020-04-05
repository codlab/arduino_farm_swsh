/*
    Pokemon Sword/Shield bots using Arduino
    Copyright (C) 2020  brianuuuSonic / codlab

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


	Contextually based on the LUFA library's Low-Level Joystick Demo
		(C) Dean Camera
	Contextually based on the HORI's Pokken Tournament Pro Pad design
		(C) HORI

	For more information, please see the README.md available at https://github.com/codlab/arduino_farm_swsh
*/

#include "BoxRelease.h"
#include "config_preprocess.h" //if box count set
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

void configureBoxRelease(Context *context) {
	context->set = nullptr;
}

// Prepare the next report for the host.
Command* boxRelease(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->bot = BoxRelease;
			context->commandIndex = 0;
			context->endIndex = 8;
			context->next_state = PROCESS_CUSTOM_1;
			return nullptr;
		case PROCESS_CUSTOM_1:
			// Get the next command sequence (new start and end)
			if (context->endIndex == 6) {
				// Complete
				context->next_state = DONE;
				return nullptr;
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

			return &sequences;
		case DONE: return nullptr;
	}
	return nullptr;
}