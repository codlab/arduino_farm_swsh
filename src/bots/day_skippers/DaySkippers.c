/*
Pokemon Sword & Shield AUTO Day Skipper - Proof-of-Concept

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

#include "DaySkippers.h"
#include "config_preprocess.h"
#include "config.h"
#include "DaySkippersSegment.h"


// Prepare the next report for the host.
Command* daySkippers(Context* context, USB_JoystickReport_Input_t* const ReportData) {
	// States and moves management
	switch (context->state) {
		case PROCESS:
			context->commandIndex = 0;
			context->endIndex = 8;
			context->next_state = PROCESS_CUSTOM_1;
			return nullptr;
		case PROCESS_CUSTOM_1:
			// Get the next command sequence (new start and end)
			if (calendar.dayToSkip > 0) {
				// Day = 0, Month = 1, Year = 2
				int passDayMonthYear = 0;
				
				if (calendar.month == 2) {
					bool isLeapYear = ((calendar.year) % 4 == 0);
					if (isLeapYear && (calendar.day) == 29)
					{
						passDayMonthYear = 1;
					}
					else if (!isLeapYear && (calendar.day) == 28)
					{
						passDayMonthYear = 1;
					}
				} else if ((calendar.month) == 12 && (calendar.day) == 31) {
					passDayMonthYear = 2;
				} else if ((calendar.month) == 4 || (calendar.month) == 6 || (calendar.month) == 9 || (calendar.month) == 11) {
					if ((calendar.day) == 30) {
						passDayMonthYear = 1;
					}
				} else { //if (m_month == 1 || m_month == 3 || m_month == 5 || m_month == 7 || m_month == 8 || m_month == 10)
					if ((calendar.day) == 31) {
						passDayMonthYear = 1;
					}
				}

				
				if (passDayMonthYear == 0) {
					// Pass day
					(calendar.day)++;
					if (m_JP_EU_US == 0) {
						context->commandIndex = 9;
						context->endIndex = 26;
					} else if (m_JP_EU_US == 1) {
						context->commandIndex = 81;
						context->endIndex = 106;
					} else { // if (m_JP_EU_US == 2)
						context->commandIndex = 165;
						context->endIndex = 190;
					}
				} else if (passDayMonthYear == 1) {
					// Pass month
					(calendar.day) = 1;
					(calendar.month)++;
					if (m_JP_EU_US == 0) {
						context->commandIndex = 27;
						context->endIndex = 50;
					} else if (m_JP_EU_US == 1) {
						context->commandIndex = 107;
						context->endIndex = 134;
					} else { // if (m_JP_EU_US == 2)
						context->commandIndex = 191;
						context->endIndex = 222;
					}
				} else {
					// Pass year
					(calendar.day) = 1;
					(calendar.month) = 1;
					(calendar.year)++;
					if (m_JP_EU_US == 0) {
						context->commandIndex = 51;
						context->endIndex = 80;
					} else if (m_JP_EU_US == 1) {
						context->commandIndex = 135;
						context->endIndex = 164;
					} else { // if (m_JP_EU_US == 2)
						context->commandIndex = 223;
						context->endIndex = 256;
					}
				}
			} else if ((calendar.dayToSkip) == 0) {
				// Go back to game
				context->commandIndex = 257;
				context->endIndex = 260;
			} else {
				// Finish
				context->next_state = DONE;
				return nullptr;
			}
			
			(calendar.dayToSkip)--;

			return &dayskipper_sequences;
		case DONE: default: return nullptr;
	}
	return nullptr;
}