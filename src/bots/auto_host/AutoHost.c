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

#include <stdlib.h>
#include "AutoHost.h"
#include "config_preprocess.h"
#include "config.h"

static const Command PROGMEM sequences[] = {
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

uint8_t host_sequence = 0;
bool _autohost_init_done = false;
uint8_t _auto_set_linkCode[] = {1,6,4,9};

void autoHostInit(Context* context) {
	if(!_autohost_init_done) {
		srand(autohost_initial_rand_seed);
		_autohost_init_done = true;
	}

	context->bot = AutoHost;
	context->commandIndex = 0;
	context->endIndex = 5;

	_auto_set_linkCode[0] = autohost_linkCode % 10;
	_auto_set_linkCode[1] = (autohost_linkCode / 10) % 10;
	_auto_set_linkCode[2] = (autohost_linkCode / 100) % 10;
	_auto_set_linkCode[3] = (autohost_linkCode / 1000) % 10;

	context->next_state = PROCESS_CUSTOM_1;
}

// Prepare the next report for the host.
Command* autoHost(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// Get the next command sequence (new start and end)
	switch(context->state) {
		case PROCESS:
			autoHostInit(context);
			return nullptr;
		case PROCESS_CUSTOM_1:
			host_sequence++;
			
			if (host_sequence == 1) {
				// Connect internet and enter raid
				context->commandIndex = 6;	// 6 = go online, 11 = local only
				context->endIndex = 12;
			} else if (host_sequence == 2) {					
				if (!autohost_useLinkCode) {
					// Skip to start raid, invite, SR
					context->commandIndex = 16;
					context->endIndex = 34;
					
					host_sequence = 0;
				} else {
					// Prepare link code, goto 0
					context->commandIndex = 35;
					context->endIndex = 41;
				}
			} else if (host_sequence == 14) {
				// Finish setting link code, invite others, SR
				context->commandIndex = 13;
				context->endIndex = 34;
				
				host_sequence = 0;
			} else { // if (host_sequence <= 13)
				// Entering link code
				if (host_sequence % 3 == 0) { // 3,6,9,12
					uint8_t number = autohost_useRandomCode ? (rand() % 10) : _auto_set_linkCode[host_sequence / 3 - 1];
					
					if (number == 0) {					
						// Just press A for 0
						context->commandIndex = 42;
						context->endIndex = 43;
						
						// Skip going down
						host_sequence += 2;
					} else if (number % 3 == 0) { // 3,6,9
						context->commandIndex = 52 + (number / 3 - 1) * 2;
						context->endIndex = 59;
					} else { // 1,4,7,2,5,8
						context->commandIndex = 44 + (number / 3) * 2;
						context->endIndex = (number % 3 == 1) ? 51 : 49;
					}
				} else if (host_sequence % 3 == 1) { // 4,7,10,13
					// Press A
					context->commandIndex = 42;
					context->endIndex = 43;
				} else { // 5,8,11
					// Reset to 0
					context->commandIndex = 36;
					context->endIndex = 41;
				}
			}
			return &sequences;
		case DONE:
			return nullptr;
	}
}