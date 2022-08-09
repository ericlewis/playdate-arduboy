/*
  Blob Attack: http://www.team-arg.org/blba-manual.html

  Arduboy version 1.8.1:  http://www.team-arg.org/blba-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2015 - Game by JO3RI - All art by CastPixel: https://twitter.com/castpixel

  Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 28

#include "globals.h"
#include "menu.h"
#include "game.h"

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


void setup() {
  arduboy.boot();                                           // begin with the boot logo en setting up the device to work
  arduboy.audio.begin();
  sound.begin();

  arduboy.bootLogoSpritesSelfMasked();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  arduboy.initRandomSeed();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer)mainGameLoop[gameState])();
  arduboy.display();
}

