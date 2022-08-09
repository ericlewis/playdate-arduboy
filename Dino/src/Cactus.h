#pragma once

// Cactus
struct Cactus
{
    float x = Arduboy2::width();
    int16_t y = 43;
    float spd = 2;
    float accel = 0.02;

    // Cactus Collision Points
    int16_t cactusRight;

    void update()
    {
        this->cactusRight = this->x + cactusWidth;

        if(this->x < -20)
        {
            this->spd += this->accel;
            this->x += Arduboy2::width() + random(40, 80);
        }
        else
        {
            this->x -= this->spd;
        }
    }

    void draw()
    {
        Sprites::drawSelfMasked(this->x, this->y, cactusImg, 0);
    }
};