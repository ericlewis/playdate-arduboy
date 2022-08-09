#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "bitmaps.h"


//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_PLAYING           6
#define STATE_GAME_PAUSE             7
#define STATE_GAME_OVER              8

#define PLAYFIELD_WIDTH              8         // Playfield width in gems
#define PLAYFIELD_HEIGHT             8         // Playfield height in gems
#define PLAYFIELD_ZERO_X             64        // zero x-position of the playfield in pixel position on the screen
#define PLAYFIELD_ZERO_Y             0         // zero x-position of the playfield in pixel position on the screen
#define GEM_FREE                     0         // an empty place in the playfield

#define SWITCH_GEM_UP                1
#define SWITCH_GEM_DOWN              2
#define SWITCH_GEM_LEFT              3
#define SWITCH_GEM_RIGHT             4

#define NO_FLAG_ON_FIELD             0
#define FLAG_ON_FIELD                1

#define GEM_PIXELS                   8

#define SCORE_SMALL                  0
#define SCORE_BIG                    1

#define GAME_MODE_EASY               1
#define GAME_MODE_NORMAL             2
#define GAME_MODE_HARD               3

#define LEVEL_TO_START_WITH          1

Arduboy2Base arduboy;
Sprites sprites;
ArduboyTones sound(arduboy.audio.enabled);

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
boolean modeIsSelected;
boolean smokeLeftRight;

unsigned long scorePlayer;
unsigned long extraScoreForChain;
boolean giveExtraScore;

int field[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];
boolean fieldFlags[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];
boolean canMoveGemsDown;


int level = LEVEL_TO_START_WITH -1;
int selectorXpos;
int selectorYpos;
int timer;
byte animationFrame = 0;
byte smokeFrame = 0;
boolean selectorFrame = false;
byte blinkFrame = 0;
boolean blinkTubes = false;
byte flickerPlace = 0;
byte testingSwitch;
byte gameMode = GAME_MODE_EASY;

#endif
