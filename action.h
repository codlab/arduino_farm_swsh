#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "Joystick.h"

void report_action(USB_JoystickReport_Input_t* const ReportData, Command* command);

#endif