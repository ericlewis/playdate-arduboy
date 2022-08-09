#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "playfield.h"

void stateGameInitLevel()
{
  playerScore = 0;
  AIScore = 0;
  scoreMax = howManyWins;
  setupGame();
  gameState = STATE_GAME_PLAYING ;
};



void stateGamePlaying()
{

  moveBall();
  AI_Logic();
  superKick();
  checkIfScored();
  checkCollisions();
  drawPlayArea();
  drawScores();
  checkAnimations();
  drawBall();
  checkWinLose();
  if (arduboy.everyXFrames(2))
  {
    if (!ballMoves)
    {
      ballTimer ++;
      if (ballTimer == 60)
      {
        ballMoves = true;
        ballTimer = 0;
      }
    }
  }
  if (arduboy.pressed(RIGHT_BUTTON) && (player.x < 20)) player.x += player.xSpeed;
  if (arduboy.pressed(LEFT_BUTTON) && (player.x > 4)) player.x -= player.xSpeed;
  if (arduboy.pressed(UP_BUTTON) && (player.y > 4)) player.y -= player.ySpeed;
  if (arduboy.pressed(DOWN_BUTTON) && (player.y + 24 < HEIGHT - 4)) player.y += player.ySpeed;
  if (arduboy.justPressed(B_BUTTON))ballMoves = true;


  if (arduboy.justPressed(A_BUTTON))
  {
    arduboy.clear();
    gameState = STATE_GAME_PAUSE;
  }

};

void stateGamePause()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, menuPause, i);
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PLAYING ;
};

void stateGameOver()
{
  if (AIScore == scoreMax)
  {
    for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, fishStick, i);
  }
  if (playerScore == scoreMax)
  {
    switch (winlose)
    {
      case 0:
        for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, fishWoman, i);
        break;
      case 1:
        for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, fishComplete, i);
        break;
      case 2:
        for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, fishMermaid, i);
        break;
    }
  }

  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
};


#endif
