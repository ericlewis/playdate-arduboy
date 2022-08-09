#ifndef MENU_H
#define MENU_H

#include "globals.h"

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  if (globalCounter > 180)
  {
    globalCounter = 0;
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuMain()
{
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, splashScreen, i);
  sprites.drawSelfMasked(0, 48, mainMenu, menuSelection - 2);
  sprites.drawSelfMasked(64, 48, mainMenu, menuSelection - 2 + 6);
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
  sprites.drawSelfMasked(33, 16, title, 0);
  sprites.drawSelfMasked(37, 32, madeBy, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}



void stateMenuSoundfx()
{
  arduboy.clear();
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 0, splashScreen, i);
  sprites.drawSelfMasked(0, 48, mainMenu, 4 + arduboy.audio.enabled());
  sprites.drawSelfMasked(64, 48, mainMenu, 6 + 4 + arduboy.audio.enabled());
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();;
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}


#endif
