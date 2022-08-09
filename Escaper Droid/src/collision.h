#ifndef COLLISION_H
#define COLLISION_H

#include "globals.h"

boolean hitBorders(int objectX, int objectY, int directionFacing, bool playerOrEnemy)
{
  // check the borders of the room
  if (directionFacing < 0) directionFacing = 3;
  if (directionFacing > 3) directionFacing = 0;
  switch (directionFacing)
  {
    case NORTH:
      if (objectX + (2 * objectY) > 89 + (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
    case EAST:
      if (objectX - (2 * objectY) < 15 - (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
    case SOUTH:
      if (objectX + (2 * objectY) < 183 + (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
    case WEST:
      if (objectX - (2 * objectY) > -81 - (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
  }
  return true;
}


void setPlayerWalkingThroughDoor(int objectX, int objectY, byte directionFacing)
{
  switch (directionFacing)
  {
    case NORTH:
      if ((tileFromXY(objectX, objectY - currentRoomY) == 2) &&             // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, 7)) &&         // the door exists in this room
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, 3))           // the door is not closed
         )
      {
        bitSet (player.characteristics, 5);                                  // if all above, set the droid is walking through the door
        player.x = translateTileToX(2);
        player.y = translateTileToY(2) + currentRoomY;
      }
      break;
    case EAST:
      if ((tileFromXY(objectX, objectY - currentRoomY) == 10) &&            // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, 6)) &&         // the door exists in this room
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, 2))           // the door is not closed
         )
      {
        bitSet (player.characteristics, 5);                                 // if all above, set the droid is walking through the door
        player.x = translateTileToX(10);
        player.y = translateTileToY(10) + currentRoomY;
      }
      break;
    case SOUTH:
      if ((tileFromXY(objectX, objectY - currentRoomY) == 22) &&            // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, 5)) &&         // the door exists in this room
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, 1))           // the door is not closed
         )
      {
        bitSet (player.characteristics, 5);                                 // if all above, set the droid is walking through the door
        player.x = translateTileToX(22);
        player.y = translateTileToY(22) + currentRoomY;
      }
      break;
    case WEST:
      if ((tileFromXY(objectX, objectY - currentRoomY) == 14) &&            // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, 4)) &&         // the door exists in this room
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, 0))           // the door is not closed
         )
      {
        bitSet (player.characteristics, 5);                                 // if all above, set the droid is walking through the door
        player.x = translateTileToX(14);
        player.y = translateTileToY(14) + currentRoomY;
      }
      break;
  }
}


boolean checkborderHit(int objectX, int objectY, byte directionFacing)
{
  if (!hitBorders(objectX, objectY, directionFacing, PLAYER)) return false;
  else setPlayerWalkingThroughDoor(objectX, objectY, directionFacing);
  return true;
}


byte tileIsOccupied(byte tileTesting, bool playerOrEnemy, bool enemyTwo)
{
  if (tileTesting < 25)
  {
    currentlyOnTestingTile = itemsOrder[tileTesting + ITEMS_ORDER_TILES_START];
    if (currentlyOnTestingTile == EMPTY_PLACE) return false;
    if (playerOrEnemy)
    {
      if (currentlyOnTestingTile == PLAYER_DROID) return false;
    }
    else
    {
      //Serial.print(tileTesting);
      //Serial.print(" : ");
      //Serial.println(currentlyOnTestingTile);
      if (!enemyTwo && currentlyOnTestingTile == ENEMY_ONE) return false;
      else if (enemyTwo && currentlyOnTestingTile == ENEMY_TWO) return false;
      else if (currentlyOnTestingTile == PLAYER_DROID) return PLAYER_DROID;
    }
    return true;
  }
  else return false;
}


boolean hitObjects (int objectX, int objectY, int directionFacing, bool playerOrEnemy, bool enemy)
{
  if (directionFacing < 0) directionFacing = 3;
  if (directionFacing > 3) directionFacing = 0;
  switch (directionFacing)
  {
    case NORTH:
      testingTile = tileFromXY(objectX - 8, objectY - 4);
      break;
    case EAST:
      testingTile = tileFromXY(objectX + 8, objectY - 4);
      break;
    case SOUTH:
      testingTile = tileFromXY(objectX + 6, objectY + 3);
      break;
    case WEST:
      testingTile = tileFromXY(objectX - 6, objectY + 3);
      break;
  }
  //Serial.println(testingTile);
  byte test = tileIsOccupied(testingTile, playerOrEnemy, enemy);
  if (test > 0)
  {
    if (test == PLAYER_DROID) playerLosesLife();
    return true;
  }
  else return false;
}

void clearElement()
{
  bitClear(stageRoom[currentRoom].elementsActive, 5);
}

void checkObjectTypeAndAct()
{
  switch ((elements[2].characteristics & 0b00000111))
  {
    case PICKUP_BLACK_CARD: // opens level door
      if ((player.assets & 0b01100000) < 0b01100000)
      {
        clearElement();
        player.assets += 0b00100000;
        scorePlayer += SCORE_BLACK_CARD;
      }
      break;
    case PICKUP_WHITE_CARD: // opens normal door
      if ((player.assets & 0b00011000) < 0b00011000)
      {
        clearElement();
        player.assets += 0b00001000;
        scorePlayer += SCORE_WHITE_CARD;
      }
      break;
    case PICKUP_BATTERY:    // 1 extra life
      if (player.life < 3)
      {
        player.life++;
      }
      clearElement();
      scorePlayer += SCORE_LIFE;
      break;
    case PICKUP_BULLET:     // 1 shot
      if ((player.assets & 0b00000111) < 0b00000111)
      {
        player.assets++;
        clearElement();
        scorePlayer += SCORE_BULLET;
      }
      break;
    case PICKUP_CHIP:       // extra points
      clearElement();
      scorePlayer += SCORE_CHIP;
      break;
    case TELEPORT:          // TELEPORT
      gameState = STATE_GAME_TRANSPORTING;
      break;
    case SWITCH_OFF:        // SWITCH OFF
      break;
    case SWITCH_ON:          // SWITCH ON
      break;
  }
}


void decideOnCollision()
{
  //Serial.print("testing : ");
  //Serial.print(testingTile);
  switch (currentlyOnTestingTile)
  {
    case ENEMY_ONE:
      break;
    case ENEMY_TWO:
      break;
    case OBJECT_THREE:
      //Serial.print(" switching");
      checkObjectTypeAndAct();
      //itemsOrder[testingTile + ITEMS_ORDER_TILES_START] = 0;
      break;
    case FLOOR_ONE:
      break;
    case FLOOR_TWO:
      break;
    case FLOOR_THREE:
      break;
    case FLOOR_FOUR:
      break;
    case FLOOR_FIVE:
      break;

  }
}


#endif
