//
//  Copyright (C) 2021 Ashlee J (@Ashteroide)
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

// Chrome Dino v3
// Created: 25 November, 2020
// Ashlee J (Ashteroide)

#include <Arduboy2.h>
#include <ArduboyTones.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#include "Images.h"

uint16_t score = 0;

#include "MenuCursor.h"
MenuCursor menuCursor = MenuCursor::Start;
#include "GameState.h"
GameState gameState = GameState::Menu;

#include "Utilities.h"
#include "Data.h"

#include "GamePlayState.h"
GamePlayState gamePlay;
#include "MenuState.h"
MenuState menuState;
#include "EndState.h"
EndState endState;

#include "Alphabet.h"
#include "HighScoreState.h"
HighScoreState highScoreState;
#include "NameEntryState.h"
NameEntryState nameEntryState;

void setup()
{
    arduboy.begin();
    sound.begin();
    // clearSaveData();
}

void loop()
{
    sound.callback();
    if(!arduboy.nextFrame())
        return;

    arduboy.pollButtons();

    arduboy.clear();

    switch(gameState)
    {
        case GameState::Menu:
            menuState.update();
            menuState.draw();
            break;

        case GameState::Game:
            gamePlay.update();
            gamePlay.draw();
            break;

        case GameState::End:
            endState.update();
            endState.draw();
            break;

        case GameState::HighScore:
            highScoreState.update();
            highScoreState.draw();
            break;

        case GameState::NameEntry:
            nameEntryState.update();
            nameEntryState.draw();
            break;
    }

    arduboy.display();
}
