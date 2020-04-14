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
	STEP_UP(1, 1),		//make sure face top and then left
	STEP_LEFT(1, 1),
	STEP_A(12, 8),		// Greetingzzz Trainer! What can I help you with? Zzzrt?
	STEP_A(12, 8),
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
	STEP_B(30, 8),
	STEP_B(30, 8),
	STEP_B(30, 8)
};

// optional day skip limit
unsigned int m_skip = 0;

//rewrite state_t
#define SETTINGS PROCESS_CUSTOM_1
#define GAME PROCESS_CUSTOM_2

void configureAutoLoto(Context *context) {
	context->set = nullptr;
}

// Prepare the next report for the host.
Command* autoLoto(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->botSteps = 0;
			context->bot = AutoLoto;
			context->next_state = SETTINGS;
			return manage_init(context);
		case SETTINGS:
			RETURN_NEW_SEQ(sequences_in_settings, GAME);
		case GAME: {
			(context->botSteps)++;
			// Get the next command sequence (new start and end)
			if(m_dayToSkip > 0 && m_skip == m_dayToSkip) {
				context->next_state = DONE;
				RETURN_NEW_SEQ(sequences_in_game, DONE);
			} else {
				m_skip++;
				RETURN_NEW_SEQ(sequences_in_game, SETTINGS);
			}
		}
		case DONE: default: return nullptr;
	}
	return nullptr;
}