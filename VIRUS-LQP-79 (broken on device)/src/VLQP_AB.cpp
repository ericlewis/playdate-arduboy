/*
  VIRUS LQP-79: http://www.team-arg.org/zmbt-manual.html

  Arduboy version 1.6.0:  http://www.team-arg.org/zmbt-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2016 - FUOPY - JO3RI - STG - CASTPIXEL - JUSTIN CYR

  Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID                  40

#include "globals.h"
#include "menu.h"
#include "player.h"
#include "enemies.h"
#include "game.h"
#include "elements.h"
#include "bitmaps.h"
#include "level.h"


typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM mainGameLoop[] =
{
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGamePrepareLevel,
  stateGameNextLevel,
  stateGamePlaying,
  stateGameOver,
  stateGamePause,
  stateGameEnd,
  stateGameNew,
  stateGameContinue,
  stateGameMayhem,
};

void setup()
{
  arduboy.boot();                                           // begin with the boot logo en setting up the device to work
  arduboy.audio.begin();
  arduboy.setFrameRate(60);
  sound.begin();
  gameID = GAME_ID;
}

void loop() {
    sound.callback();
    if (!(arduboy.nextFrame())) return;
    arduboy.pollButtons();
    arduboy.clear();
    ((FunctionPointer)(mainGameLoop[gameState]))();
    arduboy.display();
}

