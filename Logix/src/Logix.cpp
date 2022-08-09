#include "src/utils/Arduboy2Ext.h"
#include <ArduboyTones.h>
#include "src/images/Images.h"
#include "src/utils/Enums.h"
#include "src/utils/Utils.h"
#include "src/utils/Data.h"
#include "src/utils/Puzzles.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/sounds/sounds.h"

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);

GameState gameState = GameState::SplashScreen_Init;
Connector connectors[NUMBER_OF_CONNECTORS];
Connector currentConnector;
Selection selection = {0, 0, NO_GATE_SELECTED, 0};

ItemType oldType;
Level level;
uint8_t levelNumber = 0;
uint8_t counter = 0;
uint8_t resetLevelCounter = 0;
uint8_t skipLevelCounter = 0;

bool showMessageYes = true;
bool testBed = false;

uint8_t menuSelection0 = 0;
uint8_t menuSelection1 = 0;
ItemType gateIndex = ItemType::AND;
uint8_t successCount = 0;

uint8_t displayGateSelector = 0;
bool displayConnectorSelect = false;


static constexpr uint8_t ppotFrames = (1.6 *60); // 1.6s animation loop

// Playdate
void GamePlay_ShowMessage();
void renderDataLine(uint8_t &start);
void LevelInit(uint8_t levelNumber);
void LevelDisplay();
void Intro();
void render();
void GamePlay_Normal();

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void splashScreen_Init() {
    gameState = GameState::SplashScreen;
}


// ----------------------------------------------------------------------------
//  Handle state updates ..
//
void splashScreen() {

    auto justPressed = arduboy.justPressedButtons();

    if (justPressed > 0) {

        gameState = GameState::IntroInit;

    }


    Sprites::drawOverwrite(32, 16, Images::PPOT, 0);

    uint8_t p = 0x11; // Default pixel position 0 is hidden in the image

    switch (arduboy.getFrameCount(ppotFrames)) {

        // 2nd quarter of frames
        case (ppotFrames*1/4) ... (ppotFrames*2/4)-1:
            p = 0x31; // Move pixel down to position 1
            /*-fallthrough*/

            // 1st quarter of frames
        case 0 ... (ppotFrames*1/4)-1:
            // Flash 'Play' arrow by clearing the image
            // Overwrite directly to the screen buffer
            arduboy.sBuffer[91 +(3*WIDTH)] = 0x00;
            arduboy.sBuffer[92 +(3*WIDTH)] = 0x00;
            arduboy.sBuffer[93 +(3*WIDTH)] = 0x00;
            arduboy.sBuffer[94 +(3*WIDTH)] = 0x00;
            arduboy.sBuffer[95 +(3*WIDTH)] = 0x00;
            break;

            // 3rd quarter of frames
        case (ppotFrames*2/4) ... (ppotFrames*3/4)-1:
            p = 0x51; // Move pixel down to position 2
            break;

            // 4th quarter of frames
        default:
            p = 0x91; // Move pixel down to position 3
            break;

    }

    // Draw pixel to represent the tape spooling
    // Render directly to the screen buffer
    arduboy.sBuffer[52 +(3*WIDTH)] = p; // Values 0x11, 0x31, 0x51, 0x91

    // On even steps of pixel movement, update the spindle image
    if ((p & 0xA0) == 0) {
        // Draw directly to the screen buffer
        arduboy.sBuffer[45 +(3*WIDTH)] = 0xA5;
        arduboy.sBuffer[46 +(3*WIDTH)] = 0x95;
    }

}

//----------------------------------------------------------------------------------------------------------------------
// Play the Game!
//----------------------------------------------------------------------------------------------------------------------

void GamePlay() {

    if (level.showMessage) {

        selection.reset();
        GamePlay_ShowMessage();

    }
    else {

        GamePlay_Normal();

    }

}


// --------------------------------------------------------------------------------------
//  If the user has just completed the previous puzzle, prompt for the next level ..
//
void GamePlay_ShowMessage() {

    if (arduboy.justPressed(LEFT_BUTTON) && !showMessageYes) showMessageYes = !showMessageYes;
    if (arduboy.justPressed(RIGHT_BUTTON) && showMessageYes) showMessageYes = !showMessageYes;

    if (levelNumber != NUMBER_OF_PUZZLES - 1) {

        if (arduboy.justPressed(A_BUTTON) && showMessageYes)  {

            levelNumber++;
            EEPROM_Utils::setLevel(levelNumber);
            level.showMessage = false;
            gameState = GameState::LevelInit;

        }

        if (arduboy.justPressed(A_BUTTON) && !showMessageYes)  {

            level.showMessage = false;

        }

    }

    render();

}


// --------------------------------------------------------------------------------------
//  Normal play ..
//
void GamePlay_Normal() {

    displayGateSelector = 0;
    displayConnectorSelect = false;


    //----------------------------------------------------------------------------------------------------------------------
    // Has the user selected the menu?

    if (selection.item == SELECTED_MENU && arduboy.justPressed(A_BUTTON)) {

        gameState = GameState::GameMenu;

    }


        //----------------------------------------------------------------------------------------------------------------------
        // Handle simple navigation around the screen ..

    else if (!arduboy.pressed(B_BUTTON) && !arduboy.justReleased(B_BUTTON) && !arduboy.pressed(A_BUTTON) && !arduboy.justReleased(A_BUTTON)) {

        if (column(selection.item) <= 3) {

            if (arduboy.justPressed(UP_BUTTON) && selection.item % 3 > 0)            { selection.item = selection.item - 1; clearStatus(&selection, &currentConnector); }
            if (arduboy.justPressed(DOWN_BUTTON) && selection.item % 3 < 2)          { selection.item = selection.item + 1; clearStatus(&selection, &currentConnector); }

        }

        if (arduboy.justPressed(LEFT_BUTTON) && selection.item > 2 && selection.item != SELECTED_MENU)   { selection.item = selection.item - 3; clearStatus(&selection, &currentConnector); }
        else if (arduboy.justPressed(LEFT_BUTTON) && selection.item == SELECTED_MENU)  { selection.item = 9; clearStatus(&selection, &currentConnector); }

        if (arduboy.justPressed(RIGHT_BUTTON) && selection.item < 9)               { selection.item = selection.item + 3; clearStatus(&selection, &currentConnector); }
        else if (arduboy.justPressed(RIGHT_BUTTON) && selection.item >= 9)         { selection.item = SELECTED_MENU; clearStatus(&selection, &currentConnector); }

    }


        //----------------------------------------------------------------------------------------------------------------------
        // Handle the new connection ..

    else if (arduboy.pressed(A_BUTTON)) {

        if (level.items[selection.item].type != ItemType::BLANK) {

            if (arduboy.justPressed(A_BUTTON)) {

                selection.connector = (selection.connector + 1) % 3;

                if (level.items[selection.item].type == ItemType::NOT && selection.connector == 1) selection.connector = 2;
                if (level.items[selection.item].type == ItemType::LED) selection.connector = CONNECTOR_INPUT_A;

                selection.otherItem = NO_GATE_SELECTED;
                currentConnector.reset();

            }

            displayConnectorSelect = true;


            switch (selection.item) {

                case 0 ... 2: // switches

                    switch (selection.connector) {

                        case CONNECTOR_INPUT_A ... CONNECTOR_INPUT_B:
                        {
                            if (arduboy.justPressed(LEFT_BUTTON)) {

                                switch (selection.otherItem) {

                                    case NO_GATE_SELECTED:
                                        selection.otherItem = TERMINAL_MINUS;
                                        break;

                                    case TERMINAL_MINUS:
                                        selection.otherItem = TERMINAL_PLUS;
                                        break;

                                }

                            }

                            if (arduboy.justPressed(RIGHT_BUTTON)) {

                                switch (selection.otherItem) {

                                    case TERMINAL_PLUS:
                                        selection.otherItem = TERMINAL_MINUS;
                                        break;

                                    case TERMINAL_MINUS:
                                        selection.otherItem = NO_GATE_SELECTED;
                                        break;

                                }

                            }

                            currentConnector.reset();

                            if (selection.otherItem == TERMINAL_PLUS) {

                                currentConnector.x1 = 12;
                                currentConnector.y1 = 9 + (selection.item * 18) + (selection.connector == 0 ? 0 : 6);
                                currentConnector.x2 = 1;
                                currentConnector.y2 = currentConnector.y1;

                            }

                            if (selection.otherItem == TERMINAL_MINUS) {

                                currentConnector.x1 = 12;
                                currentConnector.y1 = 9 + (selection.item * 18) + (selection.connector == 0 ? 0 : 6);
                                currentConnector.x2 = 4;
                                currentConnector.y2 = currentConnector.y1;

                            }

                        }
                            break;

                        case CONNECTOR_OUTPUT:
                        {
                            if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                            if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                            if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }
                            if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }

                            if (selection.otherItem != NO_GATE_SELECTED) {

                                int8_t colDiff = column(selection.otherItem) - column(selection.item);


                                // Draw connector ..

                                currentConnector.x1 = 30;
                                currentConnector.y1 = 12 + yOffset(selection.item) + (selection.item * 18);

                                currentConnector.x2 = currentConnector.x1 + ((colDiff - 1) * 28) + 2 + row(selection.item) * 2;
                                currentConnector.y2 = currentConnector.y1;
                                currentConnector.x3 = currentConnector.x2;
                                currentConnector.y3 = 8 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18) + (level.items[selection.otherItem].type == ItemType::NOT ? 4 : 0) + (selection.otherConnector == CONNECTOR_INPUT_A ? 0 : 8);
                                currentConnector.x4 = currentConnector.x1 + 11 + ((colDiff - 1) * 28);
                                currentConnector.y4 = currentConnector.y3;

                            }
                            else {

                                currentConnector.reset();

                            }

                        }
                            break;

                    }
                    break;

                case 3 ... 8: // gates

                    switch (selection.connector) {

                        case CONNECTOR_INPUT_A ... CONNECTOR_INPUT_B:
                        {
                            if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_ALLOW_PLUS); }
                            if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_ALLOW_MINUS); }
                            if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }
                            if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }

                            currentConnector.reset();
                            currentConnector.x1 = 13 + (column(selection.item) * 28);
                            currentConnector.y1 = 8 + (level.items[selection.item].type == ItemType::NOT ? 4 : 0) + yOffset(selection.item) + (row(selection.item) * 18) + (selection.connector == 0 ? 0 : 8);

                            switch (selection.otherItem) {

                                case TERMINAL_MINUS:
                                    currentConnector.x2 = currentConnector.x1 - 2 - (row(selection.item) * 2);
                                    currentConnector.y2 = currentConnector.y1;
                                    currentConnector.x3 = currentConnector.x2;
                                    currentConnector.y3 = 62;
                                    break;

                                case TERMINAL_PLUS:
                                    currentConnector.x2 = currentConnector.x1 - 2 - (row(selection.item) * 2);
                                    currentConnector.y2 = currentConnector.y1;
                                    currentConnector.x3 = currentConnector.x2;
                                    currentConnector.y3 = 2;
                                    break;

                                case NO_GATE_SELECTED: break;

                                default:
                                {
                                    int8_t colDiff = column(selection.item) - column(selection.otherItem);
                                    currentConnector.x2 = 20 + currentConnector.x1 - (colDiff * 28) + (row(selection.otherItem) * 2);
                                    currentConnector.y2 = currentConnector.y1;
                                    currentConnector.x3 = currentConnector.x2;
                                    currentConnector.y3 = 12 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18);
                                    currentConnector.x4 = 26 + (column(selection.otherItem) * 28) + (column(selection.otherItem) == 0 ? 4 : 0);
                                    currentConnector.y4 = currentConnector.y3;
                                }
                                    break;

                            }

                        }
                            break;

                        case CONNECTOR_OUTPUT:
                        {
                            if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                            if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                            if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }
                            if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }

                            if (selection.otherItem != NO_GATE_SELECTED) {

                                int8_t colDiff = column(selection.otherItem) - column(selection.item);


                                // Draw connector ..

                                currentConnector.x1 = 30 + (column(selection.item) * 28);
                                currentConnector.y1 = 12 + yOffset(selection.item) + (row(selection.item) * 18);
                                currentConnector.x2 = currentConnector.x1 + ((colDiff - 1) * 28) + 2 + (row(selection.item) * 2);
                                currentConnector.y2 = currentConnector.y1;
                                currentConnector.x3 = currentConnector.x2;
                                currentConnector.y3 = 8 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18) + (level.items[selection.otherItem].type == ItemType::NOT ? 4 : 0) + (selection.otherConnector == CONNECTOR_INPUT_A ? 0 : 8);
                                currentConnector.x4 = 2 + (column(selection.item) * 28) + ((colDiff + 0) * 28) + 11;
                                currentConnector.y4 = currentConnector.y3;

                            }
                            else {

                                currentConnector.reset();

                            }

                        }
                            break;

                    }

                    break;

                case 9 ... 11: // LEDs
                {
                    if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                    if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                    if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }
                    if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }

                    currentConnector.reset();

                    int8_t colDiff = column(selection.item) - column(selection.otherItem);

                    if (selection.otherItem != NO_GATE_SELECTED) {

                        currentConnector.x1 = 13 + (column(selection.item) * 28);
                        currentConnector.y1 = 8 + yOffset(selection.item) + (row(selection.item) * 18) + (selection.connector == 0 ? 0 : 8);
                        currentConnector.x2 = 20 + currentConnector.x1 - (colDiff * 28) + (row(selection.otherItem) * 2);
                        currentConnector.y2 = currentConnector.y1;
                        currentConnector.x3 = currentConnector.x2;
                        currentConnector.y3 = 12 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18);
                        currentConnector.x4 = 26 + (column(selection.otherItem) * 28) + (column(selection.otherItem) == 0 ? 4 : 0);
                        currentConnector.y4 = currentConnector.y3;

                    }

                }

                    break;

            }

        }

    }


        //----------------------------------------------------------------------------------------------------------------------
        // Create the new connection ..

    else if (arduboy.justReleased(A_BUTTON)) {

        createOrDeleteConnection(&selection, &currentConnector, connectors);
        updateResults(level.items, connectors);

    }


        //----------------------------------------------------------------------------------------------------------------------
        // Handle change of gate ..

    else if (arduboy.pressed(B_BUTTON)) {

        if (arduboy.justPressed(B_BUTTON)) {
            oldType = level.items[selection.item].type;
        }

        switch (column(selection.item)) {

            case 0 ... 2:
            {

                Gate *gate = &level.items[selection.item];

                if (gate->editable) {

                    if (arduboy.justPressed(UP_BUTTON)) {

                        gate->type = ++gate->type;
                        updateResults(level.items, connectors);

                    }

                    if (arduboy.justPressed(DOWN_BUTTON)) {

                        gate->type = --gate->type;
                        updateResults(level.items, connectors);

                    }

                    displayGateSelector = 1;

                }
                else {

                    displayGateSelector = 2;

                }

            }

                break;

            case 3:
                displayGateSelector = 2;
                break;

        }

    }


        //----------------------------------------------------------------------------------------------------------------------
        // If the gate has been changed then delete the connections ..

    else if (arduboy.justReleased(B_BUTTON)) {

        if (selection.item != SELECTED_MENU && selection.item != NO_GATE_SELECTED && oldType != ItemType::BLANK && oldType != level.items[selection.item].type && (
                (oldType <= ItemType::XNOR && level.items[selection.item].type > ItemType::XNOR) ||
                (oldType > ItemType::XNOR && level.items[selection.item].type <= ItemType::XNOR) ||
                (level.items[selection.item].type == ItemType::BLANK) ||
                (level.items[selection.item].type == ItemType::NOT)
        )) {

            clearConnectors(connectors, selection.item);

        }

    }

    render();

}

const uint8_t y[] = { 4, 13, 22, 31, 43 };

// --------------------------------------------------------------------------------------
//  Render the play field ..
//
void render() {


    // Render highlight ..

    const uint8_t highlight_X[] = { 11, 40, 68, 96 };
    const uint8_t highlight_Y[] = { 3, 21, 39, 5, 23, 41, 3, 21, 39, 4, 22, 40 };


    // Render logic gates ..

    switch (selection.item) {

        case 0 ... 11:
            Sprites::drawSelfMasked(highlight_X[column(selection.item)], highlight_Y[selection.item], gateHighlight, 0);
            Sprites::drawOverwrite(123, 44, menu, 0);
            break;

        case SELECTED_MENU:
            if (gameState == GameState::GamePlay) Sprites::drawOverwrite(122, 38, menuSelect, 0);
            break;

        default: break;

    }


    // Render connectors ..

    for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

        Connector *connector = &connectors[x];

        if (connector->active) {

            if (connector->x1 > 0 && connector->x2 > 0)     { arduboy.drawLine(connector->x1, connector->y1, connector->x2, connector->y2); }
            if (connector->x2 > 0 && connector->x3 > 0)     { arduboy.drawLine(connector->x2, connector->y2, connector->x3, connector->y3); }
            if (connector->x3 > 0 && connector->x4 > 0)     { arduboy.drawLine(connector->x3, connector->y3, connector->x4, connector->y4); }

        }

    }


    // Render current connector.  Flash between black and white ..

    uint8_t colour = (arduboy.getFrameCount() % 32 < 16 ? WHITE : BLACK);

    if (currentConnector.x1 > 0 && currentConnector.x2 > 0 && currentConnector.x1 == currentConnector.x2)     { arduboy.drawVerticalDottedLine(currentConnector.y1, currentConnector.y2, currentConnector.x1, colour); }
    if (currentConnector.x1 > 0 && currentConnector.x2 > 0 && currentConnector.y1 == currentConnector.y2)     { arduboy.drawHorizontalDottedLine(currentConnector.x1, currentConnector.x2, currentConnector.y1, colour); }

    if (currentConnector.x2 > 0 && currentConnector.x3 > 0 && currentConnector.x2 == currentConnector.x3)     { arduboy.drawVerticalDottedLine(currentConnector.y2, currentConnector.y3, currentConnector.x2, colour); }
    if (currentConnector.x2 > 0 && currentConnector.x3 > 0 && currentConnector.y2 == currentConnector.y3)     { arduboy.drawHorizontalDottedLine(currentConnector.x2, currentConnector.x3, currentConnector.y2, colour); }

    if (currentConnector.x3 > 0 && currentConnector.x4 > 0 && currentConnector.x3 == currentConnector.x4)     { arduboy.drawVerticalDottedLine(currentConnector.y3, currentConnector.y4, currentConnector.x3, colour); }
    if (currentConnector.x3 > 0 && currentConnector.x4 > 0 && currentConnector.y3 == currentConnector.y4)     { arduboy.drawHorizontalDottedLine(currentConnector.x3, currentConnector.x4, currentConnector.y3, colour); }


    // Render logic gates ..

    if (level.items[0].type >= ItemType::SWITCH_UP && level.items[0].type <= ItemType::SWITCH_DOWN)   Sprites::drawSelfMasked(13, 8, switches, static_cast<uint8_t>(level.items[0].type) - static_cast<uint8_t>(ItemType::SWITCH_UP));
    if (level.items[1].type >= ItemType::SWITCH_UP && level.items[1].type <= ItemType::SWITCH_DOWN)   Sprites::drawSelfMasked(13, 26, switches, static_cast<uint8_t>(level.items[1].type) - static_cast<uint8_t>(ItemType::SWITCH_UP));
    if (level.items[2].type >= ItemType::SWITCH_UP && level.items[2].type <= ItemType::SWITCH_DOWN)   Sprites::drawSelfMasked(13, 44, switches, static_cast<uint8_t>(level.items[2].type) - static_cast<uint8_t>(ItemType::SWITCH_UP));

    Sprites::drawExternalMask(42, 7, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[3].type), static_cast<uint8_t>(level.items[3].type));
    Sprites::drawExternalMask(42, 25, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[4].type), static_cast<uint8_t>(level.items[4].type));
    Sprites::drawExternalMask(42, 43, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[5].type), static_cast<uint8_t>(level.items[5].type));

    Sprites::drawExternalMask(70, 5, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[6].type), static_cast<uint8_t>(level.items[6].type));
    Sprites::drawExternalMask(70, 23, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[7].type), static_cast<uint8_t>(level.items[7].type));
    Sprites::drawExternalMask(70, 41, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[8].type), static_cast<uint8_t>(level.items[8].type));

    if (level.items[9].type == ItemType::LED)  {

        Sprites::drawSelfMasked(97, 3, LED, (level.items[9].output() ? (arduboy.getFrameCountAlternate(FLASHING_LED_DELAY) ? 1 : 2) : 0));
        arduboy.drawLine(113, 14, 118, 14);
        arduboy.drawLine(118, 14, 118, 63);

    }

    if (level.items[10].type == ItemType::LED) {

        Sprites::drawSelfMasked(97, 21, LED, (level.items[10].output() ? (arduboy.getFrameCountAlternate(FLASHING_LED_DELAY) ? 1 : 2) : 0));
        arduboy.drawLine(113, 32, 118, 32);
        arduboy.drawLine(118, 32, 118, 63);

    }

    if (level.items[11].type == ItemType::LED) {

        Sprites::drawSelfMasked(97, 39, LED, (level.items[11].output() ? (arduboy.getFrameCountAlternate(FLASHING_LED_DELAY) ? 1 : 2) : 0));
        arduboy.drawLine(113, 50, 118, 50);
        arduboy.drawLine(118, 50, 118, 63);

    }


    // Draw remainder of layout ..

    arduboy.drawLine(0, 0, 118, 0);
    arduboy.drawLine(0, 5, 0, 55);
    arduboy.drawLine(4, 7, 4, 58);
    arduboy.drawLine(0, 63, 118, 63);
    arduboy.drawVerticalDottedLine(0, 63, 120, WHITE);
    Sprites::drawSelfMasked(0, 0, plus, 0);
    Sprites::drawSelfMasked(0, 58, minus, 0);


    // If the gate selector and the connetor selector are not being shown, show the level ..

    if (displayGateSelector == 0 && !displayConnectorSelect) {

        Sprites::drawOverwrite(123, 0, level_sideways, 0);
        Sprites::drawOverwrite(123, 24, numbers_sideways, level.id / 10);
        Sprites::drawOverwrite(123, 29, numbers_sideways, level.id % 10);

    }


    // Display gate selector (up / down) ..

    if (displayGateSelector == 1) {

        if (selection.item <= 8) { Sprites::drawExternalMask( 119, -1, upDown, upDown_Mask, 0, 0); }

    }

    if (displayGateSelector == 2) {

        if (selection.item <= 8) { Sprites::drawExternalMask( 119, -1, upDown_Locked, upDown_Mask, 0, 0); }

    }


    // Display connector selection ..

    if (displayConnectorSelect ) {

        if (selection.item <= 11) {

            switch (level.items[selection.item].type) {

                case ItemType::NOT:
                case ItemType::LED:
                    Sprites::drawExternalMask( 119, -1, connectorSelect, connectorSelect_mask, selection.connector + 3, 0);
                    break;

                default:
                    Sprites::drawExternalMask( 119, -1, connectorSelect, connectorSelect_mask, selection.connector, 0);
                    break;

            }

        }

    }


    // Show next level message?

    if (level.showMessage && gameState == GameState::GamePlay) {

        if (levelNumber == NUMBER_OF_PUZZLES - 1) {

            arduboy.fillRect(15, 20, 86, 24, BLACK);
            arduboy.drawHorizontalDottedLine(18, 100, 23, WHITE);
            arduboy.drawHorizontalDottedLine(18, 100, 38, WHITE);

            arduboy.setCursor(21, 27);
            arduboy.setTextColor(WHITE);
            arduboy.print("Congratulations!");

        }
        else {

            arduboy.fillRect(17, 20, 82, 24, BLACK);
            arduboy.drawHorizontalDottedLine(20, 96, 23, WHITE);
            arduboy.drawHorizontalDottedLine(20, 96, 39, WHITE);

            arduboy.setCursor(22, 27);
            arduboy.setTextColor(WHITE);
            arduboy.print("Next level? ");

            if (showMessageYes) {
                arduboy.fillRect(81, 27, 6, 8, WHITE);
                arduboy.setTextColor(BLACK);
            }
            else {
                arduboy.setTextColor(WHITE);
            }

            arduboy.print("Y");
            arduboy.setCursor(90, 27);

            if (!showMessageYes) {
                arduboy.fillRect(89, 27, 6, 8, WHITE);
                arduboy.setTextColor(BLACK);
            }
            else {
                arduboy.setTextColor(WHITE);
            }

            arduboy.print("N");

        }

    }

}


// --------------------------------------------------------------------------------------
//  Render current puzzle challenge ..
//
void drawChallenges(uint8_t xOffset, uint8_t yOffset) {

    for (uint8_t x = 0; x < level.numberOfOutcomes; x++) {

        arduboy.setCursor(xOffset + (x * 25) + (level.numberOfOutcomes == 2 ? 12 : 0), yOffset);
        arduboy.print("S L");
        arduboy.setCursor(xOffset + (x * 25) + (level.numberOfOutcomes == 2 ? 12 : 0), yOffset + 11);


        // Row 1 ---------------------------------------------------------

        if (level.outcome[x].switchState0 <= 1) {

            arduboy.print(level.outcome[x].switchState0);

        }
        else {

            arduboy.print(" ");

        }

        arduboy.print(" ");


        if (level.outcome[x].ledState0 <= 1) {
            arduboy.print(level.outcome[x].ledState0);
        }

        arduboy.print("\n");


        // Row 2 ---------------------------------------------------------

        if (level.outcome[x].switchState1 <= 1) {

            arduboy.print(level.outcome[x].switchState1);

        }
        else {

            arduboy.print(" ");

        }

        arduboy.print(" ");


        if (level.outcome[x].ledState1 <= 1) {
            arduboy.print(level.outcome[x].ledState1);
        }

        arduboy.print("\n");


        // Row 3 ---------------------------------------------------------

        if (level.outcome[x].switchState2 <= 1) {

            arduboy.print(level.outcome[x].switchState2);

        }
        else {

            arduboy.print(" ");

        }

        arduboy.print(" ");


        if (level.outcome[x].ledState2 <= 1) {
            arduboy.print(level.outcome[x].ledState2);
        }

        arduboy.print("\n");


        // Divider -------------------------------------------------------

        if (x < level.numberOfOutcomes - 1) {

            arduboy.drawVerticalDottedLine(yOffset + 12, yOffset + 36, xOffset + 19 + (level.numberOfOutcomes == 2 ? 12 : 0) + (x * 25), WHITE);

        }

    }

}


// --------------------------------------------------------------------------------------
//  Render cross hatch effect.  Counter range 0 - 32 ..
//
void renderCrossHatch(uint8_t counter) {

    for (uint8_t x = counter; x < 32; x++) {

        arduboy.drawHorizontalDottedLine((x%2), 128, x, BLACK);
        arduboy.drawHorizontalDottedLine((x%2), 128, 63 - x, BLACK);

    }

}

// ----------------------------------------------------------------------------------------------------------------------
//  Play the Game!
// ----------------------------------------------------------------------------------------------------------------------

void GameMenu() {

    bool islevelWiredCompletely = (testBed ? false : islevelFullyWired(&level, connectors));

    if (!islevelWiredCompletely && menuSelection0 == 1) menuSelection0 = 0;

    render();

    arduboy.fillRect(0, 0, 40, 55, BLACK);
    arduboy.drawRect(1, 1, 38, 53, WHITE);
    arduboy.drawHorizontalDottedLine( 5, 36, 41, WHITE);

    arduboy.setTextColor(WHITE);
    arduboy.setCursor(8, 4);
    arduboy.print(F("Back\nTest\nInfo\nHelp"));
    arduboy.setCursor(8, 43);
    arduboy.print(F("Quit"));
    arduboy.setTextColor(BLACK);
    arduboy.setCursor(8, y[menuSelection0]);
    arduboy.fillRect(4, y[menuSelection0], 32, 8, WHITE);

    if (!islevelWiredCompletely) {

        Sprites::drawOverwrite(30, 13, padlock, 0);

    }


    // Top level seelection ..

    switch (menuSelection0) {

        case 0:
            arduboy.print(F("Back"));
            break;

        case 1:
            arduboy.print(F("Test"));
            break;

        case 2:
            arduboy.print(F("Info"));
            Sprites::drawErase(31, 24, arrowRight, 0);
            break;

        case 3:
            arduboy.print(F("Help"));
            Sprites::drawErase(31, 33, arrowRight, 0);
            break;

        case 4:
            arduboy.setCursor(8, 43);
            arduboy.print(F("Quit"));
            break;

    }


    // Second level menu items ..

    arduboy.setTextColor(WHITE);

    switch (menuSelection1) {

        case 1:
        {

            if (successCount < 100) { successCount++; }

            arduboy.setRGBled(0, 0, 0);
            if (successCount > 0 && successCount < 16 && level.numberOfOutcomes >= 1)   { arduboy.setRGBled((!level.outcome[0].successful ? 32 : 0), (level.outcome[0].successful ? 32 : 0), 0); }
            if (successCount > 32 && successCount < 48 && level.numberOfOutcomes >= 2)  { arduboy.setRGBled((!level.outcome[1].successful ? 32 : 0), (level.outcome[1].successful ? 32 : 0), 0); }
            if (successCount > 64 && successCount < 80 && level.numberOfOutcomes >= 3)  { arduboy.setRGBled((!level.outcome[2].successful ? 32 : 0), (level.outcome[2].successful ? 32 : 0), 0); }

            arduboy.fillRect(36, 8, 78, 57, BLACK);
            arduboy.drawRect(37, 9, 76, 55, WHITE);
            arduboy.drawHorizontalDottedLine(40, 110, 19, WHITE);
            arduboy.setCursor(42, 10);
            arduboy.print("Test Results ");

            for (uint8_t x = 0; x < level.numberOfOutcomes; x++) {

                arduboy.setCursor(43, 24 + (x * 13));
                arduboy.print("Test ");
                arduboy.print(x + 1);
                arduboy.print(" ... ");

                if (level.outcome[x].successful) {

                    Sprites::drawSelfMasked(96, 23 + (x * 13), smile, 0);

                }
                else {

                    Sprites::drawSelfMasked(96, 23 + (x * 13), frown, 0);

                }

            }

            if (level.levelComplete()) {

                level.showMessage = true;
                selection.item = 0;

            }

        }

            break;

        case 2:
        {
            arduboy.fillRect(36, 8, 78, 57, BLACK);
            arduboy.drawRect(37, 9, 76, 55, WHITE);
            arduboy.drawHorizontalDottedLine(40, 110, 19, WHITE);
            arduboy.setCursor(42, 10);
            arduboy.print("Level ");
            arduboy.print(level.id / 10);
            arduboy.print(level.id % 10);

            drawChallenges(43, 23);

        }

            break;

        case 3:
        {
            arduboy.fillRect(36, 8, 81, 57, BLACK);
            arduboy.drawRect(37, 9, 79, 55, WHITE);
            arduboy.fillRect(113, 9, 7, 55, WHITE);
            Sprites::drawOverwrite(114, 10, arrowUp, 0);
            Sprites::drawOverwrite(114, 54, arrowDown, 0);
            arduboy.drawHorizontalDottedLine(40, 110, 19, WHITE);

            Sprites::drawOverwrite(51, 33, logicGates, static_cast<uint8_t>(gateIndex));

            arduboy.setCursor(40, 10);
            arduboy.print(itemTypes[static_cast<uint8_t>(gateIndex)]);

            if (gateIndex != ItemType::NOT) {

                arduboy.setCursor(85, 10);
                arduboy.print(F("A B O"));
                arduboy.setCursor(42, 32);
                arduboy.print(F("A\nB"));
            }
            else {
                arduboy.setCursor(85, 10);
                arduboy.print(F("A   O"));
                arduboy.setCursor(42, 36);
                arduboy.print(F("A"));
            }
            arduboy.setCursor(71, 37);
            arduboy.print(F("O"));

            uint8_t start = (static_cast<uint8_t>(gateIndex) * 12);

            arduboy.setCursor(85, 21);
            renderDataLine(start);
            arduboy.drawHorizontalDottedLine(82, 110, 30, WHITE);
            arduboy.setCursor(85, 32);
            renderDataLine(start);
            arduboy.drawHorizontalDottedLine(82, 110, 41, WHITE);

            if (gateIndex != ItemType::NOT) {

                arduboy.setCursor(85, 43);
                renderDataLine(start);
                arduboy.drawHorizontalDottedLine(82, 110, 52, WHITE);
                arduboy.setCursor(85, 54);
                renderDataLine(start);
                arduboy.drawHorizontalDottedLine(82, 110, 63, WHITE);

            }

        }
            break;

    }


    // Handle keypresses ..

    if (arduboy.justPressed(UP_BUTTON) && menuSelection0 > 0 && menuSelection1 == 0)      {
        if (menuSelection0 == 2 && !islevelWiredCompletely) {
            menuSelection0 = 0;
        }
        else {
            menuSelection0--;
        }
    }

    if (arduboy.justPressed(UP_BUTTON) && menuSelection1 == 3)                            { gateIndex++; if (gateIndex == ItemType::BLANK) gateIndex = ItemType::AND; }

    if (arduboy.justPressed(DOWN_BUTTON) && menuSelection0 < 4 && menuSelection1 == 0)    {
        if (menuSelection0 == 0 && !islevelWiredCompletely) {
            menuSelection0 = 2;
        }
        else {
            menuSelection0++;
        }
    }

    if (arduboy.justPressed(DOWN_BUTTON) && menuSelection1 == 3)                          { gateIndex--; if (gateIndex == ItemType::BLANK) gateIndex = ItemType::NOT; }

    if (arduboy.justPressed(RIGHT_BUTTON) && menuSelection0 == 2 && menuSelection1 == 0)  { menuSelection1 = 2; }
    if (arduboy.justPressed(RIGHT_BUTTON) && menuSelection0 == 3 && menuSelection1 == 0)  { menuSelection1 = 3; }
    if (arduboy.justPressed(LEFT_BUTTON) && menuSelection1 > 0)                           { menuSelection1 = 0; arduboy.setRGBled(0, 0, 0); }
    if (arduboy.justPressed(B_BUTTON) && menuSelection1 == 0)                             { selection.reset(); gameState = GameState::GamePlay; }
    if (arduboy.justPressed(B_BUTTON) && menuSelection1 > 0)                              { menuSelection1 = 0; arduboy.setRGBled(0, 0, 0); }

    if (arduboy.justPressed(A_BUTTON)) {

        switch (menuSelection0) {

            case 0:
                selection.reset();
                gameState = GameState::GamePlay;
                break;

            case 1:
                successCount = 0;
                test(&level, connectors);
                sound.tones(level.levelComplete() ? success : fail);
                menuSelection1 = 1;
                break;

            case 2:
                menuSelection1 = 2;
                break;

            case 3:
                menuSelection1 = 3;
                break;

            case 4:
                menuSelection0 = 0;
                menuSelection1 = 0;
                gameState = GameState::IntroInit;
                break;

        }

    }

}


// --------------------------------------------------------------------------------------
//  Render a single puzzle outcome line ..
//
void renderDataLine(uint8_t &start) {

    arduboy.print(pgm_read_byte(&data[start++]));
    arduboy.print(" ");

    uint8_t x = pgm_read_byte(&data[start++]);

    if (x < 2) {
        arduboy.print(x);
    }
    else {
        arduboy.print(" ");
    }

    arduboy.print(" ");
    arduboy.print(pgm_read_byte(&data[start++]));

}

// ----------------------------------------------------------------------------------------------------------------------
//  Setup the game.
// ----------------------------------------------------------------------------------------------------------------------

void setup() {

  arduboy.boot();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
  arduboy.setTextWrap(true);
  arduboy.setTextVertSpacing(9);
  arduboy.audio.begin();
  sound.begin();
  
  EEPROM_Utils::initEEPROM(false);
  levelNumber = EEPROM_Utils::getLevel();

}


// ----------------------------------------------------------------------------------------------------------------------
//  Main Loop
// ----------------------------------------------------------------------------------------------------------------------

void loop() {
  sound.callback();
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::SplashScreen_Init:
        splashScreen_Init();
        splashScreen();
        arduboy.display(false);
        break;

    case GameState::SplashScreen:
        splashScreen();
        arduboy.display(false);
        break;
        
    case GameState::IntroInit:
        counter = COUNTER_DELAY;
        gameState = GameState::Intro;
        sound.tones(score);

    case GameState::Intro:
        Intro();
        break;

    case GameState::LevelInit:
        sound.noTone();

        if (testBed) {
          LevelInit(TEST_BED);
          gameState = GameState::GamePlay;
          break;
        }
        else {
          LevelInit(levelNumber);
        }
        // Drop through intended ..

    case GameState::LevelDisplay:
        LevelDisplay();
        break;

    case GameState::GamePlay:
        GamePlay();
        break;

    case GameState::GameMenu:
        GameMenu();
        break;

  }

  arduboy.display(CLEAR_BUFFER);

}


// --------------------------------------------------------------------------------------
//  Display introduction page and animation ..
//
void Intro() {

  bool renderLevel = false;

  Sprites::drawOverwrite(0, 0, pcb, 0);

  if (arduboy.everyXFrames(2))    { if (counter > 0) counter--; }
  if (counter < 32)               { renderCrossHatch(counter); }
  if (counter == 0)               { Sprites::drawExternalMask(3, 14, logix, logix_mask, 0, 0); }


  // Reset levels?

  if (arduboy.pressed(UP_BUTTON) &&  arduboy.pressed(DOWN_BUTTON)) {

    resetLevelCounter++;

    switch (resetLevelCounter) {

      case 21 ... 60:
        arduboy.setRGBled(128 - (resetLevelCounter * 2), 0, 0);
        break;

      case 61:
        resetLevelCounter = 0;
        arduboy.setRGBled(0, 0, 0);
        EEPROM_Utils::initEEPROM(true);
        levelNumber = 0;
        return;

    }

  }
  else {

    if (resetLevelCounter > 0) {
    
      arduboy.setRGBled(0, 0, 0);
      resetLevelCounter = 0;

    }
    
  }


  // Skip levels ?

  if (arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(RIGHT_BUTTON)) {

    skipLevelCounter++;

    switch (skipLevelCounter) {

      case 102 ... 139:
        renderLevel = true;
        break;    

      case 80:
      case 140:
        if (arduboy.pressed(LEFT_BUTTON) && levelNumber > 0)                        levelNumber--;
        if (arduboy.pressed(RIGHT_BUTTON) && levelNumber < NUMBER_OF_PUZZLES - 1)   levelNumber++;
        EEPROM_Utils::setLevel(levelNumber);
        renderLevel = true;
        break;

      case 81 ... 100:
      case 141 ... 160:
        arduboy.setRGBled(0, 0, 32);
        renderLevel = true;
        break;

      case 101:
        arduboy.setRGBled(0, 0, 0);
        renderLevel = true;
        break;

      case 161:
        skipLevelCounter = 101;
        arduboy.setRGBled(0, 0, 0);
        renderLevel = true;
        break;

    }

  }
  else {

    if (skipLevelCounter > 0) {

      renderLevel = true;
      if ((arduboy.justReleased(LEFT_BUTTON) || arduboy.justReleased(RIGHT_BUTTON)))  skipLevelCounter = 0;

    }
    
  }


  // If render level ?

  if (renderLevel) {

    arduboy.fillRect(99, 0, 30, 8);
    arduboy.setTextColor(BLACK);
    arduboy.setCursor(101, 0);
    arduboy.print(F("Lvl "));
    arduboy.setCursor(117, 0);
    arduboy.print((levelNumber + 1) / 10);
    arduboy.print((levelNumber + 1) % 10);
    arduboy.setTextColor(WHITE);
        
  }


  // Start game when user presses A button ..

  if (arduboy.justPressed(A_BUTTON)) { testBed = false; gameState = GameState::LevelInit; }
  if (arduboy.justPressed(B_BUTTON)) { testBed = true;  gameState = GameState::LevelInit; }

}


// --------------------------------------------------------------------------------------
//  Initialise level ready for play ..
//
void LevelInit(uint8_t levelNumber) {

  LoadLevel(&level, levelNumber);

  for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) { 

    Connector *connector = &connectors[x];
    connector->reset();

  }

  gameState = GameState::LevelDisplay;
  counter = COUNTER_DELAY_SHORT;

}


// --------------------------------------------------------------------------------------
//  Display 'Level 99' banner prior to play ..
//
void LevelDisplay() {

  Sprites::drawOverwrite(0, 0, pcb, 0);
  renderCrossHatch(0);

  arduboy.fillRect(23, 0, 81, 64, BLACK);
  arduboy.fillRect(25, 0, 77, 10, WHITE);

  arduboy.setCursor(28, 1);
  arduboy.setTextColor(BLACK);
  
  arduboy.print("Level ");
  arduboy.print(level.id / 10);
  arduboy.print(level.id % 10);
  arduboy.setTextColor(WHITE);

  arduboy.drawVerticalDottedLine(0, 64, 25, WHITE);
  arduboy.drawVerticalDottedLine(0, 64, 101, WHITE);
  arduboy.drawHorizontalDottedLine(25, 101, 63, WHITE);

  drawChallenges(32, 13);

  if (counter > 0) counter--;

  if (counter == 0) {
  
    arduboy.setCursor(57, 53);
    arduboy.print("Press");
    Sprites::drawSelfMasked(87, 53, aButton, 0);
  
  }

  if (arduboy.justPressed(A_BUTTON)) {gameState = GameState::GamePlay; }

}
