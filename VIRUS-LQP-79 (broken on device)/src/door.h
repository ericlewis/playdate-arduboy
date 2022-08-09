#ifndef DOOR_H
#define DOOR_H

#include <Arduboy2.h>

// constants /////////////////////////////////////////////////////////////////

#define DOOR_WIDTH              16
#define DOOR_HEIGHT             16

#define EXIT_FACING_SOUTH       0
#define EXIT_FACING_WEST        1
#define EXIT_FACING_NORTH       2
#define EXIT_FACING_EAST        3

#define EXIT_ON_SOUTH_BORDER    0
#define EXIT_ON_WEST_BORDER     496
#define EXIT_ON_NORTH_BORDER    240
#define EXIT_ON_EAST_BORDER     0


// structures ////////////////////////////////////////////////////////////////

struct Door {
  int x;
  int y;
  byte active;
  byte orientation;
  byte frame;
  byte counter;
  byte loseLifeCounter;
};

// globals ///////////////////////////////////////////////////////////////////

extern Door exitDoor;

// method prototypes /////////////////////////////////////////////////////////

void setDoorPosition(int x, int y);
void drawDoor();
bool checkDoorCollision();

#endif
