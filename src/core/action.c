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

#include "action.h"

void report_action(USB_JoystickReport_Input_t* const ReportData, Command* command) {
    if(nullptr != command && nullptr != ReportData) {
        switch (command->button) {
            case UP:
                ReportData->LY = STICK_MIN;
                break;

            case LEFT:
                ReportData->LX = STICK_MIN;
                break;

            case DOWN:
                ReportData->LY = STICK_MAX;
                break;

            case RIGHT:
                ReportData->LX = STICK_MAX;
                break;

            case CIRCLE:
                ReportData->LX = STICK_MAX;
                ReportData->RX = STICK_MAX;
                break;

            case X:
                ReportData->Button |= SWITCH_X;
                break;

            case Y:
                ReportData->Button |= SWITCH_Y;
                break;

            case A:
                ReportData->Button |= SWITCH_A;
                break;

            case B:
                ReportData->Button |= SWITCH_B;
                break;

            case L:
                ReportData->Button |= SWITCH_L;
                break;

            case R:
                ReportData->Button |= SWITCH_R;
                break;

            case ZL:
                ReportData->Button |= SWITCH_ZL;
                break;

            case ZR:
                ReportData->Button |= SWITCH_ZR;
                break;

            case MINUS:
                ReportData->Button |= SWITCH_MINUS;
                break;

            case PLUS:
                ReportData->Button |= SWITCH_PLUS;
                break;

            case LCLICK:
                ReportData->Button |= SWITCH_LCLICK;
                break;

            case RCLICK:
                ReportData->Button |= SWITCH_RCLICK;
                break;

            case TRIGGERS:
                ReportData->Button |= SWITCH_L | SWITCH_R;
                break;

            case HOME:
                ReportData->Button |= SWITCH_HOME;
                break;

            case CAPTURE:
                ReportData->Button |= SWITCH_CAPTURE;
                break;

            default:
                // really nothing lol
                break;
        }

    }
}

void goto_next(Context* context, Command* const command) {
    context->durationCount++;
    if (context->durationCount > command->duration) {
        context->commandIndex++;
        context->durationCount = 0;

        // We reached the end of a command sequence
        if (context->commandIndex > context->endIndex) {
            context->commandIndex = -1;
        }
    }
}

Command* get_command(Context* context, Command* sequences) {
    if(context->commandIndex == -1) return nullptr;

    return &(sequences[context->commandIndex]);
}