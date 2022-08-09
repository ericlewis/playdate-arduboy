#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "globals.h"

#define ENEMY_ONE                 0
#define ENEMY_TWO                 1
#define OBJECT_THREE              2
#define FLOOR_ONE                 3
#define FLOOR_TWO                 4
#define FLOOR_THREE               5
#define FLOOR_FOUR                6
#define FLOOR_FIVE                7
#define ENEMY_BULLET              8

#define ENEMY                     0

#define ENEMY_MOVING              0
#define ENEMY_SHOTING             1

#define ENEMY_BOX                 0
#define ENEMY_JUMPER              1
#define ENEMY_MOVER               2
#define ENEMY_SHOOTER             3

#define PICKUP_BLACK_CARD         0     // opens level door
#define PICKUP_WHITE_CARD         1     // opens normal door
#define PICKUP_BATTERY            2     // 1 extra life
#define PICKUP_BULLET             3     // 1 shot
#define PICKUP_CHIP               4     // extra points
#define TELEPORT                  5     // TELEPORT
#define SWITCH_OFF                6     // TOGGLE ELEMENTS IS OFF
#define SWITCH_ON                 7     // TOGGLE ELEMENTS IS OFF

#define FLOOR_TILE                0
#define FLOOR_BOX                 1
#define FLOOR_SPIKE               2
#define FLOOR_PIRAMIDE            3
#define FLOOR_PIT                 4

#define SCORE_BLACK_CARD          100
#define SCORE_WHITE_CARD          10
#define SCORE_LIFE                20
#define SCORE_BULLET              30
#define SCORE_CHIP                200


byte objectFrame = 0;

struct Element
{
  public:
    int x, y;
    byte characteristics = 0b00000000;   //this byte holds all the enemies characteristics
    //                       ||||||||
    //                       |||||||└->  0 \
    //                       ||||||└-->  1  | these 3 bits are used to determine kind of sprite to use
    //                       |||||└--->  2 /
    //                       ||||└---->  3 \  these 2 bits are used for direction
    //                       |||└----->  4 /  00 = NORTH / 01 = EAST / 10 = SOUTH / 11 = WEST
    //                       ||└------>  5 this enemy/object can hurt you         (0 = false / 1 = true)
    //                       |└------->  6 this object is movable by player       (0 = false / 1 = true)
    //                       └-------->  7 this object can be picked up by player (0 = false / 1 = true)
    byte frame;
};

Element elements[8];

///////////////// DRAW ENEMIES //////////////
/////////////////////////////////////////////
void drawEnemies(bool enemyOneOrTwo)
{
  switch (elements[enemyOneOrTwo].characteristics & 0b00000111)
  {
    case ENEMY_BOX:
      sprites.drawPlusMask(elements[enemyOneOrTwo].x, elements[enemyOneOrTwo].y + currentRoomY, enemies_plus_mask, ((elements[enemyOneOrTwo].characteristics & 0b00011000) >> 3));
      break;
    case ENEMY_JUMPER:
      sprites.drawPlusMask(elements[enemyOneOrTwo].x, elements[enemyOneOrTwo].y + currentRoomY, enemies_plus_mask, 4 + (elements[enemyOneOrTwo].frame));
      break;
    case ENEMY_MOVER:
      sprites.drawPlusMask(elements[enemyOneOrTwo].x, elements[enemyOneOrTwo].y + currentRoomY, enemies_plus_mask, 8 + ((elements[enemyOneOrTwo].characteristics & 0b00011000) >> 3));
      break;
    case ENEMY_SHOOTER:
      break;
  }
}

void drawEnemyOne()
{
  drawEnemies(0);
}

void drawEnemyTwo()
{
  drawEnemies(1);
}



///////////////// DRAW ELEMENTS /////////////
/////////////////////////////////////////////
void drawObjectChangeable()
{
  if (arduboy.everyXFrames(8)) elements[2].frame = (++elements[2].frame) % 6;
  sprites.drawPlusMask(elements[2].x + 4, elements[2].y + currentRoomY + 6, elements_plus_mask, elements[2].frame + (6 * ((elements[2].characteristics & 0b00000111))));
}

///////////////// DRAW SPECIAL FLOOR ////////
/////////////////////////////////////////////

void drawObjectFixedOne()
{
  sprites.drawPlusMask(elements[3].x - 3, elements[3].y + currentRoomY + 9, floorTile_plus_mask, ((elements[3].characteristics & 0b00000111)));
}

void drawObjectFixedTwo()
{
  sprites.drawPlusMask(elements[4].x - 3, elements[4].y + currentRoomY + 9, floorTile_plus_mask, ((elements[4].characteristics & 0b00000111)));
}

void drawObjectFixedThree()
{
  sprites.drawPlusMask(elements[5].x - 3, elements[5].y + currentRoomY + 9, floorTile_plus_mask, ((elements[5].characteristics & 0b00000111)));
}

void drawObjectFixedFour()
{
  sprites.drawPlusMask(elements[6].x - 3, elements[6].y + currentRoomY + 9, floorTile_plus_mask, ((elements[6].characteristics & 0b00000111)));
}

void drawObjectFixedFive()
{
  sprites.drawPlusMask(elements[7].x - 3, elements[7].y + currentRoomY + 9, floorTile_plus_mask, ((elements[7].characteristics & 0b00000111)));
}


void drawBulletEnemy()
{

}


#endif
