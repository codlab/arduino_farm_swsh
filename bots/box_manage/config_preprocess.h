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

#ifdef BOX_COUNT
#ifndef BOX_MANAGER_INCLUDED
#define BOX_MANAGER_INCLUDED

#warning "OVERRIDE BOX RELEASE"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST not have any egges in the boxes
// -> You MUST completely filled all release boxes except for the last (rightmost) box
// -> If the last box is not full, pokemon must be arranged from left to right, top to bottom
// -> You MUST set the cursor mode to "Select"
// -> You MUST start the program on the top left Pokemon in a box
// -> Boxes to be released should be next to each other to the right side
// -> You have to start this program at the Change Grip/Order menu
// -> It takes ~1 minute 26 seconds to release 30 pokemon in a box

// Set how many boxes you want to release with m_boxCount
int m_boxCount = BOX_COUNT;
bool box_will_get_item = BOX_WILL_GET_ITEM;
bool box_will_release = BOX_WILL_RELEASE;
/*------------------------------------------*/

#endif
#endif