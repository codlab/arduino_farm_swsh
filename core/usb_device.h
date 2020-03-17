#ifndef USB_DEVICE_INCLUDED
#define USB_DEVICE_INCLUDED

#ifndef nullptr
#define nullptr 0

#endif
#define ECHOES 2

typedef enum {
	PROCESS,
	DONE
} State_t;

typedef struct Context {
	State_t state;
	int commandIndex;
	int endIndex;
	int durationCount;
} Context;


extern State_t state;

extern int echoes;
extern USB_JoystickReport_Input_t last_report;

/**
 * Init the report manager for the given context
 * 
 * \param context the context to initialize for the selected bot instance
 */
void InitReport(Context* context);

/**
 * Get the next joystick report for the host
 * 
 * \param context    the context holding the current state, index, endIndex and waiting duration
 * \param ReportData the joystick report data holder
 * \return           a pointer to the current command sent or nullptr if none
 */
Command* GetNextReport(Context* context, USB_JoystickReport_Input_t* const ReportData);
#endif