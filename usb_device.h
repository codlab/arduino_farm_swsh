#ifndef USB_DEVICE_INCLUDED
#define USB_DEVICE_INCLUDED

#define ECHOES 2

typedef enum {
	PROCESS,
	DONE
} State_t;


extern bool m_useRandomCode;
extern uint8_t m_seed;
extern State_t state;

extern int echoes;
extern USB_JoystickReport_Input_t last_report;

extern int durationCount;
#endif