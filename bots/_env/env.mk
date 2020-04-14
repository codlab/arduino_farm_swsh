#
#             LUFA Library
#     Copyright (C) Dean Camera, 2014.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# MCU Types: 
# at90usb1286 for Teensy 2.0++
# atmega16u2 for Arduino UNO R3
# atmega32u4 for Arduino Micro/Teensy 2.0

ROOT_FOLDER  = ../..

MCU          ?= atmega16u2
ARCH         ?= AVR8
F_CPU        ?= 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s

SRC_CONFIG   = $(ROOT_FOLDER)/Config/Descriptors.c $(LUFA_SRC_USB)
SRC_CORE     = $(ROOT_FOLDER)/core/serial_report.c $(ROOT_FOLDER)/core/millis.c $(ROOT_FOLDER)/core/action.c $(ROOT_FOLDER)/core/default_sequences.c $(ROOT_FOLDER)/core/usb_device.c $(ROOT_FOLDER)/core/calendar_skip.c $(ROOT_FOLDER)/core/day_skip.c
SRC_EXPANDED = $(SRC_CONFIG) $(SRC_CORE)
LUFA_PATH    = $(ROOT_FOLDER)/LUFA/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -I$(ROOT_FOLDER)/Config/ $(OPTIONALS)
LD_FLAGS     =