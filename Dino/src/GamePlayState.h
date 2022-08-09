#pragma once

#include "Cactus.h"
Cactus cactus;
#include "Cloud.h"
Cloud cloud;
#include "Dino.h"
Dino dino;

// GamePlayState
struct GamePlayState
{
    static constexpr uint8_t scoreInterval = 32;

    void update()
    {
        dino.update();
        cactus.update();
        cloud.update();

        if(arduboy.everyXFrames(5))
            ++score;
    }

    void draw()
    {
        arduboy.drawLine(0, dino.groundHeight, Arduboy2::width(), dino.groundHeight);

        setCursorForScore(2, 5, score);

        arduboy.print(score);

        cactus.draw();
        cloud.draw();
    }
};