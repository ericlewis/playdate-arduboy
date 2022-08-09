#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"

#define EEPROM_START                  EEPROM_STORAGE_SPACE_START + 400
#define EEPROM_START_C1               EEPROM_START
#define EEPROM_START_C2               EEPROM_START + 1
#define EEPROM_LEVEL                  EEPROM_START + 2

class EEPROM_Utils {

  public: 

    EEPROM_Utils(){};
        
    static void initEEPROM(bool forceClear);
    static uint8_t getLevel();
    static void setLevel(uint8_t levelNumber);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'L' and 'X' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */

const uint8_t letter1 = 'L';
const uint8_t letter2 = 'X';

void EEPROM_Utils::initEEPROM(bool forceClear) {

  byte c1 = EEPROM.read(EEPROM_START_C1);
  byte c2 = EEPROM.read(EEPROM_START_C2);

  if (forceClear || c1 != letter1 || c2 != letter2) { 

    EEPROM.update(EEPROM_START_C1, letter1);
    EEPROM.update(EEPROM_START_C2, letter2);
    EEPROM.update(EEPROM_LEVEL, 0);
  

  }

}


/* -----------------------------------------------------------------------------
 *   Get level number. 
 */
uint8_t EEPROM_Utils::getLevel() {

  return  EEPROM.read(EEPROM_LEVEL);

}


/* -----------------------------------------------------------------------------
 *   Save level number ..
 */
void EEPROM_Utils::setLevel(uint8_t levelNumber) {

  EEPROM.update(EEPROM_LEVEL, levelNumber);

}