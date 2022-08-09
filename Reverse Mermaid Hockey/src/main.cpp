/*
 REVERSE MARMAID HOCKEY: http://www.team-arg.org/rmh-manual.html

 Arduboy version 1.4 : http://www.team-arg.org/rmh-downloads.html
 
 MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

 2015 - DRAGULA96 - ART WORK: JO3RI & UNCLESPORKY

 Game License: MIT : https://opensource.org/licenses/MIT

 */


//determine the game
#define GAME_ID 31

#include "globals.h"
#include "menu.h"
#include "game.h"
#include "playfield.h"


typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameInitLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};



void setup()
{
  arduboy.begin();
  sound.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
}


void loop()
{
  sound.callback();
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) * (&mainGameLoop[gameState]))();
  arduboy.display();
}



