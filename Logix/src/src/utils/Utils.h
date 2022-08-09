#pragma once

#include "Enums.h"
#include "Puzzles.h"


// --------------------------------------------------------------------------------------
//  Return the upper 4 bits of a byte.
//
uint8_t leftValue(uint8_t val) {

  return val >> 4; 
      
}


// --------------------------------------------------------------------------------------
//  Return the lower 4 bits of a byte.
//
uint8_t rightValue(uint8_t val) {

  return val & 0x0F; 
      
}


// --------------------------------------------------------------------------------------
//  Determine the column from an item ID (index) ..
//
uint8_t column(uint8_t itemId) {
  return itemId / 3;
}


// --------------------------------------------------------------------------------------
//  Determine the row from an item ID (index) ..
//
uint8_t row(uint8_t itemId) {
  return itemId % 3;
}


// --------------------------------------------------------------------------------------
//  Calculate an item ID (index) from a column and row ..
//
uint8_t indexFromColRow(uint8_t col, uint8_t row) {
  return (col * 3) + row;
}


// --------------------------------------------------------------------------------------
//  Determine the yOffset based on the item's column ..
//
uint8_t yOffset(uint8_t itemId) {

  uint8_t col = column(itemId);

  switch (col) {

    case 0:   return 0;
    case 1:   return 2;
    case 2:   return 0;
    case 3:   return 6;

  }

  return 255;

}


// --------------------------------------------------------------------------------------
//  Move the second connector in a selection down ..
//
void moveSelectedOtherDown(Selection *selection, Gate gates[], uint8_t allowedMovements) {

  bool moved = false;

  int8_t rowOther = 0;
  int8_t colOther = 0;
  int8_t colSelected = column(selection->item);

  if (selection->otherItem != TERMINAL_MINUS && selection->otherItem != NO_GATE_SELECTED) {

    if (gates[selection->otherItem].type != ItemType::NOT && selection->otherConnector == CONNECTOR_INPUT_A && column(selection->otherItem) < 3) { 

      selection->otherConnector = CONNECTOR_INPUT_B;
      moved = true;

    }
    else {

      switch (selection->otherItem) {

        case TERMINAL_PLUS:
          rowOther = -1;
          colOther = column(selection->item) - 1;
          break;

        case 0 ... 11:
          rowOther = row(selection->otherItem);    
          colOther = column(selection->otherItem);
          break;
        
      }

      if (rowOther < 2) {

        for (uint8_t x = rowOther + 1; x < 3; x++) {

          if (gates[indexFromColRow(colOther, x)].type != ItemType::BLANK) {

            selection->otherItem = indexFromColRow(colOther, x);

            if (colSelected < colOther) {
              selection->otherConnector = CONNECTOR_INPUT_A;
            }
            else {
              selection->otherConnector = CONNECTOR_OUTPUT;
            }

            moved = true;
            break;

          }

        }

      }

    }

  
    if (!moved && (allowedMovements & MOVEMENT_ALLOW_MINUS) == MOVEMENT_ALLOW_MINUS) {

      selection->otherItem = TERMINAL_MINUS;
      selection->otherConnector = CONNECTOR_INPUT_A;

    }

  }

}


// --------------------------------------------------------------------------------------
//  Move the second connector in a selection up ..
//
void moveSelectedOtherUp(Selection *selection, Gate gates[], uint8_t allowedMovements) {

  bool moved = false;
  int8_t rowOther = 0;
  int8_t colOther = 0;
  int8_t colSelected = column(selection->item);

  if (selection->otherItem != TERMINAL_PLUS && selection->otherItem != NO_GATE_SELECTED) {

    if (gates[selection->otherItem].type != ItemType::NOT && selection->otherConnector == CONNECTOR_INPUT_B) { 

      selection->otherConnector = CONNECTOR_INPUT_A;
      moved = true;

    }
    else {

      switch (selection->otherItem) {

        case TERMINAL_MINUS:
          rowOther = 3;
          colOther = column(selection->item) - 1;
          break;

        case 0 ... 11:
          rowOther = row(selection->otherItem);    
          colOther = column(selection->otherItem);
          break;
        
      }

      if (rowOther > 0) {

        for (int8_t x = rowOther - 1; x >= 0; x--) {

          if (gates[indexFromColRow(colOther, x)].type != ItemType::BLANK) {

            selection->otherItem = indexFromColRow(colOther, x);

            if (colSelected < colOther) {
              selection->otherConnector = ( colOther < 3 ? CONNECTOR_INPUT_B : CONNECTOR_INPUT_A);
            }
            else {
              selection->otherConnector = CONNECTOR_OUTPUT;
            }

            moved = true;
            break;

          }

        }

      }

    }

    if (!moved && ((allowedMovements & MOVEMENT_ALLOW_PLUS) == MOVEMENT_ALLOW_PLUS)) {

      selection->otherItem = TERMINAL_PLUS;
      selection->otherConnector = CONNECTOR_INPUT_A;

    }

  }

}


// --------------------------------------------------------------------------------------
//  Move the second connector in a selection right ..
//
void moveSelectedOtherRight(Selection *selection, Gate gates[], uint8_t allowedMovements) {

  uint8_t currentItem = (selection->otherItem == NO_GATE_SELECTED ? selection->item : selection->otherItem);
  selection->otherConnector = CONNECTOR_INPUT_A;

  switch (currentItem) {

    case NO_GATE_SELECTED:
    case 0 ... 2:
      if      (gates[3].type != ItemType::BLANK) { selection->otherItem = 3; }
      else if (gates[4].type != ItemType::BLANK) { selection->otherItem = 4; }
      else if (gates[5].type != ItemType::BLANK) { selection->otherItem = 5; }

      else if (gates[6].type != ItemType::BLANK) { selection->otherItem = 6; }
      else if (gates[7].type != ItemType::BLANK) { selection->otherItem = 7; }
      else if (gates[8].type != ItemType::BLANK) { selection->otherItem = 8; }

      else if (gates[9].type == ItemType::LED)  { selection->otherItem = 9; }
      else if (gates[10].type == ItemType::LED) { selection->otherItem = 10; }
      else if (gates[11].type == ItemType::LED) { selection->otherItem = 11; }
      break;
    
    case 3 ... 5:
      if      (gates[6].type != ItemType::BLANK) { selection->otherItem = 6; }
      else if (gates[7].type != ItemType::BLANK) { selection->otherItem = 7; }
      else if (gates[8].type != ItemType::BLANK) { selection->otherItem = 8; }
      else if (gates[9].type == ItemType::LED)  { selection->otherItem = 9; }
      else if (gates[10].type == ItemType::LED) { selection->otherItem = 10; }
      else if (gates[11].type == ItemType::LED) { selection->otherItem = 11; }
      break;

    case 6 ... 8:
      if      (gates[9].type == ItemType::LED)  { selection->otherItem = 9; }
      else if (gates[10].type == ItemType::LED) { selection->otherItem = 10; }
      else if (gates[11].type == ItemType::LED) { selection->otherItem = 11; }
      break;

  }

  if (column(selection->item) == column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }
  if ((allowedMovements & MOVEMENT_PREVENT_LEFT) == MOVEMENT_PREVENT_LEFT && column(selection->item) > column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }
  if ((allowedMovements & MOVEMENT_PREVENT_RIGHT) == MOVEMENT_PREVENT_RIGHT && column(selection->item) < column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }

}



// --------------------------------------------------------------------------------------
//  Move the second connector in a selection left ..
//
void moveSelectedOtherLeft(Selection *selection, Gate gates[], uint8_t allowedMovements) {

  uint8_t currentItem = (selection->otherItem == NO_GATE_SELECTED ? selection->item : selection->otherItem);
  selection->otherConnector = CONNECTOR_INPUT_A;

  switch (currentItem) {

    case 3 ... 5:
      if      (gates[0].type != ItemType::BLANK) { selection->otherItem = 0; }
      else if (gates[1].type != ItemType::BLANK) { selection->otherItem = 1; }
      else if (gates[2].type != ItemType::BLANK) { selection->otherItem = 2; }
      break;

    case 6 ... 8:
      if      (gates[3].type != ItemType::BLANK) { selection->otherItem = 3; }
      else if (gates[4].type != ItemType::BLANK) { selection->otherItem = 4; }
      else if (gates[5].type != ItemType::BLANK) { selection->otherItem = 5; }

      else if (gates[0].type != ItemType::BLANK) { selection->otherItem = 0; }
      else if (gates[1].type != ItemType::BLANK) { selection->otherItem = 1; }
      else if (gates[2].type != ItemType::BLANK) { selection->otherItem = 2; }
      break;
    
    case 9 ... 11:
      if      (gates[6].type != ItemType::BLANK) { selection->otherItem = 6; }
      else if (gates[7].type != ItemType::BLANK) { selection->otherItem = 7; }
      else if (gates[8].type != ItemType::BLANK) { selection->otherItem = 8; }

      else if (gates[3].type != ItemType::BLANK) { selection->otherItem = 3; }
      else if (gates[4].type != ItemType::BLANK) { selection->otherItem = 4; }
      else if (gates[5].type != ItemType::BLANK) { selection->otherItem = 5; }

      else if (gates[0].type != ItemType::BLANK) { selection->otherItem = 0; }
      else if (gates[1].type != ItemType::BLANK) { selection->otherItem = 1; }
      else if (gates[2].type != ItemType::BLANK) { selection->otherItem = 2; }

      break;

  }

  if (column(selection->item) == column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }
  if ((allowedMovements & MOVEMENT_PREVENT_LEFT) == MOVEMENT_PREVENT_LEFT && column(selection->item) > column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }
  if ((allowedMovements & MOVEMENT_PREVENT_RIGHT) == MOVEMENT_PREVENT_RIGHT && column(selection->item) < column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }

}


// --------------------------------------------------------------------------------------
//  Create a new connection (or delete it if it exists) ..
//
void createOrDeleteConnection(Selection *selection, Connector *currentConnector, Connector connectors[]) {

  bool removed = false;


  // False alarm ?

  if (selection->otherItem == NO_GATE_SELECTED || selection->item == NO_GATE_SELECTED || selection->otherItem == selection->item) {currentConnector->reset(); return; }



  // Before adding a connection, test to see if one already exists ..

  for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

    Connector *connector = &connectors[x];

    switch (selection->otherItem) {

      case TERMINAL_MINUS:
      case TERMINAL_PLUS:

        if (connector->fromItem == selection->otherItem && connector->toItem == selection->item && 
            connector->toInput == selection->connector && connector->active == true) {

          connector->active = false;
          removed = true;

        }

        break;

      default:

        if (selection->item < selection->otherItem) {

          if (connector->fromItem == selection->item && connector->toItem == selection->otherItem && 
              connector->toInput == selection->otherConnector && connector->active == true) {

            connector->active = false;
            removed = true;

          }

        }
        else {

          if (connector->fromItem == selection->otherItem && connector->toItem == selection->item &&
              connector->toInput == selection->connector && connector->active == true) {

            connector->active = false;
            removed = true;

          }

        }
        break;

    }

    if (removed) break;

  }


  // Add the connectotr only if it wasn't just removed ..

  if (!removed) {  

    bool added = false;

    for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

      Connector *connector = &connectors[x];

      if (!connector->active) {

        switch (selection->otherItem) {

          case TERMINAL_MINUS:
          case TERMINAL_PLUS:

            connector->fromItem = selection->otherItem;
            connector->toItem = selection->item;
            connector->toInput = selection->connector;
            connector->active = true;
            added = true;
            break;

          default:

            if (selection->item < selection->otherItem) {

              connector->fromItem = selection->item;
              connector->toItem = selection->otherItem;
              connector->toInput = selection->otherConnector;
              connector->active = true;

            }
            else {

              connector->fromItem = selection->otherItem;
              connector->toItem = selection->item;
              connector->toInput = selection->connector;
              connector->active = true;

            }

            added = true;
            break;

        }

        connector->x1 = currentConnector->x1;
        connector->y1 = currentConnector->y1;
        connector->x2 = currentConnector->x2;
        connector->y2 = currentConnector->y2;
        connector->x3 = currentConnector->x3;
        connector->y3 = currentConnector->y3;
        connector->x4 = currentConnector->x4;
        connector->y4 = currentConnector->y4;

      }

      if (added) break;

    }

  }

  currentConnector->reset();

}


// --------------------------------------------------------------------------------------
//  Update the results ..
//
void updateResults(Gate gates[], Connector connectors[]) {

  
  // Switches ..

  for (uint8_t x = 0; x < 12; x++) { 

    Gate *gate = &gates[x];

    bool input1 = false;
    bool input2 = false;

    if (gate->type != ItemType::BLANK) {

      for (uint8_t y = 0; y < NUMBER_OF_CONNECTORS; y++) {

        Connector *connector = &connectors[y];

#ifdef DEBUG
        if (x == 0){
          Serial.print("Conn: ");
          Serial.print(y);
          Serial.print(" fi:");
          Serial.print(connector->fromItem);
          Serial.print(" type: ");
          Serial.print((uint8_t)gates[connector->fromItem].type);
          Serial.print(" tit: ");
          Serial.print(connector->toItem);
          Serial.print(" tin: ");
          Serial.print(connector->toInput);
          Serial.print(" type: ");
          Serial.print((uint8_t)gates[x].type);
          Serial.print(" output: ");
          Serial.print(gates[x].output());
        }
#endif

        if (connector->active && (connector->toItem == x) && (connector->toInput == CONNECTOR_INPUT_A) & (connector->fromItem == TERMINAL_PLUS)) { input1 = true; }
        if (connector->active && (connector->toItem == x) && (connector->toInput == CONNECTOR_INPUT_B) & (connector->fromItem == TERMINAL_PLUS)) { input2 = true; }

        if (connector->fromItem != TERMINAL_PLUS && connector->fromItem != TERMINAL_MINUS) {

          Gate *fromGate = &gates[connector->fromItem];

          if (connector->active && (connector->toItem == x) && (connector->toInput == CONNECTOR_INPUT_A) & (fromGate->output() == true)) { input1 = true; }
          if (connector->active && (connector->toItem == x) && (connector->toInput == CONNECTOR_INPUT_B) & (fromGate->output() == true)) { input2 = true; }

        }

#ifdef DEBUG
        if (x == 0){
          Serial.println(" .");
        }
#endif

      }

    }

    gate->input1 = input1;
    gate->input2 = input2;

#ifdef DEBUG
    Serial.print("Gate ");
    Serial.print(x);
    Serial.print(" (");
    Serial.print(gate->input1);
    Serial.print(", ");
    Serial.print(gate->input2);
    Serial.print(")=");
    Serial.println(gate->output());
#endif

  }

}


// --------------------------------------------------------------------------------------
//  Test the results against the expected outcomes ..
//
void test(Level *level, Connector connectors[] ) {

  for (uint8_t outcomeIdx = 0; outcomeIdx < level->numberOfOutcomes; outcomeIdx++) {

    level->outcome[outcomeIdx].successful = true;

    if (level->outcome[outcomeIdx].switchState0 <= 1) {

      level->items[0].type = (level->outcome[outcomeIdx].switchState0 == 0 ? ItemType::SWITCH_DOWN : ItemType::SWITCH_UP);

    }

    if (level->outcome[outcomeIdx].switchState1 <= 1) {

      level->items[1].type = (level->outcome[outcomeIdx].switchState1 == 0 ? ItemType::SWITCH_DOWN : ItemType::SWITCH_UP);

    }

    if (level->outcome[outcomeIdx].switchState2 <= 1) {

      level->items[2].type = (level->outcome[outcomeIdx].switchState2 == 0 ? ItemType::SWITCH_DOWN : ItemType::SWITCH_UP);

    }

    updateResults(level->items, connectors);

    if (level->outcome[outcomeIdx].ledState0 <= 1 && level->items[9].output() != level->outcome[outcomeIdx].ledState0) {
      level->outcome[outcomeIdx].successful = false;
    }

    if (level->outcome[outcomeIdx].ledState1 <= 1 && level->items[10].output() != level->outcome[outcomeIdx].ledState1) {
      level->outcome[outcomeIdx].successful = false;
    }

    if (level->outcome[outcomeIdx].ledState2 <= 1 && level->items[11].output() != level->outcome[outcomeIdx].ledState2) {
      level->outcome[outcomeIdx].successful = false;
    }

  } 

}


// --------------------------------------------------------------------------------------
//  Load a level ..
//
void LoadLevel(Level *level, uint8_t levelId) {

  const uint8_t *levelToLoad = puzzles[levelId];
  uint16_t start = 0;

  level->id = (levelId + 1);

  for (uint8_t x = 0; x < NUMBER_OF_GATES; x++) {

    uint8_t item = pgm_read_byte(&levelToLoad[start++]);
    level->items[x].id = x;
    level->items[x].type = static_cast<ItemType>(item);
    level->items[x].editable = (x < 3 || level->items[x].type == ItemType::BLANK);

  }

  level->numberOfOutcomes = pgm_read_byte(&levelToLoad[start++]);

  for (uint8_t x = 0; x < 3; x++) {
    
    uint8_t val = pgm_read_byte(&levelToLoad[start++]);
    level->outcome[x].switchState0 = leftValue(val);
    level->outcome[x].ledState0 = rightValue(val);

    val = pgm_read_byte(&levelToLoad[start++]);
    level->outcome[x].switchState1 = leftValue(val);
    level->outcome[x].ledState1 = rightValue(val);

    val = pgm_read_byte(&levelToLoad[start++]);
    level->outcome[x].switchState2 = leftValue(val);
    level->outcome[x].ledState2 = rightValue(val);

  }

}


// --------------------------------------------------------------------------------------
//  Clear all connections to the nominated item ..
//
void clearConnectors(Connector connectors[], uint8_t itemId) {

  for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

    Connector *connector = &connectors[x];

    if (connector->fromItem == itemId || connector->toItem) {

      connector->active = false;

    }

  }

}


// --------------------------------------------------------------------------------------
//  Clear the selection status ..
//
void clearStatus(Selection *selection, Connector *currentConnector) {

  selection->reset();
  currentConnector->reset();

}


// --------------------------------------------------------------------------------------
//  Are all gates in the game fully wired ..
//
bool islevelFullyWired(Level *level, Connector connectors[] ) {

  bool oneGateUsed = false;

  for (uint8_t y = 0; y < NUMBER_OF_GATES; y++) {

    Gate *gate = &level->items[y];

    if (gate->type != ItemType::BLANK) { 

      if (y >= 3 && y <= 8) oneGateUsed = true;

      bool inputA = false;
      bool inputB = false;
      bool output = false;

      if (gate->type == ItemType::NOT) { inputB = true; }
      if (gate->type == ItemType::LED) { inputB = true; output = true; }

      for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

        Connector *connector = &connectors[x];

        if (connector->active) {

          if (connector->fromItem == y)                                            output = true;
          if (connector->toItem == y && connector->toInput == CONNECTOR_INPUT_A)   inputA = true;
          if (connector->toItem == y && connector->toInput == CONNECTOR_INPUT_B)   inputB = true;

        }

      }

      if (!inputA || !inputB || !output) {

        return false;

      }

    }

  }

  return oneGateUsed;

}