#pragma once

#include "Arduboy2Ext.h"
#include "../Entities/Slot.h"
#include "HighScoreEditor.h"


class EEPROM_Utils {

  public: 

    EEPROM_Utils(){};
        
    static void initEEPROM(bool forceClear);
    static Slot getSlot(uint8_t x);
    static uint8_t saveScore(uint16_t score);
    static void writeChars(uint8_t slotIndex, HighScore &highScore);
    #ifdef HIGH_SCORES
    static uint16_t getHighScore();
    #endif
    static int16_t checkSum(bool update);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters '4' and '3' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */
void EEPROM_Utils::initEEPROM(bool forceClear) {

  byte c1 = EEPROM.read(Constants::EEPROM_Start_C1);
  byte c2 = EEPROM.read(Constants::EEPROM_Start_C2);;

  if (forceClear || c1 != '4' || c2 != '3') { 

    EEPROM.put(Constants::EEPROM_Start_C1, '4');
    EEPROM.put(Constants::EEPROM_Start_C2, '3');
    EEPROM.put(Constants::EEPROM_Level, 0);

    #ifndef HIGH_SCORES

      uint16_t score = 0;

      EEPROM.put(EEPROM_SCORE_1, score);
      EEPROM.put(EEPROM_SCORE_2, score);
      EEPROM.put(EEPROM_SCORE_3, score);

    #else

      uint16_t score = 10;

      for (uint8_t x = 0; x < MAX_NUMBER_OF_SCORES; x++) {

        EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x), x); 
        EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 1, x); 
        EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 2, x); 
        EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 3, score);
        score = score - 2;

      }

      EEPROM_Utils::checkSum(true);

    #endif

  }
  else {

    int16_t checkSumOld = 0;
    int16_t checkSumNow = EEPROM_Utils::checkSum(false);
    EEPROM.get(Constants::EEPROM_Checksum, checkSumOld);

    if (checkSumNow != checkSumOld) {

        EEPROM_Utils::initEEPROM(true);

    }
      
  }    

}


/* -----------------------------------------------------------------------------
 *   Get High Score details. 
 */
#ifdef HIGH_SCORES
uint16_t EEPROM_Utils::getHighScore() {

  uint16_t score;
  EEPROM.get(Constants::EEPROM_Top_Start + 3, score);
  return score;

}
#endif

/* -----------------------------------------------------------------------------
 *   Get slot details. 
 */
#ifdef HIGH_SCORES
Slot EEPROM_Utils::getSlot(uint8_t x) {

  Slot slot;

  slot.setChar0(EEPROM.read(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x)));
  slot.setChar1(EEPROM.read(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 1));
  slot.setChar2(EEPROM.read(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 2));

  uint16_t score = 0;
  EEPROM.get(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 3, score);
  slot.setScore(score);


  return slot;

}
#endif


/* -----------------------------------------------------------------------------
 *   Save score and return index.  255 not good enough! 
 */
#ifdef HIGH_SCORES
uint8_t EEPROM_Utils::saveScore(uint16_t score) {

  uint8_t idx = DO_NOT_EDIT_SLOT;

  for (uint8_t x = 0; x < MAX_NUMBER_OF_SCORES; x++) {

    Slot slot = getSlot(x);

    if (slot.getScore() < score) {

      idx = x;
      break;

    }

  }

  if (idx < DO_NOT_EDIT_SLOT) {

    for (uint8_t x = MAX_NUMBER_OF_SCORES - 1; x > idx; x--) {

      Slot slot = getSlot(x - 1);

      EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x), slot.getChar0());
      EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 1, slot.getChar1());
      EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 2, slot.getChar2());
      EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * x) + 3, slot.getScore());

    }

    EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * idx), 0);
    EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * idx) + 1, 0);
    EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * idx) + 2, 0);
    EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * idx) + 3, score);

    EEPROM_Utils::checkSum(true);

  }

  return idx;

}
#endif


/* -----------------------------------------------------------------------------
 *   Save characters in the nominated slot index. 
 */
#ifdef HIGH_SCORES
void EEPROM_Utils::writeChars(uint8_t slotIndex, HighScore &highScore) {

    EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * slotIndex), highScore.getChar(0));
    EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * slotIndex) + 1, highScore.getChar(1));
    EEPROM.put(Constants::EEPROM_Top_Start + (Constants::EEPROM_Entry_Size * slotIndex) + 2, highScore.getChar(2));

    EEPROM_Utils::checkSum(true);

}
#endif


/* -----------------------------------------------------------------------------
 *   Generate and optionally save a check sum .. 
 */
int16_t EEPROM_Utils::checkSum(bool update) {
    return 0;
}