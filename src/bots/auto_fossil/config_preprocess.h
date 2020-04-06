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

#if defined FIRST_FOSSIL_TOP_SLOT && defined SECOND_FOSSIL_TOP_SLOT && defined TIME_BEFORE_SR && defined AUTO_SOFT_RESET
#ifndef AUTO_FOSSIL_INCLUDED
#define AUTO_FOSSIL_INCLUDED

#warning "OVERRIDE FOSSIL CONFIG"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have set text speed to FAST
// -> You MUST have the fossil pokemon register in the PokeDex
// -> You MUST turn off asking for nickname
// -> You have to start this program at the Change Grip/Order menu
// -> Save and talk to Cara Liss and check which slot of the fossil you wish to choose
// -> Make sure the number of both fossils is less than m_timesBeforeSR
// -> When music plays, keep an eye on the game to see if you got a shiny
// -> It takes ~18 seconds per fossil and ~34 seconds to soft reset

bool m_firstFossilTopSlot = FIRST_FOSSIL_TOP_SLOT >= 1; 	// true (top), false (bottom)
bool m_secondFossilTopSlot = SECOND_FOSSIL_TOP_SLOT >= 1; 	// true (top), false (bottom)
unsigned long m_timesBeforeSR = TIME_BEFORE_SR; 			// Number of fossil to get before restarting/stopping
/*------------------------------------------*/

#endif
#endif