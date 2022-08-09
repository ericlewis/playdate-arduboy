/*
  Begemmed: http://www.team-arg.org/bgem-manual.html

  Arduboy version 1.4:  http://www.team-arg.org/bgem-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2015 - JO3RI

  Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 30

#include "globals.h"
#include "menu.h"
#include "game.h"
#include "stage.h"

typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM  mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};

void setup()
{
  arduboy.begin();
  sound.begin();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
}

void loop() {
  sound.callback();
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) * (&mainGameLoop[gameState]))();
  arduboy.display();
}




