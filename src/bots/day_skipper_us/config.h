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

#ifndef DAY_SKIPPER_JP_INCLUDED
#define DAY_SKIPPER_JP_INCLUDED

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> Set your date info here, date must be between 1/1/2000 to 12/31/2060 (22280 days advance)
// -> Use this website to check your end date: https://www.timeanddate.com/date/dateadd.html
// -> MAKE SURE the end date is within range, if your seed is over 22280 skips, enter the game
// to register the time skip, set a new start date, update this program and start again!
// -> It takes ~43 seconds per 30 days
#include "../../core/calendar_skip.h"

CalendarSkip calendarUS = {
    .day = 28,
    .month = 11,
    .year = 2020,
    .dayToSkip = 50
};
/*------------------------------------------*/

#endif