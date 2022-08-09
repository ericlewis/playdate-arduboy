#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"

void stateMenuPlay()
{
  scorePlayer = 0;
  player.set();
  globalCounter = 0;
  level = LEVEL_TO_START_WITH - 1;
  gameState = STATE_GAME_NEXT_LEVEL;
}

void stateGamePlaying()
{
  checkOrderOfObjects(currentRoom, level);
  drawRoom();
  if (!bitRead(player.characteristics, 5))
  {
    if (!bitRead(player.characteristics, 4)) checkInputs();
  }
  else
  {
    if (player.steps < 5)
    {
      if (arduboy.everyXFrames(2)) walkThroughDoor();
    }
    else
    {
      player.isOnTile = goToTile(currentRoom, level);
      currentRoomY = setCurrentRoomY(player.isOnTile);
      currentRoom = goToRoom(currentRoom, level);
      player.x = translateTileToX (player.isOnTile) + offsetXAfterDoor(player.isOnTile);
      player.y = translateTileToY (player.isOnTile) + offsetYAfterDoor(player.isOnTile) + currentRoomY ;
      player.steps = 0;
      enterRoom(currentRoom, level);
      bitClear (player.characteristics, 5);
      bitSet (player.characteristics, 6);
      gameState = STATE_GAME_NEXT_ROOM;
      return; //don't update the enemies yet, first go through the door
    }
  }
  updatePlayer();
  if (!bitRead(player.characteristics, 4)) updateEnemies();
  drawHUD();
  //Serial.println(tileFromXY(player.x, player.y - currentRoomY));

}


void stateGameNextRoom()
{
  checkOrderOfObjects(currentRoom, level);
  drawRoom();

  {
    if (player.steps < 5)
    {
      if (arduboy.everyXFrames(2)) walkThroughDoor();
    }
    else
    {
      player.steps = 0;
      bitClear (player.characteristics, 6);
      gameState = STATE_GAME_PLAYING;
    }
  }
  drawHUD();
}

void stateGameNextLevel()
{
  level++;
  currentRoom = 0;
  player.isOnTile = TILE_GAME_STARTS_ON;
  currentRoomY = -14;
  player.x = translateTileToX (player.isOnTile);
  player.y = translateTileToY (player.isOnTile) + currentRoomY ;
  buildRooms(level);
  enterRoom(currentRoom, level);
  gameState = STATE_GAME_PLAYING;
}


void stateGamePause()
{
  sprites.drawSelfMasked(0, 0, pauseScreen, 0);
  drawNumbers(43, 54, scorePlayer, BIG_FONT);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_GAME_PLAYING;
}

void stateGameOver()
{
  sprites.drawSelfMasked(0, 0, gameOverScreen, 0);
  drawNumbers(43, 54, scorePlayer, BIG_FONT);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateGameTransporting()
{
  //playerTurnAround();
  currentRoom = transportToRoom(currentRoom);
  player.x = translateTileToX (player.isOnTile) ;
  player.y = translateTileToY (player.isOnTile) + currentRoomY ;
  player.steps = 0;
  enterRoom(currentRoom, level);
  //checkOrderOfObjects(currentRoom, level);
  gameState = STATE_GAME_PLAYING;
}

#endif
