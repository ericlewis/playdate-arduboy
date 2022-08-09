/*
  Bubble PoP: http://www.team-arg.org/bpop-manual.html

  Arduboy version 0.1:  http://www.team-arg.org/bpop-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2018 - GAVENO - JO3RI

  License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 14

#include "globals.h"
#include "menu.h"
#include "game.h"
#include "inputs.h"
#include "player.h"
#include "elements.h"
#include "enemies.h"
#include "levels.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameArcade,
  stateGameInfinite,
  stateGameNextLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};


void setup() {
  arduboy.boot();
  arduboy.audio.begin();
  arduboy.bootLogoSpritesSelfMasked();
  //ATM.play(titleSong);
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  arduboy.initRandomSeed();                                 // This sets the random to more random, remove this if no random is needed !
}


void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer)mainGameLoop[gameState])();
  //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
  arduboy.display();
}

