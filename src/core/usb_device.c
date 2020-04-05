/*
    Pokemon Sword/Shield bots using Arduino
    Copyright (C) 2020  brianuuuSonic / codlab

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


	Contextually based on the LUFA library's Low-Level Joystick Demo
		(C) Dean Camera
	Contextually based on the HORI's Pokken Tournament Pro Pad design
		(C) HORI

	For more information, please see the README.md available at https://github.com/codlab/arduino_farm_swsh
*/

#include <avr/pgmspace.h>

#include "context.h"
#include "../Joystick.h"
#include "millis.h"
#include "action.h"
#include "usb_device.h"
#include "serial_report.h"

Context context = {
	.state = PROCESS,
	.next_state = PROCESS,
	.commandIndex = 0,
	.endIndex = 0,
	.durationCount = 0,
	.bot = MissingNo,
	.botState = ON,
	.set = nullptr
};

void set(const char* set);

int echoes = 0;
USB_JoystickReport_Input_t last_report;

int main(void) {
	// We'll start by performing hardware and peripheral setup.
	SetupHardware();

	init_millis(F_CPU);
	// We'll then enable global interrupts for our use.
	GlobalInterruptEnable();
	// Once that's done, we'll enter an infinite loop.

	reportInit();

	for (;;)
	{
		// We need to run our task to process and deliver data for our IN and OUT endpoints.
		HID_Task();
		// We also need to run the main USB management task.
		USB_USBTask();
	}
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// We need to disable clock division before initializing the USB hardware.
	//clock_prescale_set(clock_div_1);
	// We can then initialize our hardware and peripherals, including the USB stack.

	#ifdef ALERT_WHEN_DONE
	// Both PORTD and PORTB will be used for the optional LED flashing and buzzer.
	#warning LED and Buzzer functionality enabled. All pins on both PORTB and \
PORTD will toggle when printing is done.
	DDRD  = 0xFF; //Teensy uses PORTD
	PORTD =  0x0;
                  //We'll just flash all pins on both ports since the UNO R3
	DDRB  = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
	PORTB =  0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
	#endif
	// The USB stack should be initialized last.
	USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	// We setup the HID report endpoints.
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

	// We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
	// We can handle two control requests: a GetReport and a SetReport.

	// Not used here, it looks like we don't receive control request from the Switch.
}

// Prepare the next report for the host.
void PrepareReport(USB_JoystickReport_Input_t* const ReportData) {
	memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;
}

Command *sequences = nullptr;

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
	// If the device isn't connected and properly configured, we can't do anything here.
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	fechCurrentMillis();
	checkReceived(&context);
	checkSend(&context);

	switch(context.botState) {
		case PAUSE:
		case OFF:
			return;
		default: { /*continue*/ }
	}

	// We'll start with the OUT endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
	// We'll check to see if we received something on the OUT endpoint.
	if (Endpoint_IsOUTReceived()) {
		// If we did, and the packet has data, we'll react to it.
		if (Endpoint_IsReadWriteAllowed()) {
			// We'll create a place to store our data received from the host.
			USB_JoystickReport_Output_t JoystickOutputData;
			// We'll then take in that data, setting it up in our storage.
			while(Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
			// At this point, we can react to this data.

			// However, since we're not doing anything with this data, we abandon it.
		}
		// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
		Endpoint_ClearOUT();
	}

	// We'll then move on to the IN endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
	// We first check to see if the host is ready to accept data.
	if (Endpoint_IsINReady()) {
		// We'll create an empty report.
		USB_JoystickReport_Input_t JoystickInputData;
		PrepareReport(&JoystickInputData);

		// Repeat ECHOES times the last report
		if (echoes > 0) {
			memcpy(&JoystickInputData, &last_report, sizeof(USB_JoystickReport_Input_t));
			echoes--;
		} else {
			// or get the new report
			// We'll then populate this report with what we want to send to the host.

			Command* command = nullptr;

			//if begin of a session
			if(nullptr == sequences) {
				context.commandIndex = 0;
				context.state = context.next_state;
				sequences = GetNextReport(&context, &JoystickInputData);
			}

			//if the session gave a pointer to sequences
			//the sequences is a variables which is retained, loop after loop it will be valid until "finished"
			if(nullptr != sequences) {
				command = get_command(&context, sequences);

				//if we have a command to do (the sequences is not finished)
				if (nullptr != command) {
					Command temp;
					memcpy_P(&temp, command, sizeof(Command));

					report_action(&JoystickInputData, &temp);
					goto_next(&context, &temp); //after, go to the next command to execute

					//we have a command to execute, we execute it
					// Prepare to echo this report
					memcpy(&last_report, &JoystickInputData, sizeof(USB_JoystickReport_Input_t));
					echoes = ECHOES;
				}
			}

			if(nullptr == command || context.commandIndex == -1) {
				sequences = nullptr;
			}
		}

		// Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
		while(Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
		// We then send an IN packet on this endpoint.
		Endpoint_ClearIN();
	}
}