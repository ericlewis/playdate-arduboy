#pragma once

#include "DinoState.h"

// Dino
struct Dino
{
    DinoState state = DinoState::RunningState;

    static constexpr uint8_t animationStep = 12;
    static constexpr uint8_t maxJumpHeight = 8;
    static constexpr uint8_t groundHeight = 62;
    static constexpr uint8_t x = 5;
    int16_t y = (Arduboy2::height() - 2 - dinoHeight);
    uint16_t dinoFrameCounter = 0;
    int8_t jumpVelocity = 0;
    bool autoJump = false;

    // Dino Collision Points
    int16_t dinoBottom;
    int16_t dinoRight;

    void update()
    {
        switch(this->state)
        {
            case DinoState::RunningState:
                updateRunningState();
                break;
            
            case DinoState::JumpingState:
                updateJumpingState();
                break;

            case DinoState::FallingState:
                updateFallingState();
                break;
        }

        this->dinoBottom = y + dinoHeight;
        this->dinoRight = (x + dinoWidth) - 4;
        
        // Collision Detection
        if(this->dinoBottom > cactus.y && this->dinoRight > cactus.x && this->dinoRight < cactus.cactusRight)
        {
            gameState = GameState::End;

            sound.tone(500, 100, 250, 200);
        }

        this->y += this->jumpVelocity;

        if(this->dinoFrameCounter >= this->animationStep)
            this->dinoFrameCounter = 0;
        else
            ++this->dinoFrameCounter;
    }

    void updateRunningState()
    {
        this->jumpVelocity = 0;
        this->y = this->groundHeight - dinoHeight;

        if(this->autoJump)
        {
            if(cactus.x - (this->x + cactusWidth) < (cactus.spd * cactusHeight) && (cactus.x - this->x) > 5)
                this->state = DinoState::JumpingState;
        }

        if(arduboy.justPressed(UP_BUTTON))
        {
            this->state = DinoState::JumpingState;

            sound.tone(500, 50);
        }

        if(this->dinoFrameCounter <= this->animationStep / 2)
            Sprites::drawSelfMasked(this->x, this->y, dinoImg, 2);
        else
            Sprites::drawSelfMasked(this->x, this->y, dinoImg, 1);
    }

    void updateJumpingState()
    {
        if(this->y <= this->maxJumpHeight)
            this->state = DinoState::FallingState;
        else
            this->jumpVelocity = -2;

        Sprites::drawSelfMasked(this->x, this->y, dinoImg, 0);
    }

    void updateFallingState()
    {
        if(this->y >= (this->groundHeight - dinoHeight))
            this->state = DinoState::RunningState;
        else
            this->jumpVelocity = 2;

        Sprites::drawSelfMasked(this->x, this->y, dinoImg, 0);
    }
};