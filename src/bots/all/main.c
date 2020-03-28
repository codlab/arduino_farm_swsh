/*
Pokemon Sword & Shield AUTO 3 Day Skipper - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
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

// Prepare the next report for the host.
Command* GetNextReport(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	autoHost(context, ReportData);
	autoFossil(context, ReportData);
	autoLoto(context, ReportData);
	berryFarmer(context, ReportData);
	boxRelease(context, ReportData);
	turboA(context, ReportData);
	wattFarmer(context, ReportData);
	daySkipperEU(context, ReportData);
	daySkipperUS(context, ReportData);
	daySkipperJP(context, ReportData);
	daySkipperEUNoLimit(context, ReportData);
	daySkipperUSNoLimit(context, ReportData);
	daySkipperJPNoLimit(context, ReportData);
	return auto3DaySkipper(context, ReportData);
}