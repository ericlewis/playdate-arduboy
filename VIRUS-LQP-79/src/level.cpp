#include "level.h"

// method implementations ///////////////////////////////////////////////////

int mapPositionX;
int mapPositionY;
int level;
int displayLevel;


// getTileType
// takes x and y in tile coordinates
// returns the tile type
unsigned char getTileType(unsigned int posX, unsigned int posY) {
  return
    // Block:read type (byte)
    *(
      // Block:first index is block number
      &blocks[
        // Levels:read index (byte) poso block[] array
        (*(
           // Levels:first index is level
           &maps[level - 1]
           // Levels:second index is map section (upper x/y bits)
           [((posX >> 3) + (posY & 0xF8)) >> 1]
         ) & ((posX & 8) ? 0x0F : 0xFF)) >> ((posX & 8) ? 0 : 4)
        // Block:second index is map tiles (lower x/y bits)
      ][
        (posX & 0x07) + ((posY & 0x07) << 3)
      ]
    );
}

void newDraw(unsigned posX, unsigned posY) {
  unsigned int intX = posX >> 3, subX = posX & 0x07;
  unsigned int intY = posY >> 3, subY = posY & 0x07;
  for (uint8_t x = 0; x < (subX ? 17 : 16); x++) {
    for (uint8_t y = 0; y < (subY ?  9 :  8); y++) {
      sprites.drawSelfMasked(
        ((int)x << 3) - subX, ((int)y << 3) - subY, tileset,
        getTileType(intX + x, intY + y)
        + (40 * ((level-1)%3))
      );
    }
  }
}


void drawLevel()
{
  newDraw (mapPositionX, mapPositionY);
}


void drawNumbers(uint8_t NumbersX, uint8_t NumbersY, uint8_t fontType, int timerOrScoreOrLevel)
{
  char buf[10];
  char charLen;
  char pad;
  //scorePlayer = arduboy.cpuLoad();
  switch (timerOrScoreOrLevel)
  {
    case DATA_TIMER:
      itoa(exitDoor.counter, buf, 10);
      charLen = strlen(buf);
      pad = 3 - charLen;
      break;
    case DATA_SCORE:
      ltoa(scorePlayer, buf, 10);
      charLen = strlen(buf);
      pad = 6 - charLen;
      break;
    case DATA_LEVEL:
      itoa(displayLevel, buf, 10);
      charLen = strlen(buf);
      pad = 3 - charLen;
      break;
  }


  //draw 0 padding
  for (uint8_t i = 0; i < pad; i++)
  {
    switch (fontType)
    {
      case FONT_TINY:
        sprites.drawSelfMasked(NumbersX + (4 * i), NumbersY, numbersTiny, 0);
        break;
      case FONT_SMALL:
        sprites.drawPlusMask(NumbersX + (7 * i), NumbersY, numbersSmall_plus_mask, 0);
        break;
      case FONT_BIG:
        sprites.drawSelfMasked(NumbersX + (10 * i), NumbersY, numbersBig, 0);
        break;
    }
  }

  for (uint8_t i = 0; i < charLen; i++)
  {
    char digit = buf[i];
      uint8_t j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (uint8_t z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    switch (fontType)
    {
      case FONT_TINY:
        sprites.drawSelfMasked(NumbersX + (pad * 4) + (4 * i), NumbersY, numbersTiny, digit);
        break;
      case FONT_SMALL:
        sprites.drawPlusMask(NumbersX + (pad * 7) + (7 * i), NumbersY, numbersSmall_plus_mask, digit);
        break;
      case FONT_BIG:
        sprites.drawSelfMasked(NumbersX + (pad * 10) + (10 * i), NumbersY, numbersBig, digit);
        break;
    }
  }
}

void mapCollide(int& x, int& y, bool horizontal, char& vel, char w, char h)
{
  short tilex;
  short tiley;
    uint8_t tileXMax = x % TILE_WIDTH != 0;
    uint8_t tileYMax = y % TILE_HEIGHT != 0;
  for (tilex = x / TILE_WIDTH; tilex < x / TILE_WIDTH + 2 + tileXMax; tilex++)
  {
    for (tiley = y / TILE_HEIGHT; tiley < y / TILE_HEIGHT + 2 + tileYMax; tiley++)
    {
      if (getTileType(tilex, tiley) > 10)
      {
        if (horizontal)
        {
          if (vel < 0)
            x = tilex * TILE_WIDTH + TILE_WIDTH;
          else if (vel > 0)
            x = tilex * TILE_WIDTH - w;
        }
        else
        {
          if (vel < 0)
            y = tiley * TILE_HEIGHT + TILE_HEIGHT;
          else if (vel > 0)
            y = tiley * TILE_HEIGHT - h;
        }
        vel = 0;
      }
    }
  }
}
