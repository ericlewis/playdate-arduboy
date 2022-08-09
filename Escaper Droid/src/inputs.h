#ifndef INPUTS_H
#define INPUTS_H

#include "globals.h"
#include "player.h"
#include "collision.h"


void checkInputs()
{
  if (arduboy.everyXFrames(2))
  {
    byte testingTile = tileFromXY(player.x, player.y - currentRoomY);
    if (testingTile < 25) player.isOnTile = tileFromXY(player.x, player.y - currentRoomY);
    //Serial.println(player.isOnTile);
    if (arduboy.pressed(UP_BUTTON))
    {
      bitClear(player.characteristics, 0);
      bitClear(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, NORTH))
      {
        if (hitObjects(player.x, player.y - currentRoomY, NORTH, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }


    else if (arduboy.pressed(RIGHT_BUTTON))
    {
      bitSet(player.characteristics, 0);
      bitClear(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, EAST))
      {
        if (hitObjects(player.x, player.y - currentRoomY, EAST, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }

    else if (arduboy.pressed(DOWN_BUTTON))
    {
      bitClear(player.characteristics, 0);
      bitSet(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, SOUTH))
      {
        if (hitObjects(player.x, player.y - currentRoomY, SOUTH, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }

    else if (arduboy.pressed(LEFT_BUTTON))
    {
      bitSet(player.characteristics, 0);
      bitSet(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, WEST))
      {
        if (hitObjects(player.x, player.y - currentRoomY, WEST, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }
  }

  if (arduboy.justPressed(A_BUTTON))gameState = STATE_GAME_PAUSE;
  if (arduboy.justPressed(B_BUTTON))
  {

  }
}

////Moving the Enemies /////
////////////////////////////
void moveEnemies(int enemyX, int enemyY, byte directionFacing, bool enemy)
{
  switch (directionFacing)
  {
    case NORTH:
      elements[enemy].y -= 1;
      elements[enemy].x -= 2;
      break;
    case EAST:
      elements[enemy].y -= 1;
      elements[enemy].x += 2;
      break;
    case SOUTH:
      elements[enemy].y += 1;
      elements[enemy].x += 2;
      break;
    case WEST:
      elements[enemy].y += 1;
      elements[enemy].x -= 2;
      break;
  }
}

void enemyTurn(bool enemy, bool leftOrRight)
{
  byte test = (((elements[enemy].characteristics & 0b00011000) >> 3) - 1 + (leftOrRight * 2)) & 0b00000011;;
  elements[enemy].characteristics = (elements[enemy].characteristics & 0b11100111) + (test << 3);
}


void updateEnemies()
{
  if (arduboy.everyXFrames(6))
  {
    for (byte i = 0; i < 2; i++)
    {
      elements[i].frame = ((++elements[i].frame) % 4);
      if (levels[level - 1][ELEMENTS_DATA_START_AT_BYTE + i + (BYTES_USED_FOR_EVERY_ROOM * currentRoom)])
      {
        switch (elements[i].characteristics & 0b00000111)
        {
          case ENEMY_BOX:
            if (!hitBorders(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY, i)))
            {
              moveEnemies(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, i);
            }
            else enemyTurn(i, TURN_RIGHT);
            break;
          case ENEMY_JUMPER:
            if (!hitBorders(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY, i)))
            {
              moveEnemies(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, i);
              if (checkIfOnCenterTile (elements[i].x, elements[i].y))
              {
                // check if left is an empty tile
                if (!hitBorders(elements[i].x, elements[i].y, ((elements[i].characteristics & 0b00011000) >> 3) - 1, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, ((elements[i].characteristics & 0b00011000) >> 3) - 1, ENEMY, i)))
                {
                  enemyTurn(i, TURN_LEFT);
                }
              }
            }
            else enemyTurn(i, TURN_RIGHT);
            break;
          case ENEMY_MOVER:
            if (!hitBorders(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY, i)))
            {
              moveEnemies(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, i);
            }
            else enemyTurn(i, TURN_LEFT);
            break;
          case ENEMY_SHOOTER:
            break;
        }
      }
    }
  }
}


#endif
