#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#ifndef nullptr
#define nullptr 0
#endif

#include "../Joystick.h"

void report_action(USB_JoystickReport_Input_t* const ReportData, Command* command);

#endif