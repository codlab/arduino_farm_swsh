#include <avr/pgmspace.h>
#include "sram.h"

void readCommand(const Command* from, Command* to) {
  memcpy_P(to, from, sizeof(Command));
}