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

#ifndef USB_DEVICE_INCLUDED
#define USB_DEVICE_INCLUDED

#include "../Joystick.h"
#include "bot.h"

#ifndef nullptr
#define nullptr 0
#endif
#define ECHOES 2

typedef enum {
	PROCESS,
	PROCESS_SETTINGS, //name to facilitate tracking specific settings actions
	PROCESS_CUSTOM_1,
	PROCESS_CUSTOM_2,
	PROCESS_CUSTOM_3,
	PROCESS_CUSTOM_4,
	PROCESS_CUSTOM_5,
	PROCESS_CUSTOM_6,
	PROCESS_CUSTOM_7,
	PROCESS_CUSTOM_8,
	PROCESS_CUSTOM_9,
	DONE
} State_t;

typedef struct Context {
	State_t state;
	State_t next_state;
	int commandIndex;
	int endIndex;
	int durationCount;
	Bot bot;
} Context;


extern State_t state;

extern int echoes;
extern USB_JoystickReport_Input_t last_report;

/**
 * Get the next joystick report for the host
 * 
 * \param context    the context holding the current state, index, endIndex and waiting duration
 * \param ReportData the joystick report data holder
 * \return           a pointer to the current command sent or nullptr if none
 */
Command* GetNextReport(Context* context, USB_JoystickReport_Input_t* const ReportData);
#endif