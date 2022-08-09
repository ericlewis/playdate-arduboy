#include "Arduboy2Ext.h"

Arduboy2Ext::Arduboy2Ext() {
  cursor_wrap_leftPos = 0;              
  cursor_wrap_rightPos = WIDTH;           
  vert_spacing = DEFAULT_VERT_SPACING;      
}

void Arduboy2Ext::setTextWrap(bool w, uint8_t l, uint8_t r) {
  textWrap = w;
  cursor_wrap_leftPos = l;
  cursor_wrap_rightPos = r;
}

void Arduboy2Ext::setTextWrap(bool w) {
  Arduboy2::setTextWrap(w);
}

void Arduboy2Ext::setTextWrapPositions(uint8_t l, uint8_t r)
{
  cursor_wrap_leftPos = l;
  cursor_wrap_rightPos = r;
}

void Arduboy2Ext::setTextVertSpacing(uint8_t y) {
  vert_spacing = y;
}

size_t Arduboy2Ext::write(uint8_t c) { 
  
  if (c == '\n')  {
    cursor_y += (textSize * vert_spacing);  
    cursor_x = cursor_wrap_leftPos;    
  }
  else if (c == '\r') {
    // skip em
  }
  else {
    
    drawChar(cursor_x, cursor_y, c, textColor, textBackground, textSize);
    cursor_x += textSize * 6;
    
    if ((c == '\n' || textWrap) && (cursor_x > (cursor_wrap_rightPos - textSize * 6))) {
      write('\n');
    }

  }

  return 1;

}

uint8_t Arduboy2Ext::justPressedButtons() const {

  return (~previousButtonState & currentButtonState);

}

uint8_t Arduboy2Ext::pressedButtons() const {

  return currentButtonState;

}

void Arduboy2Ext::clearButtonState() {

  currentButtonState = previousButtonState = 0;

}

uint16_t Arduboy2Ext::getFrameCount() const {

  return frameCount;

}

uint8_t Arduboy2Ext::getFrameCount(uint8_t mod) const {

  return frameCount % mod;

}

bool Arduboy2Ext::getFrameCountAlternate(uint8_t mod) const {

  return (frameCount % mod) < (mod / 2);

}


/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 */
void Arduboy2Ext::drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour) {

  if (x1 > x2) { uint8_t x3 = x1; x1 = x2; x2 = x3; }
  for (uint8_t x3 = x1; x3 <= x2; x3+=2) {
    drawPixel(x3, y, colour);
  }
  
}


/* ----------------------------------------------------------------------------
 *  Draw a vertical dotted line. 
 */
void Arduboy2Ext::drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t colour) {

  if (y1 > y2) { uint8_t y3 = y1; y1 = y2; y2 = y3; }
  for (uint8_t y3 = y1; y3 <= y2; y3+=2) {
    drawPixel(x, y3, colour);
  }
  
}