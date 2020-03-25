#if defined USE_LINK_CODE && defined USE_RANDOM_CODE && defined INITIAL_RAND_SEED && defined LINK_CODE
#ifndef AUTO_HOST_INCLUDED
#define AUTO_HOST_INCLUDED

#include "../../core/usb_device.h"

/*------------------------------------------*/
// INSTRUCTION
// -> You MUST stand in front of an active den (with watts already collected)
// -> Be careful if time is synced and past mid-night, you will start able to collect watts
// -> You MUST not have connected to the internet at the beginning
// -> You are adviced to have wired connection for stable and fast internet
// -> If you use WiFi you probably need to adjust your connection time below in code
// -> This program relies on clients to be ready before 1 minute mark, otherwise the raid fails
// -> Program will close the game when raid starts, and it may freeze on client's side for 10-15 seconds
// -> You have to start this program at the Change Grip/Order menu
// -> It takes ~3 minutes 25 seconds to host a raid

// -> Use optional link code or not? (true/false)
bool autohost_useLinkCode = USE_LINK_CODE >= 1;

// -> Use random code (if m_useLinkCode = true)
// -> m_seed range is 0 to 255, same seed will always generate the same link code sequence
// -> As long as the board is not unplugged, the sequence will go random forever
// -> If the board is unplugged, the squence will start at the beginning again
bool autohost_useRandomCode = USE_RANDOM_CODE >= 1;
uint8_t autohost_initial_rand_seed = INITIAL_RAND_SEED;

// -> Set optional link code here (if m_useLinkCode = true, m_useRandomCode = false)
// -> e.g 4501: autohost_linkCode = 4501;
// -> e.g 0389: autohost_linkCode = 389;
int autohost_linkCode = LINK_CODE;
/*------------------------------------------*/

#endif
#endif