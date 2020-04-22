include ../_env/env.mk

TARGET       = MultiBot
SRC_AUTO     = ../auto_3day_skipper/Auto3DaySkipper.c ../auto_fossil/AutoFossil.c ../auto_host/AutoHost.c ../auto_loto/AutoLoto.c
SRC_FARMERS  = ../berry_farmer/BerryFarmer.c ../box_manage/BoxManager.c ../turbo_a/TurboA.c ../watt_farmer/WattFarmer.c
SRC_DAY_US   = ../day_skipper_us/DaySkipper_US.c ../day_skipper_us_nolimit/DaySkipper_US_NoLimit.c
SRC_DAY_EU   = ../day_skipper_eu/DaySkipper_EU.c ../day_skipper_eu_nolimit/DaySkipper_EU_NoLimit.c
SRC_DAY_JP   = ../day_skipper_jp/DaySkipper_JP.c ../day_skipper_jp_nolimit/DaySkipper_JP_NoLimit.c
SRC_CRASH_FREE_EGG   = ../crashfreeegg_dup/CrashFreeEgg.c
SRC_TWITCH   = ../twitch_plays/TwitchPlays.c
SRC          = main.c $(SRC_AUTO) $(SRC_FARMERS) $(SRC_DAY_EU) $(SRC_DAY_US) $(SRC_DAY_JP) $(SRC_CRASH_FREE_EGG) $(SRC_EXPANDED) $(SRC_TWITCH)

# Default target
all:

include ../_env/lufa.mk