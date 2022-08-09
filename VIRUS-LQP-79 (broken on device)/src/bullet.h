#ifndef BULLET_H
#define BULLET_H

#include "enemies.h"
#include "level.h"

// constants /////////////////////////////////////////////////////////////////

#define BULLET_MAX        6
#define BULLET_WIDTH      2
#define BULLET_HEIGHT     2
#define BULLET_DIRECTIONS 8

extern const signed char BulletXVelocities[8];

// structures ////////////////////////////////////////////////////////////////

struct Bullet {
  int x;
  int y;
  char vx;
  char vy;
  byte active;
};

// globals ///////////////////////////////////////////////////////////////////

extern Bullet bullets[BULLET_MAX];

// method prototypes /////////////////////////////////////////////////////////

bool setBullet(Bullet& obj, int x, int y, char vx, char vy);
void addBullet(int x, int y, byte direction, char vx, char vy);
void updateBullet(Bullet& obj);
void updateBullets();
void drawBullets();
void clearBullets();

#endif
