#ifndef USB_DEVICE_INCLUDED
#define USB_DEVICE_INCLUDED

#ifndef nullptr
#define nullptr 0

#endif
#define ECHOES 2

typedef enum {
	PROCESS,
	PROCESS_CUSTOM_1,
	PROCESS_CUSTOM_2,
	PROCESS_CUSTOM_3,
	PROCESS_CUSTOM_4,
	PROCESS_CUSTOM_5,
	PROCESS_CUSTOM_6,
	PROCESS_CUSTOM_7,
	PROCESS_CUSTOM_8,
	PROCESS_CUSTOM_9,
	DONE
} State_t;

typedef struct Context {
	State_t state;
	State_t next_state;
	int commandIndex;
	int endIndex;
	int durationCount;
} Context;


extern State_t state;

extern int echoes;
extern USB_JoystickReport_Input_t last_report;

/**
 * Get the next joystick report for the host
 * 
 * \param context    the context holding the current state, index, endIndex and waiting duration
 * \param ReportData the joystick report data holder
 * \return           a pointer to the current command sent or nullptr if none
 */
Command* GetNextReport(Context* context, USB_JoystickReport_Input_t* const ReportData);
#endif