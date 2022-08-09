/**
 * @file Sprites.cpp
 * \brief
 * A class for drawing animated sprites from image and mask bitmaps.
 */

#include "Sprites.h"

void Sprites::drawExternalMask(int16_t x, int16_t y, const uint8_t *bitmap,
                               const uint8_t *mask, uint8_t frame, uint8_t mask_frame)
{
    draw(x, y, bitmap, frame, mask, mask_frame, SPRITE_MASKED);
}

void Sprites::drawOverwrite(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    draw(x, y, bitmap, frame, NULL, 0, SPRITE_OVERWRITE);
}

void Sprites::drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK_ERASE);
}

void Sprites::drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    draw(x, y, bitmap, frame, NULL, 0, SPRITE_IS_MASK);
}

void Sprites::drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    draw(x, y, bitmap, frame, NULL, 0, SPRITE_PLUS_MASK);
}

//common functions
void Sprites::draw(int16_t x, int16_t y,
                   const uint8_t *bitmap, uint8_t frame,
                   const uint8_t *mask, uint8_t sprite_frame,
                   uint8_t drawMode)
{
    unsigned int frame_offset;

    if (bitmap == NULL)
        return;

    uint8_t width = *bitmap;
    uint8_t height = *++bitmap;
    bitmap++;
    if (frame > 0 || sprite_frame > 0) {
        frame_offset = (width * ( height / 8 + ( height % 8 == 0 ? 0 : 1)));
        // sprite plus mask uses twice as much space for each frame
        if (drawMode == SPRITE_PLUS_MASK) {
            frame_offset *= 2;
        } else if (mask != NULL) {
            mask += sprite_frame * frame_offset;
        }
        bitmap += frame * frame_offset;
    }

    // if we're detecting the draw mode then base it on whether a mask
    // was passed as a separate object
    if (drawMode == SPRITE_AUTO_MODE) {
        drawMode = mask == NULL ? SPRITE_UNMASKED : SPRITE_MASKED;
    }

    drawBitmap(x, y, bitmap, mask, width, height, drawMode);
}

void Sprites::drawBitmap(int16_t x, int16_t y,
                         const uint8_t *bitmap, const uint8_t *mask,
                         uint8_t w, uint8_t h, uint8_t draw_mode)
{
    SpritesB::drawBitmap(x, y, bitmap, mask, w, h, draw_mode);
}