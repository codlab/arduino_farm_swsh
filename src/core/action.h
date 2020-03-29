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

#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "../Joystick.h"
#include "usb_device.h"

#define SIZE(ARRAY) sizeof(ARRAY) / sizeof(Command)
#define RETURN_NEW_SEQ(ARRAY, NEXT_STATE) \
    context->next_state = NEXT_STATE; \
    context->endIndex = sizeof(ARRAY) / sizeof(Command) - 1; \
    return &ARRAY;

#define STEP_NOTHING(TIME) {NOTHING, TIME}
#define STEP_TRIGGERS(TIME, WAIT_FOR) {TRIGGERS, TIME}, {NOTHING, WAIT_FOR}
#define STEP_DOWN(TIME, WAIT_FOR) {DOWN, TIME}, {NOTHING, WAIT_FOR}
#define STEP_LEFT(TIME, WAIT_FOR) {LEFT, TIME}, {NOTHING, WAIT_FOR}
#define STEP_RIGHT(TIME, WAIT_FOR) {RIGHT, TIME}, {NOTHING, WAIT_FOR}
#define STEP_UP(TIME, WAIT_FOR) {UP, TIME}, {NOTHING, WAIT_FOR}
#define STEP_A(TIME, WAIT_FOR) {A, TIME}, {NOTHING, WAIT_FOR}
#define STEP_HOME(TIME, WAIT_FOR) {HOME, TIME}, {NOTHING, WAIT_FOR}
#define STEP_B(TIME, WAIT_FOR) {B, TIME}, {NOTHING, WAIT_FOR}
#define STEP_X(TIME, WAIT_FOR) {X, TIME}, {NOTHING, WAIT_FOR}
#define STEP_R(TIME, WAIT_FOR) {R, TIME}, {NOTHING, WAIT_FOR}
#define STEP_L(TIME, WAIT_FOR) {L, TIME}, {NOTHING, WAIT_FOR}
#define STEP_Y(TIME, WAIT_FOR) {Y, TIME}, {NOTHING, WAIT_FOR}

void report_action(USB_JoystickReport_Input_t* const ReportData, Command* command);

void goto_next(Context* context, Command* const command);

Command* get_command(Context* context, Command* sequences);
#endif