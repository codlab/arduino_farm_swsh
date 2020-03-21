#if defined FIRST_FOSSIL_TOP_SLOT && defined SECOND_FOSSIL_TOP_SLOT && defined TIME_BEFORE_SR && defined AUTO_SOFT_RESET
#ifndef AUTO_FOSSIL_INCLUDED
#define AUTO_FOSSIL_INCLUDED

#warning "OVERRIDE FOSSIL CONFIG"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST have set text speed to FAST
// -> You MUST have the fossil pokemon register in the PokeDex
// -> You MUST turn off asking for nickname
// -> You have to start this program at the Change Grip/Order menu
// -> Save and talk to Cara Liss and check which slot of the fossil you wish to choose
// -> Make sure the number of both fossils is less than m_timesBeforeSR
// -> When music plays, keep an eye on the game to see if you got a shiny
// -> It takes ~18 seconds per fossil and ~34 seconds to soft reset

bool m_firstFossilTopSlot = FIRST_FOSSIL_TOP_SLOT == 1 : true : false; 	// true (top), false (bottom)
bool m_secondFossilTopSlot = SECOND_FOSSIL_TOP_SLOT == 1 : true : false; 	// true (top), false (bottom)
int m_timesBeforeSR = TIME_BEFORE_SR; 			// Number of fossil to get before restarting/stopping
bool m_autoSoftReset = AUTO_SOFT_RESET;		// true: Auto soft reset after complete, false: stop when done
/*------------------------------------------*/

#endif
#endif