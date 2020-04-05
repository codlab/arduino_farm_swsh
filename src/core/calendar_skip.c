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

#include <string.h>
#include "calendar_skip.h"

#define FROM_BUFFER_SIZE 100
static char from_buffer[FROM_BUFFER_SIZE];

#define TRANSFORM_TO_(TYPE, BUFFER) \
    TYPE v = 0; \
    int index = 0; \
    while(index < strlen(BUFFER)) { \
        int digit = BUFFER[index] - '0'; \
        if(digit < 0 || digit > 9) return v; \
        v = v * 10 + digit; \
        index ++; \
    } \
    return v; \

unsigned int to_uint(const char* str) {
    TRANSFORM_TO_(unsigned int, str);    
}

unsigned long to_ulong(const char* str) {
    TRANSFORM_TO_(unsigned int, str);
}

#define BUFFER_TO_VALUE(BUFFER, TYPE, VAR, OUTPUT) \
    char* VAR = strtok(BUFFER, ' '); \
    if(NULL == VAR) return input; \
    OUTPUT = TYPE(VAR);


CalendarSkip* from(CalendarSkip* input, const char* str, unsigned int length) {
    if(NULL == input) return NULL;
    int i = 0;
    for(; i < length && i < FROM_BUFFER_SIZE; from_buffer[i] = str[i], i++) { };
    from_buffer[i] = from_buffer[99] = NULL;

    BUFFER_TO_VALUE(from_buffer, to_uint, day, input->day)
    BUFFER_TO_VALUE(NULL, to_uint, month, input->month)
    BUFFER_TO_VALUE(NULL, to_uint, year, input->year)
    BUFFER_TO_VALUE(NULL, to_ulong, dayToSkip, input->dayToSkip)

    return NULL;
}