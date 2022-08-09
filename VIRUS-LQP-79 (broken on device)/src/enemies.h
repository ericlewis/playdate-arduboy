#ifndef ENEMIES_H
#define ENEMIES_H

#include "globals.h"
#include "bitmaps.h"
#include "player.h"

// constants /////////////////////////////////////////////////////////////////

#define ENEMY_FACING_WEST        0
#define ENEMY_FACING_EAST        1

#define ZOMBIE_FRAME_SKIP        2
#define ZOMBIE_FRAME_COUNT       8
#define ZOMBIE_MAX               24
#define ZOMBIE_WIDTH             16
#define ZOMBIE_HEIGHT            16

#define ZOMBIE_SPEED             1
#define ZOMBIE_STEP_DELAY        3

#define ZOMBIE_FLASH_TIME        5


// structures ////////////////////////////////////////////////////////////////

struct Enemy
{
  public:
    int x;
    int y;
    byte frame;
    byte direction;
    byte health;
    byte active;
    byte flashTime;
    byte type;
};

// globals ///////////////////////////////////////////////////////////////////

extern Enemy zombies[ZOMBIE_MAX];


// method prototypes /////////////////////////////////////////////////////////

void setZombie(Enemy& obj, int x, int y, byte type);
bool spawnZombie();
bool addZombie(int x, int y);
void updateZombie(Enemy& obj);
void updateZombies();
void drawZombie(Enemy& obj);
void drawZombies();
bool zombieHealthOffset(Enemy& obj, char amount);
bool zombieCollision(Enemy& obj, int x, int y, char w, char h);
void clearZombies();
void zombieCollide(int &x, int &y, bool horizontal, char &vel, char w, char h);

#endif
