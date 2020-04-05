#ifndef BOT_INCLUDED
#define BOT_INCLUDED

typedef enum BotState {
	ON,
	PAUSE, //TODO add PAUSE
	OFF
} BotState;

typedef enum Bot {
	MissingNo, //a
	Auto3DaySkipper, //b
	AutoFossil, //c
	AutoHost, //d
	AutoLoto, //e
	BerryFarmer, //f
	BoxRelease, //g
	CrashFreeEggDup, //h
	DaySkipperEU, //i
	DaySkipperEUNoLimit, //j
	DaySkipperJP, //k
	DaySkipperJPNoLimit, //l
	DaySkipperUS, //m
	DaySkipperUSNoLimit, //n
	TurboA, //o
	WattFarmer, //p
    MissingNo1, //q
    MissingNo2, //r
    MissingNo3, //s
    MissingNo4, //t
    MissingNo5, //u
    MissingNo6, //v
    MissingNo7, //w
    MissingNo8, //x
    MissingNo9, //y
    MissingNo10, //z
    CustomBot1, //A
    CustomBot2, //B
    CustomBot3, //C
    CustomBot4, //D
    CustomBot5, //E
    CustomBot6, //F
    CustomBot7, //G
    CustomBot8, //H
    CustomBot9, //I
    CustomBot10, //J
    CustomBot11, //K
    CustomBot12, //L
    CustomBot13, //M
    CustomBot14, //N
    CustomBot15, //O
    CustomBot16, //P
    CustomBot17, //Q
    CustomBot18, //R
    CustomBot19, //S
    CustomBot20, //T
    CustomBot21, //U
    CustomBot22, //V
    CustomBot23, //W
    CustomBot24, //X
    CustomBot25, //Y
    CustomBot26 //Z
} Bot;

#endif