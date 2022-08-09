#pragma once

// Cloud
struct Cloud
{
    int16_t x = Arduboy2::width() + cloudWidth;
    uint8_t y = 10;
    uint8_t spd = 1;

    void update()
    {
        if(this->x < -cloudWidth)
            this->x = Arduboy2::width() + random(cloudWidth, 100);
        else
            this->x -= this->spd;
    }

    void draw()
    {
        Sprites::drawSelfMasked(this->x, this->y, cloudImg, 0);
    }
};