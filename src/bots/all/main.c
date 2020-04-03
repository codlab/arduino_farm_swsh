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
#include "../box_release/BoxRelease.h"
#include "../turbo_a/TurboA.h"
#include "../watt_farmer/WattFarmer.h"
#include "../day_skipper_eu/DaySkipper_EU.h"
#include "../day_skipper_eu_nolimit/DaySkipper_EU_NoLimit.h"
#include "../day_skipper_us/DaySkipper_US.h"
#include "../day_skipper_us_nolimit/DaySkipper_US_NoLimit.h"
#include "../day_skipper_jp/DaySkipper_JP.h"
#include "../day_skipper_jp_nolimit/DaySkipper_JP_NoLimit.h"
#include "../crashfreeegg_dup/CrashFreeEgg.h"

// Prepare the next report for the host.
Command* GetNextReport(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	switch(currentBot()) {
		case MissingNo: return nullptr;
		case Auto3DaysSkipper: return auto3DaySkipper(context, ReportData);
		case AutoFossil: return autoFossil(context, ReportData);
		case AutoHost: return autoHost(context, ReportData);
		case AutoLoto: return autoLoto(context, ReportData);
		case BerryFarmer: return berryFarmer(context, ReportData);
		case BoxRelease: return boxRelease(context, ReportData);
		case CrashFreeEggDup: return crashFreeEgg(context, ReportData);
		case DaySKipperEU: return daySkipperEU(context, ReportData);
		case DaySKipperEUNoLimit: return daySkipperEUNoLimit(context, ReportData);
		case DaySKipperJP: return daySkipperJP(context, ReportData);
		case DaySKipperJPNoLimit: return daySkipperJPNoLimit(context, ReportData);
		case DaySKipperUS: return daySkipperUS(context, ReportData);
		case DaySKipperUSNoLimit: return daySkipperUSNoLimit(context, ReportData);
		case TurboA: return turboA(context, ReportData);
		case WattFarmer: return wattFarmer(context, ReportData);
		default: return nullptr;
	}
}