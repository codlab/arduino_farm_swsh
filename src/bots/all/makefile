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
TARGET       = MultiBot
SRC_EXPANDED = main.c $(ROOT_FOLDER)/Config/Descriptors.c $(LUFA_SRC_USB) $(ROOT_FOLDER)/core/action.c $(ROOT_FOLDER)/core/usb_device.c
SRC_AUTO     = ../auto_3day_skipper/Auto3DaySkipper.c ../auto_fossil/AutoFossil.c ../auto_host/AutoHost.c ../auto_loto/AutoLoto.c
SRC_FARMERS  = ../berry_farmer/BerryFarmer.c ../box_release/BoxRelease.c ../turbo_a/TurboA.c ../watt_farmer/WattFarmer.c
SRC_DAY_US   = ../day_skipper_us/DaySkipper_US.c ../day_skipper_us_nolimit/DaySkipper_US_NoLimit.c
SRC_DAY_EU   = ../day_skipper_eu/DaySkipper_EU.c ../day_skipper_eu_nolimit/DaySkipper_EU_NoLimit.c
SRC_DAY_JP   = ../day_skipper_jp/DaySkipper_JP.c ../day_skipper_jp_nolimit/DaySkipper_JP_NoLimit.c
SRC          = $(SRC_AUTO) $(SRC_FARMERS) $(SRC_DAY_EU) $(SRC_DAY_US) $(SRC_DAY_JP) $(SRC_EXPANDED)
LUFA_PATH    = $(ROOT_FOLDER)/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -I$(ROOT_FOLDER)/Config/
LD_FLAGS     =

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk