#ifndef MENU_H
#define MENU_H

#include "globals.h"

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  if (globalCounter > 90)
  {
    globalCounter = 0;
    gameState = STATE_MENU_MAIN;
  }
};

void stateMenuMain()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, menuTitle, i);
  sprites.drawSelfMasked(22, 56, menuMain, menuSelection - 2);
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 5))menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 2))menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = menuSelection;
}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuInfo()
{
  sprites.drawSelfMasked(14, 4, menuInfo, 0);
  sprites.drawSelfMasked(64, 4, menuInfo, 1);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, menuTitle, i);
  sprites.drawSelfMasked(27, 56, menuOffOn, arduboy.audio.enabled());
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}


void stateMenuPlay()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, BBQgrill, i);
  sprites.drawSelfMasked(61, 23, numbers, howManyWins);
  for (byte shrimpAmount = 0; shrimpAmount < howManyWins; shrimpAmount++) sprites.drawPlusMask(5 + (shrimpAmount * 17), 35, BBQshrimp_plus_mask, 0);
  if (arduboy.justPressed(RIGHT_BUTTON | UP_BUTTON) && (howManyWins < 7))howManyWins++;
  if (arduboy.justPressed(LEFT_BUTTON | DOWN_BUTTON) && (howManyWins > 1))howManyWins--;
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_INIT_LEVEL;
}


#endif
