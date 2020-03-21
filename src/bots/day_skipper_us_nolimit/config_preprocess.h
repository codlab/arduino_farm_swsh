#ifdef DAY_TO_SKIP
#ifndef DAY_SKIPPER_US_NOLIMIT_INCLUDED
#define DAY_SKIPPER_US_NOLIMIT_INCLUDED

#warning "OVERRIDE DAY SKIP US CONFIG"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have y-comm glitch active and system time unsynced
// -> You MUST set your current date to Janurary 1st or any month with 31 days, year doesn't matter
// -> It takes ~31 seconds per 30 days

// -> Set number of days you want to skip,can't be greater than 4294967295
unsigned long us_dayToSkip = DAY_TO_SKIP;
/*------------------------------------------*/

#endif
#endif