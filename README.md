# Pokemon Shield & Sword Bots through Arduino

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/codlab/arduino_farm_swsh.png?branch=master)](https://travis-ci.org/codlab/arduino_farm_swsh)

This repository is an evolution of the Auto Controller by brianuuuSonic original work. The intent is to provide a modularity and clean environment for users to :

  - easily fork the project
  - make modification
  - add their own bots

# Information

Based on the LUFA library's Low-Level Joystick Demo (C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design (C) HORI

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken Tournament Pro Pad as a Pro Controller. Physical design limitations prevent the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the exception of Home and Capture. Descriptor modification allows us to unlock these buttons for our use.

This project implements a modified version of HORI's Pokken Tournament Pro Pad USB descriptors to allow for the creation of custom controllers for the Nintendo Switch. This project includes the following codes:

# Features

- DaySkipper_US: Auto day skipper for US date arrangement (mm/dd/yyyy hh:mm:AM)
- DaySkipper_EU: Auto day skipper for EU date arrangement (dd/mm/yyyy hh:mm)
- DaySkipper_JP: Auto day skipper for JP date arrangement (yyyy/mm/dd hh:mm) **FASTEST**
- DaySkipper_US_NoLimit: DaySkipper_US but without 22280 days limit
- DaySkipper_EU_NoLimit: DaySkipper_EU but without 22280 days limit
- DaySkipper_JP_NoLimit: DaySkipper_JP but without 22280 days limit **FASTEST**
- Auto3DaySkipper: Auto roll den to the 4th day, SR and repeats
- AutoLoto: Infinite loto reward grinding
- AutoFossil: Shiny Fossil grinding
- AutoHost: Auto hosting raids
- TurboA: A button masher (for digging duo)
- WattFarmer: Fast watt collector
- BerryFarmer: Fast berry farmer
- BoxRelease: Release all pokemon in PC boxes

# Instructions

## Configuration

Given the chosen bot, notice the `config.h` file, in those files are the specific settings to edit before compilation. Some other information are also stated there.

## Compiling

- compile every bots
```make all```

- cleaning
```make clean```

## Flashing

Depending on your OS, [MacOS](https://github.com/codlab/arduino_farm_swsh#flash-1) or [Windows](https://github.com/codlab/arduino_farm_swsh#flash) or Linux

## Incoming

New features coming soon :

- reading serial command

# Prerequisites

## Windows

### Software

- [WinAVR](https://sourceforge.net/projects/winavr/files/)
- [Flip (optional)](https://www.microchip.com/developmenttools/ProductDetails/flip)
- [Teensy loader (optional)](https://www.pjrc.com/teensy/loader_win10.html)

WinAVR is mandatory to manage the compilation and hex creation. The loaders depends on the board used to inject the USB's spoofer.

Install the proper software to have the compilation and flashing tools

### Flash

Start flip or teensy loader, select the hex file you want to flash, load the USB device and flash.


## MacOS

### Install

Id you don't have HomeBrew already installed

```
xcode-select --install
bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

AVR for MacOS and DFU Programmer

```
brew tap osx-cross/avr
brew install avr-gcc dfu-programmer
```

### Flash

```
sudo dfu-programmer atmega16u2 erase
sudo dfu-programmer atmega16u2 flash path/to/the/file.hex
sudo dfu-programmer atmega16u2 reset
```

Replace atmega16u2 with the proper Atmel type you have. Arduino UNO is for instance atmega16u2


# Videos

Basic Tutorial (MUST WATCH): https://youtu.be/y2xFf7e_KSU
v1.1.0 Update: https://youtu.be/OZ0yrr061-E
v2.0.0 Update: https://youtu.be/kR5WMZgpXJA
v3.1.0 Update: https://youtu.be/VgGTfOE_Flc

# Y-Comm Link Battle Day Spoofing Glitch:

1. Connect to internet
2. Do a y-com link battle
3. Wait until "trainer is found" prompt shows up
4. DC from server either with airplane mode on (turn it off afterwards!) or take your wired connected switch out from the dock, you can also Search for Local Friends on wifi-docked switch
5. Day skip glitch is now active, goto Pokemon Center to avoid crash and start skipping
(This works locally as well if you have 2 Switches)
Video Instruction: https://youtu.be/OZ0yrr061-E?t=80

# Change Log:

- `v4.0.0` Clean architecture and modular, move bot sequence to flash mem
- `v3.1.0` Added random link code for Auto Host
- `v3.0.0` Auto Host, Box Release initial release, increase HOME button after first time connect board from 50 to 60
- `v2.2.0` Watt Farmer, Berry farmer now speeds up with syncing time
- `v2.1.0` Berry Farmer initial release, added optional day to skip in Auto Loto
- `v2.0.4` Added 10 to Auto 3 Day Skipper, Auto Loto, Watt Farmer when going HOME screen or back to game
- `v2.0.3` Watt Farmer saves every 100,000W (50 skips)
- `v2.0.2` Changed HOME button press before SR in Auto Fossil and Auto 3 Day Skipper to 40
- `v2.0.1` Use array in No Limit Day Skipper, now can actually go infinite
- `v2.0.0` No Limit Day Skipper, Auto Fossil, Auto 3 Day Skipper, Turbo A, Watt Farmer initial release
- `v1.1.0` Day Skipper optimization, added Day Skipper for Japanese region, Auto Loto initial release
- `v1.0.0` US and EU Day Skipper initial release


# Credits

shinyquagsire23 - Original Switch-Fightstick code
june - Auto Hosting timing reference
[brianuuuSonic](https://www.youtube.com/channel/UCHV0EP9TifKSo7RERIbY1QA)

# Special Thanks

Just-Kim - Beta testing Auto Host, Box Surprise Trade timing reference
SaxManDan - Bata testing Auto Host