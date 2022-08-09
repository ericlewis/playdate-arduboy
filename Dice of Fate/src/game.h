#ifndef GAME_H
#define GAME_H

#include "globals.h"

#define AMOUNT_OF_DICE_TYPE             8
#define AMOUNT_OF_ROLLING_DICE          5

byte currentDice;
byte amountOfDice;
byte frameDice;
byte frameCoin;
byte coinSlowDown;
int slideCounter;
boolean slideLeft;
boolean slidingDice;
boolean goBack;
boolean showDiceName;
boolean showResult;
byte diceMax[] = {2, 5, 7, 9, 11, 13, 21, 99};
byte bandPositionX[] = {0, 8, 16, 24, 32, 56, 54, 56};
byte frameBand = 0;
byte diceResult;



struct Dice
{
  public:
    int x, y;
    byte type;
    byte result;
};

Dice rollingDice[AMOUNT_OF_DICE_TYPE];

void placeDice()
{
  for (byte i = 0; i < AMOUNT_OF_DICE_TYPE; i++)
  {
    rollingDice[i].type = i;
    rollingDice[i].x = 8 + (38 * (-(currentDice - 2) + i));
    rollingDice[i].y = 7;
    for (byte k = 0; k < AMOUNT_OF_DICE_TYPE; k++)
    {
      rollingDice[i].result = 0;
    }
  }
  rollingDice[currentDice - 1].y = 1;
  frameDice = 0;
  frameCoin = 0;
  coinSlowDown = 0;
}

void drawNumbers(byte numbersX, byte numbersY, int number)
{
  char buf[10];
  //number = arduboy.cpuLoad();
  itoa(number, buf, 10);
  char charLen = strlen(buf);

  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    byte j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (byte z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    sprites.drawSelfMasked(numbersX - (4 + (5 * (charLen - 1))) + (10 * i), numbersY, allNumbers, digit);
  }
}

void drawBorders()
{
  for (byte i = 0; i < 128; i++)
  {
    sprites.drawPlusMask(i, 24, resultBorders_plus_mask, 0);
  }
  for (byte i = 56; i < 128 - 56; i++)
  {
    sprites.drawPlusMask(i, 24, resultBorders_plus_mask, 1);
  }
}

void drawBands()
{
  sprites.drawSelfMasked(130 - bandPositionX[frameBand], 28, resultBands, 0);
  sprites.drawSelfMasked(bandPositionX[frameBand] - 56, 28, resultBands, 1);

}

void drawResult()
{
  drawNumbers(64, 31, diceResult);
}



void stateMenuPlay()
{
  currentDice = 4;
  placeDice();
  slideLeft = true;
  slidingDice = false;
  goBack = false;
  showDiceName = true;
  slideCounter = 0;
  amountOfDice = 2;
  gameState = STATE_DICE_TYPE_AND_AMOUNT;
}


void stateDiceTypeAndAmount()
{
  if (currentDice < 2 || currentDice > 7) amountOfDice = 1;
  sprites.drawSelfMasked(rollingDice[0].x, rollingDice[0].y, justCoin, 0);
  for (byte i = 1; i < AMOUNT_OF_DICE_TYPE; i++)
  {
    sprites.drawSelfMasked(rollingDice[i].x , rollingDice[i].y, allDice, (3 * (rollingDice[i].type -1)) + frameDice);
  }
  if (arduboy.justPressed(RIGHT_BUTTON) && !slidingDice && (currentDice < AMOUNT_OF_DICE_TYPE))
  {
    slidingDice = true;
    slideLeft = true;
    if (currentDice < 2) amountOfDice = 2;
    currentDice++;
    for (byte i = 0; i < AMOUNT_OF_DICE_TYPE; i++) rollingDice[i].y = 7;
    showDiceName = false;
    sound.tone(300, 20);
  }
  if (arduboy.justPressed(LEFT_BUTTON) && !slidingDice && (currentDice > 1))
  {
    slidingDice = true;
    slideLeft = false;
    if (currentDice > 7) amountOfDice = 2;
    currentDice--;
    for (byte i = 0; i < AMOUNT_OF_DICE_TYPE; i++) rollingDice[i].y = 7;
    showDiceName = false;
    sound.tone(300, 20);
  }
  if (arduboy.justPressed(UP_BUTTON) && (amountOfDice < AMOUNT_OF_ROLLING_DICE) && (currentDice > 1) && (currentDice < 8))
  {
    buttonPressed[BUTTON_UP] = true;
    amountOfDice++;
    sound.tone(600, 20);
  }
  if (arduboy.justPressed(DOWN_BUTTON) && (amountOfDice > 1) && (currentDice > 1) && (currentDice < 8))
  {
    buttonPressed[BUTTON_DOWN] = true;
    amountOfDice--;
    sound.tone(600, 20);
  }

  if (arduboy.justPressed(B_BUTTON))
  {
    buttonPressed[BUTTON_B] = true;
    for (byte i = 0; i < AMOUNT_OF_ROLLING_DICE; i++)
    {
      rollingDice[i].type = currentDice - 1;
      rollingDice[i].x = 46;
      rollingDice[i].y = 1;
      sound.tone(300, 20);
    }
    gameState = STATE_DICE_ROLLING;
  }

  if (arduboy.justPressed(A_BUTTON))
  {
    buttonPressed[BUTTON_A] = true;
    gameState = STATE_MENU_MAIN;
    sound.tone(300, 20);
  }

  if (slideLeft && slidingDice && (slideCounter < 37))
  {
    slideCounter += 2;
    for (byte i = 0; i < AMOUNT_OF_DICE_TYPE; i++)
    {
      rollingDice[i].x -= 2;
    }
  }

  if (!slideLeft && slidingDice && (slideCounter < 37))
  {
    slideCounter += 2;
    for (byte i = 0; i < AMOUNT_OF_DICE_TYPE; i++)
    {
      rollingDice[i].x += 2;
    }
  }
  if (slideCounter == 34)rollingDice[currentDice - 1].y = 6;
  if (slideCounter == 36)rollingDice[currentDice - 1].y = 4;
  if (slideCounter > 37)
  {
    slideCounter = 0;
    slidingDice = false;
    rollingDice[currentDice - 1].y = 1;
    showDiceName = true;
  }
  sprites.drawSelfMasked(52, 48, numberFrame, 0);
  drawNumbers(60, 50, amountOfDice);
  sprites.drawSelfMasked(1, 53, allButtons, 2 * BUTTON_A + buttonPressed[BUTTON_A]);
  sprites.drawSelfMasked(97, 53, allButtons, 2 * BUTTON_B + buttonPressed[BUTTON_B]);
  if (currentDice > 1 && currentDice < 8) sprites.drawSelfMasked(66, 48, allButtons, 2 * BUTTON_UP + buttonPressed[BUTTON_UP]);
  if (currentDice > 1 && currentDice < 8) sprites.drawSelfMasked(66, 55, allButtons, 2 * BUTTON_DOWN + buttonPressed[BUTTON_DOWN]);
  sprites.drawSelfMasked(12, 56, allWords, 0);
  if (currentDice > 1) sprites.drawSelfMasked(108, 56, allWords, 1);
  else sprites.drawSelfMasked(108, 56, allWords, 3);
  if (showDiceName)sprites.drawSelfMasked(55, 40, diceName, currentDice - 1);
}

void stateDiceRolling()
{
  diceResult = 0;
  coinSlowDown = 0;
  if (arduboy.everyXFrames(8))
  {
    frameDice++;
    sound.tone(880, 20);
  }
  if (arduboy.everyXFrames(3))
  {
    frameCoin++;
    sound.tone(880, 20);
  }
  if (frameDice > 2) frameDice = 0;
  if (frameCoin > 7) frameCoin = 0;
  if (rollingDice[0].type > 0)
  {
    for (byte i = 0; i < AMOUNT_OF_ROLLING_DICE; i++)
    {
      if (rollingDice[0].type < 7) rollingDice[i].result = random (1, diceMax[currentDice - 1]);
      else rollingDice[i].result = random (0, diceMax[currentDice - 1]);
    }
    sprites.drawSelfMasked(rollingDice[0].x, rollingDice[0].y, allDice, (3 * (rollingDice[0].type -1)) + frameDice);
  }
  else
  {
    for (byte i = 0; i < AMOUNT_OF_ROLLING_DICE; i++)
    {
      rollingDice[i].result = random (0, diceMax[currentDice - 1]);
    }
    sprites.drawSelfMasked(rollingDice[0].x, rollingDice[0].y, justCoin, frameCoin);
  }
  if (arduboy.justPressed(A_BUTTON))
  {
    buttonPressed[BUTTON_A] = true;
    gameState = STATE_DICE_TYPE_AND_AMOUNT;
    placeDice();
    sound.tone(300, 20);
  }
  if (arduboy.justPressed(B_BUTTON))
  {
    buttonPressed[BUTTON_B] = true;
    frameBand = 0;
    showResult = false;
    for (byte i = 0; i < amountOfDice; i++)
    {
      diceResult = diceResult + rollingDice[i].result;
    }
    gameState = STATE_DICE_RESULT;
    sound.tone(300, 20);
  }

  sprites.drawSelfMasked(1, 53, allButtons, 2 * BUTTON_A + buttonPressed[BUTTON_A]);
  sprites.drawSelfMasked(97, 53, allButtons, 2 * BUTTON_B + buttonPressed[BUTTON_B]);
  sprites.drawSelfMasked(12, 56, allWords, 0);
  sprites.drawSelfMasked(108, 56, allWords, 2);
}

void stateDiceResult()
{
  if (arduboy.justPressed(A_BUTTON))
  {
    buttonPressed[BUTTON_A] = true;
    gameState = STATE_DICE_TYPE_AND_AMOUNT;
    placeDice();
    sound.tone(300, 20);
  }
  if (arduboy.justPressed(B_BUTTON))
  {
    buttonPressed[BUTTON_B] = true;
    gameState = STATE_DICE_ROLLING;
    sound.tone(300, 20);
  }
  if (rollingDice[0].type > 0)
  {
    drawBorders();
    drawBands();
    if (showResult) drawResult();

    if (arduboy.everyXFrames(2)) frameBand++;
    if (frameBand == 4) sound.tone(880, 20);
    if (frameBand == 6) sound.tone(1000, 20);
    if (frameBand > 7)
    {
      frameBand = 7;
      showResult = true;
    }
    if (amountOfDice > 1)
    {
      for (byte i = 0; i < amountOfDice; i++)
      {
        drawNumbers(77 - (13 * amountOfDice) + (26 * i), 8, rollingDice[i].result);
      }
      for (byte i = 0; i < (amountOfDice - 1); i++)
      {
        sprites.drawSelfMasked(88 - (13 * amountOfDice) + (26 * i), 12, justADot, 0);
      }
    }
  }
  else
  {
    if ((arduboy.everyXFrames(3 + coinSlowDown)) && (coinSlowDown < 3)) frameCoin++;
    if (arduboy.everyXFrames(6) && coinSlowDown > 2)
    {
      if ((!rollingDice[0].result) && (frameCoin != 4)) frameCoin++;
      if ((rollingDice[0].result) && (frameCoin != 0)) frameCoin++;
    }
    if (frameCoin > 7)
    {
      coinSlowDown++;
      frameCoin = 0;
    }
    sprites.drawSelfMasked(rollingDice[0].x, rollingDice[0].y, justCoin, frameCoin);
  }
  sprites.drawSelfMasked(1, 53, allButtons, 2 * BUTTON_A + buttonPressed[BUTTON_A]);
  sprites.drawSelfMasked(97, 53, allButtons, 2 * BUTTON_B + buttonPressed[BUTTON_B]);
  sprites.drawSelfMasked(12, 56, allWords, 0);
  if (currentDice > 1) sprites.drawSelfMasked(108, 56, allWords, 1);
  else sprites.drawSelfMasked(108, 56, allWords, 3);
}



#endif
