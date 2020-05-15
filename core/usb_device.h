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

#include "context.h"
#include "../Joystick.h"

#ifndef nullptr
#define nullptr 0
#endif

/**
 * Get the next joystick report for the host
 * 
 * \param context    the context holding the current state, index, endIndex and waiting duration
 * \param ReportData the joystick report data holder
 * \return           a pointer to the current command sent or nullptr if none
 */
Command* GetNextReport(Context* context, USB_JoystickReport_Input_t* const ReportData);

#endif