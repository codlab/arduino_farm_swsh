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

#ifndef AUTO_HOST_INCLUDED
#define AUTO_HOST_INCLUDED

#include "../../core/usb_device.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST stand in front of an active den (with watts already collected)
// -> Be careful if time is synced and past mid-night, you will start able to collect watts
// -> You MUST not have connected to the internet at the beginning
// -> You are adviced to have wired connection for stable and fast internet
// -> If you use WiFi you probably need to adjust your connection time below in code
// -> This program relies on clients to be ready before 1 minute mark, otherwise the raid fails
// -> Program will close the game when raid starts, and it may freeze on client's side for 10-15 seconds
// -> You have to start this program at the Change Grip/Order menu
// -> It takes ~3 minutes 25 seconds to host a raid

// -> Use optional link code or not? (true/false)
bool autohost_useLinkCode = true;

// -> Use random code (if autohost_useLinkCode = true)
// -> autohost_seed range is 0 to 255, same seed will always generate the same link code sequence
// -> As long as the board is not unplugged, the sequence will go random forever
// -> If the board is unplugged, the squence will start at the beginning again
bool autohost_useRandomCode = true;
uint8_t autohost_initial_rand_seed = 169;

// -> Set optional link code here (if autohost_useLinkCode = true, autohost_useRandomCode = false)
// -> e.g 4501: autohost = 4501;
// -> e.g 0389: autohost = 389;
int autohost_linkCode = 1649;
/*------------------------------------------*/

#endif