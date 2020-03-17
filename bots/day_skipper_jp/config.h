#ifndef DAY_SKIPPER_JP_INCLUDED
#define DAY_SKIPPER_JP_INCLUDED

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> Set your date info here, date must be between 2000/1/1 to 2060/12/31 (22280 days advance)
// -> Use this website to check your end date: https://www.timeanddate.com/date/dateadd.html
// -> MAKE SURE the end date is within range, if your seed is over 22280 skips, enter the game
// to register the time skip, set a new start date, update this program and start again!
// -> It takes ~31 seconds per 30 days

int m_day = 3;
int m_month = 5;
int m_year = 2024;
int m_dayToSkip = 290;
/*------------------------------------------*/

#endif