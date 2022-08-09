#ifndef MENU_H
#define MENU_H

#include "globals.h"

byte titleFrame = 0;
byte titleCounter = 27;
boolean slideMenuLeft;
boolean slidingMenu;
byte slideMenuCounter = 0;
byte menuX;
byte soundMenuX;

void titleScreen()
{
  if (arduboy.everyXFrames(6)) titleCounter++;
  if (titleCounter > 47)titleCounter = 0;
  titleFrame = titleCounter;
  if (titleFrame > 7) titleFrame = 0;
  for (byte i = 0; i < 2; i++)
  {
    sprites.drawSelfMasked(i * 64, 0, titleTop, i);
    sprites.drawSelfMasked(i * 64, 12, titleGleam, titleFrame + (i * 8));
  }
}

void updateSlidingMenu(byte amount)
{
  if (slideMenuLeft && slidingMenu && (slideMenuCounter < amount + 1))
  {
    slideMenuCounter++;
    if (amount == 25)menuX++;
    if (amount == 21)soundMenuX++;
  }

  if (!slideMenuLeft && slidingMenu && (slideMenuCounter < amount + 1))
  {
    slideMenuCounter++;
    if (amount == 25)menuX--;
    if (amount == 21)soundMenuX--;
  }

  if (slideMenuCounter > amount - 1)
  {
    slideMenuCounter = 0;
    slidingMenu = false;
  }
}

void stateMenuIntro()
{
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  globalCounter++;
  if (globalCounter > 180)
  {
    gameState = STATE_MENU_MAIN;
    slideMenuLeft = false;
    slidingMenu = false;
    menuX = 25 * (menuSelection - 2);
    soundMenuX = 21 * !arduboy.audio.enabled();
  }
}

void stateMenuMain()
{
  titleScreen();
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(i * 64, 36, titleBottom, i);
  updateSlidingMenu(25);
  if (!slidingMenu) sprites.drawSelfMasked(52, 47, titleLine, 0);
  for (byte i = 0; i < 4; i++)
  {
    {
      if (((2 + i) - menuSelection) != 0 || slidingMenu)
      {
        sprites.drawPlusMask(52 - menuX + (25 * i), 48, titleText_plus_mask, i);
      }
      if ((((2 + i) - menuSelection) == 0) && !slidingMenu)
      {
        sprites.drawPlusMask(52 - menuX + (25 * i), 48, titleText_plus_mask, i + 7);
      }
    }
  }

  sprites.drawSelfMasked(51, 56, titleSelector, 0);
  if (arduboy.justPressed(LEFT_BUTTON) && !slidingMenu && (menuSelection > 2))
  {
    slidingMenu = true;
    slideMenuLeft = false;
    menuSelection--;
  }
  else if (arduboy.justPressed(RIGHT_BUTTON) && !slidingMenu && (menuSelection < 5))
  {
    slidingMenu = true;
    slideMenuLeft = true;
    menuSelection++;
  }

  else if (arduboy.justPressed(B_BUTTON)) gameState = menuSelection;
}

void stateMenuHelp()
{
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  titleScreen();
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(i * 64, 36, infoScreen, i);
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  titleScreen();
  for (byte i = 0; i < 2; i++) sprites.drawSelfMasked(i * 64, 36, titleBottom, i);
  updateSlidingMenu(21);
  if (!slidingMenu) sprites.drawSelfMasked(54, 47, titleLine, 1);
  sprites.drawPlusMask(soundMenuX + 21 - 9, 48, titleText_plus_mask, 6);
  sprites.drawPlusMask(soundMenuX + 21 + 12, 48, titleText_plus_mask, 4 + ((1 - slidingMenu) * (7 * (1 - arduboy.audio.enabled()))));
  sprites.drawPlusMask(soundMenuX + 21 + 33, 48, titleText_plus_mask, 5 + ((1 - slidingMenu) * (7 * arduboy.audio.enabled())));


  sprites.drawSelfMasked(51, 56, titleSelector, 1);
  if (arduboy.justPressed(LEFT_BUTTON) && !slidingMenu && arduboy.audio.enabled())
  {
    slidingMenu = true;
    slideMenuLeft = true;
    arduboy.audio.off();
  }
  else if (arduboy.justPressed(RIGHT_BUTTON) && !slidingMenu && !arduboy.audio.enabled())
  {
    slidingMenu = true;
    slideMenuLeft = false;
    arduboy.audio.on();
  }
  else if (arduboy.justPressed(B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}

#endif
