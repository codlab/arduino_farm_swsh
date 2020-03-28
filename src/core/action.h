#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "../Joystick.h"
#include "usb_device.h"

#define SIZE(ARRAY) sizeof(ARRAY) / sizeof(Command)

#define STEP_NOTHING(TIME) {NOTHING, TIME}
#define STEP_TRIGGERS(TIME, WAIT_FOR) {TRIGGERS, TIME}, {NOTHING, WAIT_FOR}
#define STEP_DOWN(TIME, WAIT_FOR) {DOWN, TIME}, {NOTHING, WAIT_FOR}
#define STEP_LEFT(TIME, WAIT_FOR) {LEFT, TIME}, {NOTHING, WAIT_FOR}
#define STEP_RIGHT(TIME, WAIT_FOR) {RIGHT, TIME}, {NOTHING, WAIT_FOR}
#define STEP_UP(TIME, WAIT_FOR) {UP, TIME}, {NOTHING, WAIT_FOR}
#define STEP_A(TIME, WAIT_FOR) {A, TIME}, {NOTHING, WAIT_FOR}
#define STEP_HOME(TIME, WAIT_FOR) {HOME, TIME}, {NOTHING, WAIT_FOR}
#define STEP_B(TIME, WAIT_FOR) {B, TIME}, {NOTHING, WAIT_FOR}

void report_action(USB_JoystickReport_Input_t* const ReportData, Command* command);

void goto_next(Context* context, Command* const command);

Command* get_command(Context* context, Command* sequences);
#endif