#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "stage.h"

boolean objectVisible;

void stateMenuPlay()
{
  arduboy.clear();
  level = level = LEVEL_TO_START_WITH - 1;
  scorePlayer = 0;
  initPlayfield();
  removeFlag();
  modeIsSelected = false;
  selectorXpos = 4;
  selectorYpos = 4;
  timer = 24;
  testingSwitch = 0;
  canMoveGemsDown = true;
  giveExtraScore = false;
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, splashScreen, i);
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(64 * i, 48, tubes, i);
  sprites.drawSelfMasked(33, 52, gameWords, gameMode - 1);
  if (arduboy.justPressed(RIGHT_BUTTON) && (gameMode < 3)) gameMode++;
  if (arduboy.justPressed(LEFT_BUTTON) && (gameMode > 1)) gameMode--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.clear();
    gameState = STATE_GAME_PLAYING;
  }
};



void stateGamePlaying()
{
  if (arduboy.everyXFrames(45)) timer--;
  if (timer < 0)
  {
    gameState = STATE_GAME_OVER;
    delay(500);
  }
  if (arduboy.everyXFrames(600))
  {
    blinkTubes = true;
    flickerPlace = (random(0, 6) * 9);
  }
  if (arduboy.everyXFrames(4))
  {

    selectorFrame = !selectorFrame;
    blinkFrame++;
    if (blinkFrame > 12)
    {
      blinkFrame = 0;
      blinkTubes = false;
    }
    animationFrame++;
    if (animationFrame > 11) animationFrame = 0;

    smokeFrame++;
    if (smokeFrame > 23)
    {
      smokeFrame = 0;
      smokeLeftRight = !smokeLeftRight;
    }
  }
  updateStage();
  removeGems();
  while (canMoveGemsDown) moveGemsDown();
  addNewGems();

  checkInputs();
};

void stateGamePause()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, splashScreen, i);
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(64 * i, 48, tubes, i);
  sprites.drawSelfMasked(33, 52, gameWords, 3);
  if (arduboy.justPressed(A_BUTTON))
  {
    arduboy.clear();
    gameState = STATE_GAME_PLAYING;
  }
}


void stateGameOver()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, splashScreen, i);
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(64 * i, 48, tubes, i);
  drawScore(41, 52, SCORE_BIG);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.clear();
    gameState = STATE_MENU_MAIN;
  }
};


#endif
