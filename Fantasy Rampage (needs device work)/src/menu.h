#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "font.h"
#include "song_data.h"

#define SMALL_MONSTERS_ON_ONE_LINE                      8

struct Monsters
{
  public:
    int x, y;
    byte frame;
};

Monsters smallMonster[(SMALL_MONSTERS_ON_ONE_LINE * 2)];

byte smallMonsterLineOne;
byte smallMonsterLineTwo;


void setSmallMonster()
{
  smallMonsterLineOne = 0;
  smallMonsterLineTwo = 0;
  for (byte i = 0; i < (SMALL_MONSTERS_ON_ONE_LINE * 2); i++)
  {
    if (i < SMALL_MONSTERS_ON_ONE_LINE)
    {
      smallMonster[i].x = i * 20;
      smallMonster[i].y = 2;
      smallMonster[i].frame = smallMonsterLineOne;
      smallMonsterLineOne++;
    }
    else
    {
      smallMonster[i].x = (i - SMALL_MONSTERS_ON_ONE_LINE) * 20;
      smallMonster[i].y = 46;
      smallMonster[i].frame = smallMonsterLineTwo;
      smallMonsterLineTwo++;
    }
  }
}


void drawTitleScreen()
{
  if (arduboy.everyXFrames(1))
  {
    for (byte i = 0; i < (SMALL_MONSTERS_ON_ONE_LINE * 2); i++)
    {
      if (i < SMALL_MONSTERS_ON_ONE_LINE)
      {
        smallMonster[i].x--;
        if (smallMonster[i].x < -16)
        {
          smallMonster[i].x = 144;
          smallMonster[i].frame = smallMonsterLineOne;
          smallMonsterLineOne = (++smallMonsterLineOne) % 24;

        }
      }
      else
      {
        smallMonster[i].x++;
        if (smallMonster[i].x > 144)
        {
          smallMonster[i].x = -16;
          smallMonster[i].frame = smallMonsterLineTwo;
          smallMonsterLineTwo = (++smallMonsterLineTwo) % 24;
        }
      }
    }

  }
  sprites.drawSelfMasked(0, 21, titleName, 0);

  for (byte i = 0; i < (SMALL_MONSTERS_ON_ONE_LINE * 2); i++) sprites.drawSelfMasked(smallMonster[i].x, smallMonster[i].y, monstersSmall, smallMonster[i].frame);
}

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, T_arg, 0);
  if (globalCounter > 120)
  {
    gameState = STATE_MENU_MAIN;
    // TODO: ATM.play(themeSong);
    setSmallMonster();
  }
}

void stateMenuMain()
{

  drawTitleScreen();
  print_progmem(14, 38, text_menu);
  sprites.drawSelfMasked(12 + ((menuSelection - 2) * 25), 36, selectorTop, 0);
  sprites.drawPlusMask(11 + ((menuSelection - 2) * 25), 44, selectorMid_plus_mask, 0);
  
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
  drawTitleScreen();
  print_progmem(14, 38, text_info);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  print_progmem(29, 38, text_sound);
  sprites.drawSelfMasked(59 + (arduboy.audio.enabled() * 23), 36, selectorTop, 0);
  sprites.drawPlusMask(58 + (arduboy.audio.enabled() * 23), 44, selectorMid_plus_mask, 0);
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
  }
}


#endif
