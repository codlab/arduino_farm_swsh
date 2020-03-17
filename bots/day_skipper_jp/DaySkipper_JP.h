/*
Pokemon Sword & Shield Auto Fossil - Proof-of-Concept

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

#ifndef DAY_SKIPPER_JP
#define DAY_SKIPPER_KP
#include "../../Joystick.h"
#include "../../core/action.h"
#include "../../core/usb_device.h"

Command* daySkipperJP(Context* context, USB_JoystickReport_Input_t* const ReportData);

void daySkipperJPInit(Context* context);

#endif