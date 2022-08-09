#include "globals.h"

// globals ///////////////////////////////////////////////////////////////////

Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);
unsigned long scorePlayer;
byte gameID;
byte gameState  = STATE_MENU_INTRO;
byte gameType = STATE_GAME_NEW;
byte globalCounter = 0;

// function implementations //////////////////////////////////////////////////

// burp
// returns the value a given percent distance between start and goal
// percent is given in 4.4 fixed point
short burp(short start, short goal, unsigned char step)
{
  short a = goal;
  short b = start;
  char sign = 0;

  if (start > goal)
  {
    a = start;
    b = goal;
    sign = -1;
  }
  else if (start < goal)
  {
    sign = 1;
  }

  start += sign * (1 + ((a - b) * step) / 16);
  if (a < b) return goal;

  return start;
}

