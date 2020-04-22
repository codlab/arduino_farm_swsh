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

/**
 * NOTE : THIS IS A WIP TO SHOW HOW AND IN WHICH CONDITION IT CAN BE POSSIBLE
 * 
 * - it's validated that the memory can hold the program
 * 
 * Future steps :
 * 
 * - add a switch method which will check for the Serial information about the configuration to load if any
 * - in such case, reset data
 */

#include "../../core/serial_report.h"
#include "../auto_3day_skipper/Auto3DaySkipper.h"
#include "../auto_fossil/AutoFossil.h"
#include "../auto_host/AutoHost.h"
#include "../auto_loto/AutoLoto.h"
#include "../berry_farmer/BerryFarmer.h"
#include "../box_manage/BoxManager.h"
#include "../turbo_a/TurboA.h"
#include "../watt_farmer/WattFarmer.h"
#include "../day_skipper_eu/DaySkipper_EU.h"
#include "../day_skipper_eu_nolimit/DaySkipper_EU_NoLimit.h"
#include "../day_skipper_us/DaySkipper_US.h"
#include "../day_skipper_us_nolimit/DaySkipper_US_NoLimit.h"
#include "../day_skipper_jp/DaySkipper_JP.h"
#include "../day_skipper_jp_nolimit/DaySkipper_JP_NoLimit.h"
#include "../crashfreeegg_dup/CrashFreeEgg.h"
#include "../twitch_plays/TwitchPlays.h"

void configure(Context* context) {
	switch(context->bot) {
		case AutoFossil:
			configureAutoFossil(context);
			context->next_step = autoFossil;
			break;
		case AutoHost:
			configureAutoHost(context);
			context->next_step = autoHost;
			break;
		case AutoLoto:
			configureAutoLoto(context);
			context->next_step = autoLoto;
			break;
		case BoxManager:
			configureBoxManager(context);
			context->next_step = boxManager;
			break;
		case DaySkipperEU:
			configureDaySkipperEU(context);
			context->next_step = daySkipperEU;
			break;
		case DaySkipperEUNoLimit:
			configureDaySkipperEUNoLimit(context);
			context->next_step = daySkipperEUNoLimit;
			break;
		case DaySkipperJP:
			configureDaySkipperJP(context);
			context->next_step = daySkipperJP;
			break;
		case DaySkipperJPNoLimit:
			configureDaySkipperJPNoLimit(context);
			context->next_step = daySkipperJPNoLimit;
			break;
		case DaySkipperUS:
			configureDaySkipperUS(context);
			context->next_step = daySkipperUS;
			break;
		case DaySkipperUSNoLimit:
			configureDaySkipperUSNoLimit(context);
			context->next_step = daySkipperUSNoLimit;
			break;
		case Auto3DaySkipper:
			context->next_step = auto3DaySkipper;
			break;
		case BerryFarmer:
			context->next_step = berryFarmer;
			break;
		case CrashFreeEggDup:
			context->next_step = crashFreeEgg;
			break;
		case TurboA:
			context->next_step = turboA;
			break;
		case WattFarmer:
			context->next_step = wattFarmer;
			break;
		case TwitchPlays:
			context->next_step = twitchPlays;
			break;
		case MissingNo:
		default:
			context->next_step = nullptr;
	}
}