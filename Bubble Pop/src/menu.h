#ifndef MENU_H
#define MENU_H

#include "globals.h"

byte sequenceFrame;
const unsigned char PROGMEM frameSequences[] = {
  // bouncing Ball Frame Sequence
  0, 1, 2, 0, 0, 0, 0, 0,
  // bouncing Ball Height Sequence
  0, 0, 0, 1, 0, 0, 0, 0,
};

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  if (globalCounter > 180) gameState = STATE_MENU_MAIN;
  gameState = STATE_MENU_MAIN;
}

void stateMenuMain()
{
  if (arduboy.everyXFrames(5)) sequenceFrame = (++sequenceFrame) % 8;
  sprites.drawSelfMasked(0, 0, titleScreen, 0);
  for (byte i = 0; i < 4; i++) sprites.drawErase(19 + (24 * i), 55, textMenu, i);
  sprites.drawErase(18 + ((menuSelection - 2) * 24), 53, selector, 0);
  sprites.drawPlusMask(26 + ((menuSelection - 2) * 24), 45 - pgm_read_byte(&frameSequences[sequenceFrame + 8]), bouncingBall_plus_mask, pgm_read_byte(&frameSequences[sequenceFrame]));
  
  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 5)) menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 2)) menuSelection--;
  if (arduboy.justPressed(B_BUTTON)) gameState = menuSelection;
}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  sprites.drawSelfMasked(0, 0, titleScreen, 0);
  sprites.drawErase(24, 55, textInfo, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  if (arduboy.everyXFrames(5)) sequenceFrame = (++sequenceFrame) % 8;
  sprites.drawSelfMasked(0, 0, titleScreen, 0);
  for (byte i = 0; i < 3; i++) sprites.drawErase(34 + (25 * i), 55, textSound, i);
  sprites.drawErase(55 + (arduboy.audio.enabled() * 24), 53, selector, 0);
  
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
  
}


#endif
