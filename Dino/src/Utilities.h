#pragma once

// TextToMiddle
inline int textToMiddle(int charWidth)
{
    return (Arduboy2::width() - (arduboy.getCharacterWidth(charWidth) + arduboy.getCharacterSpacing(charWidth - 1))) / 2;
}

// SetCursorForScore
void setCursorForScore(uint8_t x, uint8_t y, uint16_t score)
{
    arduboy.setCursorY(y);

    if(score < 100)
        arduboy.setCursorX(textToMiddle(x + 0));
    else if(score < 1000)
        arduboy.setCursorX(textToMiddle(x + 1));
    else if(score < 10000)
        arduboy.setCursorX(textToMiddle(x + 2));
    else
        arduboy.setCursorX(textToMiddle(x + 3));
}

// CountDigits in Number
int countDigits(uint16_t number)
{
    int count = 0;

    while(number != 0)
    {
        number = number / 10;
        ++count;
    }
    return count;
}