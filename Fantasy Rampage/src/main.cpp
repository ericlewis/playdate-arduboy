/*
  Fantasy Rampage: http://www.team-arg.org/frmp-manual.html

  Arduboy version 1.0:  http://www.team-arg.org/frmp-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2017 - Trodoss - JO3RI

  License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 48

#include "globals.h"
#include "menu.h"
#include "font.h"
#include "game.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameSelectGameMode,
  stateGameSelectPlayerMode,
  stateGamePlaying,
};


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);                                 // set the frame rate of the game at 30 fps
  arduboy.initRandomSeed();                                 // This sets the random to more random, remove this if no random is needed !
  // TODO: ATM.play(themeSong);
}


void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) * (&mainGameLoop[gameState]))();
  arduboy.display();
  if (arduboy.everyXFrames(3)) blinking = !blinking;
}

