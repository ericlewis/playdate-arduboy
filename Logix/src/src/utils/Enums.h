#pragma once

#include "Arduboy2Ext.h"
#include "Data.h"

#define _DEBUG
#define NUMBER_OF_GATES 12
#define NUMBER_OF_CONNECTORS 20

#define ITEM_TYPES_RESERVED 250
#define TERMINAL_MINUS 250
#define TERMINAL_PLUS 251

#define NO_GATE_SELECTED 255
#define NO_CONNECTOR_SELECTED 255

#define CONNECTOR_INPUT_A  0
#define CONNECTOR_INPUT_B  1
#define CONNECTOR_OUTPUT  2

#define MOVEMENT_NO_RESTRICTIONS 0
#define MOVEMENT_PREVENT_LEFT 1
#define MOVEMENT_PREVENT_RIGHT 2
#define MOVEMENT_ALLOW_LEFT 4
#define MOVEMENT_ALLOW_RIGHT 8
#define MOVEMENT_ALLOW_MINUS 16
#define MOVEMENT_ALLOW_PLUS 32

#define SELECTED_MENU 12
#define FLASHING_LED_DELAY 32
#define COUNTER_DELAY 75
#define COUNTER_DELAY_SHORT 40

static const uint8_t LOGIC_GATES_SIZE = 10;

const char itemType0[] PROGMEM = "AND";
const char itemType1[] PROGMEM = "NAND";
const char itemType2[] PROGMEM = "OR";
const char itemType3[] PROGMEM = "NOR";
const char itemType4[] PROGMEM = "XOR";
const char itemType5[] PROGMEM = "XNOR";
const char itemType6[] PROGMEM = "NOT";

const char * const itemTypes[] PROGMEM = { itemType0, itemType1, itemType2, itemType3, itemType4, itemType5, itemType6 };

enum class GameState : uint8_t {

  IntroInit,
  Intro,
  LevelInit,
  LevelDisplay,
  GamePlay,
  GameMenu,
  SplashScreen_Init,
  SplashScreen,
  
};


enum class ItemType : uint8_t {

  AND,
  NAND,
  OR,
  NOR,
  XOR,
  XNOR,
  NOT,          // 6
  BLANK,
  SWITCH_UP,    // 8
  SWITCH_DOWN,  // 9
  LED,

};


struct OutcomeData {

  uint8_t switchState0;
  uint8_t switchState1;
  uint8_t switchState2;
  uint8_t ledState0;
  uint8_t ledState1;
  uint8_t ledState2;
  bool successful;
  
};


struct Connector {

  uint8_t id;
  uint8_t fromItem;
  uint8_t toItem;
  uint8_t toInput;
  bool active;

  uint8_t x1;
  uint8_t y1;

  uint8_t x2;
  uint8_t y2;

  uint8_t x3;
  uint8_t y3;

  uint8_t x4;
  uint8_t y4;

  void reset() {

    active = false;

    x1 = 0;
    y1 = 0;

    x2 = 0;
    y2 = 0;

    x3 = 0;
    y3 = 0;

    x4 = 0;
    y4 = 0;

  }

};


struct Gate {
  
  uint8_t id;
  ItemType type;
  uint8_t status;
  bool editable;

  bool input1;
  bool input2;

  bool output() {

    uint8_t start = (static_cast<uint8_t>(type) * 12);
  
    switch (type) {

      case ItemType::AND ... ItemType::XNOR:
        {
          uint8_t stateIdx = (input1 ? 2 : 0) + (input2 ? 1 : 0);
          return (pgm_read_byte(&data[start + (stateIdx * 3) + 2]) == 1);
        }
        return false;

      case ItemType::NOT:
        return !input1;

      case ItemType::SWITCH_UP:
        return input1;

      case ItemType::SWITCH_DOWN:
        return input2;

      case ItemType::LED:
        return (input1 | input2);

      default: return false;

    }

  }
  
};


struct Level {

  uint8_t id;
  Gate items[NUMBER_OF_GATES];
  uint8_t numberOfOutcomes;
  OutcomeData outcome[3];
  bool showMessage;             // Should the next level question be presented to the user?

  bool levelComplete() {

    bool returnVal = true;

    for (uint8_t x = 0; x < numberOfOutcomes; x++) {

      if (!outcome[x].successful) returnVal = false;

    }

    return returnVal;

  }

};


struct Selection {
  
  uint8_t item;
  uint8_t connector;
  uint8_t otherItem;
  uint8_t otherConnector;

  void reset() {

    connector = NO_CONNECTOR_SELECTED;
    otherItem = NO_GATE_SELECTED;
    otherConnector = NO_CONNECTOR_SELECTED;

  }

};


// --------------------------------------------------------------------------------------
//  Item type incrementors / decrementors ..
//
inline ItemType operator++( ItemType & c ) {

  switch (c) {

    case ItemType::AND ... ItemType::BLANK:

      if (c == ItemType::BLANK) {
        c = ItemType::AND;
      }
      else {
        c = static_cast<ItemType>( static_cast<uint8_t>(c) + 1 );
      }

      return c;

    case ItemType::SWITCH_UP ... ItemType::SWITCH_DOWN:

      if (c == ItemType::SWITCH_DOWN) {
        c = ItemType::SWITCH_UP;
      }

      return c;

    default:

      return c;

  }

}

inline ItemType operator++( ItemType & c, int ) {

  ItemType result = c;
  ++c;
  return result;

}

inline ItemType operator--( ItemType & c ) {

  switch (c) {

    case ItemType::AND ... ItemType::BLANK:

      if (c == ItemType::AND) {
        c = ItemType::BLANK;
      }
      else {
        c = static_cast<ItemType>( static_cast<uint8_t>(c) - 1 );
      }

      return c;

    case ItemType::SWITCH_UP ... ItemType::SWITCH_DOWN:

      if (c == ItemType::SWITCH_UP) {
        c = ItemType::SWITCH_DOWN;
      }

      return c;

    default:

      return c;

  }

}

inline ItemType operator--( ItemType & c, int ) {

  ItemType result = c;
  --c;
  return result;

}