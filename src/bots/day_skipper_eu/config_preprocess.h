#if defined DAY && defined MONTH && defined YEAR && defined DAY_TO_SKIP
#ifndef DAY_SKIPPER_INCLUDED
#define DAY_SKIPPER_INCLUDED

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> Set your date info here, date must be between 1/1/2000 to 31/12/2060 (22280 days advance)
// -> Use this website to check your end date: https://www.timeanddate.com/date/dateadd.html
// -> MAKE SURE the end date is within range, if your seed is over 22280 skips, enter the game
// to register the time skip, set a new start date, update this program and start again!
// -> It takes ~43 seconds per 30 days

#include "../../core/calendar_skip.h"

CalendarSkip calendarEU = {
    .day = DAY,
    .month = MONTH,
    .year = YEAR,
    .dayToSkip = DAY_TO_SKIP
};
/*------------------------------------------*/

#endif
#endif