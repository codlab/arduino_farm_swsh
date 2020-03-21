#include "action.h"

void report_action(USB_JoystickReport_Input_t* const ReportData, Command* command) {
    if(nullptr != command && nullptr != ReportData) {
        switch (command->button) {
            case UP:
                ReportData->LY = STICK_MIN;
                break;

            case LEFT:
                ReportData->LX = STICK_MIN;				
                break;

            case DOWN:
                ReportData->LY = STICK_MAX;				
                break;

            case RIGHT:
                ReportData->LX = STICK_MAX;				
                break;

            case X:
                ReportData->Button |= SWITCH_X;
                break;

            case Y:
                ReportData->Button |= SWITCH_Y;
                break;

            case A:
                ReportData->Button |= SWITCH_A;
                break;

            case B:
                ReportData->Button |= SWITCH_B;
                break;

            case L:
                ReportData->Button |= SWITCH_L;
                break;

            case R:
                ReportData->Button |= SWITCH_R;
                break;

            case ZL:
                ReportData->Button |= SWITCH_ZL;
                break;

            case ZR:
                ReportData->Button |= SWITCH_ZR;
                break;

            case MINUS:
                ReportData->Button |= SWITCH_MINUS;
                break;

            case PLUS:
                ReportData->Button |= SWITCH_PLUS;
                break;

            case LCLICK:
                ReportData->Button |= SWITCH_LCLICK;
                break;

            case RCLICK:
                ReportData->Button |= SWITCH_RCLICK;
                break;

            case TRIGGERS:
                ReportData->Button |= SWITCH_L | SWITCH_R;
                break;

            case HOME:
                ReportData->Button |= SWITCH_HOME;
                break;

            case CAPTURE:
                ReportData->Button |= SWITCH_CAPTURE;
                break;

            default:
                // really nothing lol
                break;
        }

    }
}

void goto_next(Context* context, Command* const command) {
    context->durationCount++;
    if (context->durationCount > command->duration) {
        context->commandIndex++;
        context->durationCount = 0;

        // We reached the end of a command sequence
        if (context->commandIndex > context->endIndex) {
            context->commandIndex = -1;
        }
    }
}