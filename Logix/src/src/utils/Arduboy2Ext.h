#pragma once

#include <Arduboy2.h>
#define DEFAULT_VERT_SPACING 12 

class Arduboy2Ext : public Arduboy2 {
  
  protected:

    uint8_t cursor_wrap_leftPos;    
    uint8_t cursor_wrap_rightPos;   
    uint8_t vert_spacing;      

  public:

    Arduboy2Ext();

    uint8_t justPressedButtons() const;
    uint8_t pressedButtons() const;
    uint16_t getFrameCount() const;
    uint8_t getFrameCount(uint8_t mod) const;
    bool getFrameCountAlternate(uint8_t mod) const;

    void clearButtonState();
    
    void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour);
    void drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t colour);

        
    /** \brief                          
     * Set or disable text wrap mode and wrapping positions.
     *
     * \param w `true` enables text wrap mode. `false` disables it.
     * \param l left hand X position to wrap text to.  Default is 0.
     * \param r right hand X position to wrap text at.  Default is WIDTH
     *
     * \details
     * Text wrap mode is enabled by specifying `true`. In wrap mode, the text
     * cursor will be moved to the start of the next line (based on the current
     * text size) if the following character wouldn't fit entirely at the end of
     * the current line (where the rightmost column value is defined by r).  
     * Wrapping text will reset the X coordinate to l.
     * If wrap mode is disabled, characters will continue to be written to the
     * same line. A character at the right edge of the screen may only be
     * partially displayed and additional characters will be off screen.
     */
    void setTextWrap(bool w, uint8_t l, uint8_t r);            

    void setTextWrap(bool w);            
    
    /** \brief                         //SJH
     * Set wrapping positions, left and right.
     *
     * \param l left hand X position to wrap text to.  Default is 0.
     * \param r right hand X position to wrap text at.  Default is WIDTH
     *
     * \details
     * Set the left most and right most X positions for wrapping text.
     * Only effective if setTextWrap / textWrap is true.
     */ 
    void setTextWrapPositions(uint8_t l, uint8_t r);      

    /** \brief                          
     * Set vertical line spacing.
     *
     * \param y amount to increase y coordinate on wrap, default is DEFAULT_VERT_SPACING
     *
     * \details
     * Sets the value by which to increase the text cursor Y value when
     * wrapping text or rendering a line feed.  Only effective if setTextWrap /
     * textWrap is true.
     */
    void setTextVertSpacing(uint8_t y);             
    
    virtual size_t write(uint8_t);


};
