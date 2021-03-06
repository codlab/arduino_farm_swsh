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

#ifndef DAY_SKIPPER_NOLIMIT
#define DAY_SKIPPER_NOLIMIT
#include "../Joystick.h"
#include "bot.h"
#include "calendar_skip.h"
#include "context.h"
#include "action.h"
#include "usb_device.h"
#include "lang.h"

Command* daySkipperNoLimit(Bot mode, unsigned long daySkip, Context* context, USB_JoystickReport_Input_t* const ReportData, Region region);

#endif