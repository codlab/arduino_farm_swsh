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

#include "BoxManager.h"
#include "config_preprocess.h" //if box count set
#include "config.h"

static const Command PROGMEM sequences[] = {
	STEP_NOTHING(300),
	STEP_PLUS(1, 30),
	STEP_PLUS(1, 30),
};

static const Command PROGMEM pokemon_release[] = {
	STEP_A(1, 10),
	STEP_UP(1, 10),
	STEP_UP(1, 10),
	STEP_A(1, 50),
	STEP_UP(1, 50),
	STEP_A(1, 100),
	STEP_A(1, 100)
};

static const Command PROGMEM pokemon_get_item[] = {
	STEP_A(1, 10),
	STEP_DOWN(1, 10),
	STEP_DOWN(1, 10),
	STEP_A(1, 30),
	STEP_A(1, 30),
	STEP_A(1, 60),
	STEP_A(1, 60)
};

static const Command PROGMEM go_first_slot[] = {
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_LEFT(1, 1),
	STEP_UP(1,1),
	STEP_UP(1,1),
	STEP_UP(1,1),
	STEP_UP(1,1)
};

static const Command PROGMEM next_box[] = {
	STEP_R(1,10)
};

static const Command PROGMEM next_same_row[] = {
	STEP_RIGHT(1, 5)
};

static const Command PROGMEM __debug[] = {
	STEP_A(1, 20),
	STEP_B(1, 20)
};

static const Command PROGMEM next_new_row[] = {
	STEP_RIGHT(1, 10), //cursor is now on party
	STEP_RIGHT(1, 5),
	STEP_DOWN(1, 5),
};

const int width = 6;
const int height = 5;
int column = 0;
int row = 0;
int current_box = 0;

void configureBoxManager(Context *context) {
	context->set = nullptr;
}

#define PROCESS_AFTER_INIT PROCESS_CUSTOM_1
#define PROCESS_OBJECT PROCESS_CUSTOM_2
#define PROCESS_RELEASE PROCESS_CUSTOM_3
#define GO_NEXT_CELL PROCESS_CUSTOM_4
#define GO_NEXT_BOX PROCESS_CUSTOM_5

Command* boxManager(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->botSteps = 0;
			context->bot = BoxManager;
			RETURN_NEW_SEQ(sequences, PROCESS_OBJECT);
		case PROCESS_OBJECT:
			if(box_will_get_item) RETURN_NEW_SEQ(pokemon_get_item, PROCESS_RELEASE);
			if(box_will_release) RETURN_NEW_SEQ(pokemon_release, GO_NEXT_CELL);
			RETURN_NULL_SEQ(GO_NEXT_CELL);
		case PROCESS_RELEASE:
			if(box_will_release) RETURN_NEW_SEQ(pokemon_release, GO_NEXT_CELL);
			RETURN_NULL_SEQ(GO_NEXT_CELL);
		case GO_NEXT_CELL:
			(context->botSteps) ++;
			column ++;
			if(column < width) RETURN_NEW_SEQ(next_same_row, PROCESS_OBJECT);
			//else need to go next row

			column = 0;
			row ++;
			if(row < height) RETURN_NEW_SEQ(next_new_row, PROCESS_OBJECT);
			// else next to the next box

			current_box ++;
			row = 0;
			if(current_box < box_count) RETURN_NEW_SEQ(go_first_slot, GO_NEXT_BOX);
			//it's finished

			RETURN_NULL_SEQ(DONE);
		case GO_NEXT_BOX:
			RETURN_NEW_SEQ(next_box, PROCESS_OBJECT);
		case DONE: default: return nullptr;
	}
	return nullptr;
}