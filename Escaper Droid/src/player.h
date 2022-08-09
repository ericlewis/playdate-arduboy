#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

#define PLAYER_DROID            9
#define PLAYER_BULLET           10
#define PLAYER_IMUNE_TIME       30

#define PLAYER                  1

struct EscaperDroid
{
  public:
    int x, y;
    int life;
    byte characteristics;
    byte imuneTimer;
    byte steps;
    byte assets;
    byte isOnTile;

    void set()
    {
      characteristics = 0b10000110;   //this byte holds all the Escaperdroids characteristics
      //                  ||||||||
      //                  |||||||└->  0  | this 2 bits are used for direction
      //                  ||||||└-->  1  | 00 = NORTH / 01 = EAST / 10 = SOUTH / 11 = WEST
      //                  |||||└--->  2  The droid is visible                   (0 = false / 1 = true)
      //                  ||||└---->  3  The droid is imune                     (0 = false / 1 = true)
      //                  |||└----->  4  The droid is dying                     (0 = false / 1 = true)
      //                  ||└------>  5  The droid is going through a door      (0 = false / 1 = true)
      //                  |└------->  6  The droid is coming out a door         (0 = false / 1 = true)
      //                  └-------->  7  the droid's battery meter is visible   (0 = false / 1 = true)
      imuneTimer = PLAYER_IMUNE_TIME;
      steps = 0;
      life = 2;
      assets = 0b00000000;
      //         ||||||||
      //         |||||||└->  0  \ 
      //         ||||||└-->  1   | this 3 bits are used for amount of shots
      //         |||||└--->  2  /
      //         ||||└---->  3  \
      //         |||└----->  4   | this 2 bits are used for amount of white cards
      //         ||└------>  5  \
      //         |└------->  6   | this 2 bites are used for the amount of black cards
      //         └-------->  7   reserved
      isOnTile = TILE_GAME_STARTS_ON;
    }
};

EscaperDroid player;

void walkThroughDoor()
{
  if (bitRead(player.characteristics, 5) || bitRead(player.characteristics, 6)) player.steps++;
  switch (player.characteristics & 0b00000011)
  {
    case NORTH:
      if (currentRoomY < 0) currentRoomY++;
      else player.y -= 1;
      player.x -= 2;
      break;
    case EAST:
      if (currentRoomY < 0)currentRoomY++;
      else player.y -= 1;
      player.x += 2;
      break;
    case SOUTH:
      if (currentRoomY > -30)currentRoomY--;
      else player.y += 1;
      player.x += 2;
      break;
    case WEST:
      if (currentRoomY > -30)currentRoomY--;
      else player.y += 1;
      player.x -= 2;
      break;
  }
}

void playerLosesLife()
{
  if (!bitRead(player.characteristics, 3))
  {
    player.life -= 1;
    bitSet(player.characteristics, 3);
    if (player.life < 1) bitSet(player.characteristics, 4);
  }
}

void updatePlayer()
{
  if (bitRead(player.characteristics, 3))
  {
    if (arduboy.everyXFrames(4))
    {
      player.imuneTimer--;
      player.characteristics ^= 0b00000100;
    }
    if (player.imuneTimer < 1)
    {
      player.imuneTimer = PLAYER_IMUNE_TIME;
      bitClear(player.characteristics, 3);
    }
  }
  else if (player.life < 1)
  {
    gameState = STATE_GAME_OVER;
  }
}

void playerTurnAround()
{
  byte test = ((player.characteristics & 0b00000011) + 2) & 0b00000011;
  player.characteristics = (player.characteristics & 0b00000011) + (test);
}


void drawPlayer()
{
  if (bitRead(player.characteristics, 2))
  {
    sprites.drawPlusMask(player.x, player.y, droid_plus_mask, player.characteristics & 0b00000011);
  }
}

void drawBulletPlayer()
{

}

#endif
