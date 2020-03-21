#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "../Joystick.h"
#include "usb_device.h"

void report_action(USB_JoystickReport_Input_t* const ReportData, Command* command);

void goto_next(Context* context, Command* const command);

#endif