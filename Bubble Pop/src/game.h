#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"


#define RADIUS_AMOUNT       6
#define SQUARE_SIZE         8
#define RADIUS              (SQUARE_SIZE * RADIUS_AMOUNT)
#define MAGNITUDE           0.01

#define LINE_LENGHT         (16 + (2*RADIUS_AMOUNT))
#define LINE_SKIP           (LINE_LENGHT/2)
#define LINE_DOUBLE         (4 + RADIUS_AMOUNT)
#define LINE_SUBSTRACT      ((LINE_SKIP * 16) - 8)

const unsigned char PROGMEM textXY[][2] = {{1, 0}, {2, 0}, {3, 1}, {3, 2}, {2, 3}, {1, 3}, {0, 2}, {0, 1}};

//const unsigned char PROGMEM circlePoints[][2] = {{96, 48}, {95, 47}, {95, 47}, {95, 46}, {95, 46}, {95, 45}, {95, 45}, {95, 44}, {95, 44}, {95, 43}, {95, 43}, {95, 42}, {95, 42}, {95, 41}, {95, 41}, {95, 40}, {95, 40}, {95, 39}, {95, 39}, {95, 38}, {95, 38}, {94, 37}, {94, 37}, {94, 37}, {94, 36}, {94, 36}, {94, 35}, {94, 35}, {94, 34}, {93, 34}, {93, 33}, {93, 33}, {93, 32}, {93, 32}, {93, 31}, {93, 31}, {92, 31}, {92, 30}, {92, 30}, {92, 29}, {92, 29}, {92, 28}, {91, 28}, {91, 27}, {91, 27}, {91, 27}, {91, 26}, {90, 26}, {90, 25}, {90, 25}, {90, 24}, {89, 24}, {89, 24}, {89, 23}, {89, 23}, {88, 22}, {88, 22}, {88, 22}, {88, 21}, {87, 21}, {87, 20}, {87, 20}, {87, 20}, {86, 19}, {86, 19}, {86, 18}, {85, 18}, {85, 18}, {85, 17}, {85, 17}, {84, 17}, {84, 16}, {84, 16}, {83, 15}, {83, 15}, {83, 15}, {82, 14}, {82, 14}, {82, 14}, {81, 13}, {81, 13}, {81, 13}, {80, 12}, {80, 12}, {80, 12}, {79, 11}, {79, 11}, {78, 11}, {78, 11}, {78, 10}, {77, 10}, {77, 10}, {77, 9}, {76, 9}, {76, 9}, {75, 8}, {75, 8}, {75, 8}, {74, 8}, {74, 7}, {73, 7}, {73, 7}, {73, 7}, {72, 6}, {72, 6}, {71, 6}, {71, 6}, {71, 5}, {70, 5}, {70, 5}, {69, 5}, {69, 5}, {68, 4}, {68, 4}, {68, 4}, {67, 4}, {67, 3}, {66, 3}, {66, 3}, {65, 3}, {65, 3}, {64, 3}, {64, 2}, {64, 2}, {63, 2}, {63, 2}, {62, 2}, {62, 2}, {61, 2}, {61, 1}, {60, 1}, {60, 1}, {59, 1}, {59, 1}, {58, 1}, {58, 1}, {58, 1}, {57, 0}, {57, 0}, {56, 0}, {56, 0}, {55, 0}, {55, 0}, {54, 0}, {54, 0}, {53, 0}, {53, 0}, {52, 0}, {52, 0}, {51, 0}, {51, 0}, {50, 0}, {50, 0}, {49, 0}, {49, 0}, {48, 0}, {48, 0}, {48, 0}, {47, 0}, {47, 0}, {46, 0}, {46, 0}, {45, 0}, {45, 0}, {44, 0}, {44, 0}, {43, 0}, {43, 0}, {42, 0}, {42, 0}, {41, 0}, {41, 0}, {40, 0}, {40, 0}, {39, 0}, {39, 0}, {38, 0}, {38, 0}, {38, 1}, {37, 1}, {37, 1}, {36, 1}, {36, 1}, {35, 1}, {35, 1}, {34, 1}, {34, 1}, {33, 2}, {33, 2}, {32, 2}, {32, 2}, {32, 2}, {31, 2}, {31, 3}, {30, 3}, {30, 3}, {29, 3}, {29, 3}, {28, 3}, {28, 4}, {28, 4}, {27, 4}, {27, 4}, {26, 4}, {26, 5}, {25, 5}, {25, 5}, {25, 5}, {24, 6}, {24, 6}, {23, 6}, {23, 6}, {22, 7}, {22, 7}, {22, 7}, {21, 7}, {21, 8}, {20, 8}, {20, 8}, {20, 8}, {19, 9}, {19, 9}, {18, 9}, {18, 10}, {18, 10}, {17, 10}, {17, 10}, {17, 11}, {16, 11}, {16, 11}, {16, 12}, {15, 12}, {15, 12}, {14, 13}, {14, 13}, {14, 13}, {13, 14}, {13, 14}, {13, 14}, {12, 15}, {12, 15}, {12, 15}, {11, 16}, {11, 16}, {11, 17}, {11, 17}, {10, 17}, {10, 18}, {10, 18}, {9, 18}, {9, 19}, {9, 19}, {8, 20}, {8, 20}, {8, 20}, {8, 21}, {7, 21}, {7, 22}, {7, 22}, {7, 22}, {6, 23}, {6, 23}, {6, 24}, {6, 24}, {5, 24}, {5, 25}, {5, 25}, {5, 26}, {5, 26}, {4, 27}, {4, 27}, {4, 27}, {4, 28}, {4, 28}, {3, 29}, {3, 29}, {3, 30}, {3, 30}, {3, 31}, {2, 31}, {2, 31}, {2, 32}, {2, 32}, {2, 33}, {2, 33}, {2, 34}, {1, 34}, {1, 35}, {1, 35}, {1, 36}, {1, 36}, {1, 36}, {1, 37}, {1, 37}, {0, 38}, {0, 38}, {0, 39}, {0, 39}, {0, 40}, {0, 40}, {0, 41}, {0, 41}, {0, 42}, {0, 42}, {0, 43}, {0, 43}, {0, 44}, {0, 44}, {0, 45}, {0, 45}, {0, 46}, {0, 46}, {0, 46}, {0, 47}, {0, 47}, {0, 48}, {0, 48}, {0, 49}, {0, 49}, {0, 50}, {0, 50}, {0, 51}, {0, 51}, {0, 52}, {0, 52}, {0, 53}, {0, 53}, {0, 54}, {0, 54}, {0, 55}, {0, 55}, {0, 56}, {0, 56}, {0, 56}, {0, 57}, {1, 57}, {1, 58}, {1, 58}, {1, 59}, {1, 59}, {1, 60}, {1, 60}, {1, 61}, {1, 61}, {2, 62}, {2, 62}, {2, 63}, {2, 63}, {2, 63}, {2, 64}, {3, 64}, {3, 65}, {3, 65}, {3, 66}, {3, 66}, {3, 67}, {4, 67}, {4, 67}, {4, 68}, {4, 68}, {4, 69}, {5, 69}, {5, 70}, {5, 70}, {5, 70}, {6, 71}, {6, 71}, {6, 72}, {6, 72}, {7, 73}, {7, 73}, {7, 73}, {7, 74}, {8, 74}, {8, 75}, {8, 75}, {8, 75}, {9, 76}, {9, 76}, {9, 76}, {10, 77}, {10, 77}, {10, 78}, {10, 78}, {11, 78}, {11, 79}, {11, 79}, {12, 79}, {12, 80}, {12, 80}, {13, 81}, {13, 81}, {13, 81}, {14, 82}, {14, 82}, {14, 82}, {15, 83}, {15, 83}, {15, 83}, {16, 84}, {16, 84}, {16, 84}, {17, 84}, {17, 85}, {18, 85}, {18, 85}, {18, 86}, {19, 86}, {19, 86}, {20, 86}, {20, 87}, {20, 87}, {21, 87}, {21, 88}, {22, 88}, {22, 88}, {22, 88}, {23, 89}, {23, 89}, {24, 89}, {24, 89}, {24, 90}, {25, 90}, {25, 90}, {26, 90}, {26, 90}, {27, 91}, {27, 91}, {27, 91}, {28, 91}, {28, 91}, {29, 92}, {29, 92}, {30, 92}, {30, 92}, {30, 92}, {31, 93}, {31, 93}, {32, 93}, {32, 93}, {33, 93}, {33, 93}, {34, 93}, {34, 94}, {35, 94}, {35, 94}, {36, 94}, {36, 94}, {36, 94}, {37, 94}, {37, 94}, {38, 95}, {38, 95}, {39, 95}, {39, 95}, {40, 95}, {40, 95}, {41, 95}, {41, 95}, {42, 95}, {42, 95}, {43, 95}, {43, 95}, {44, 95}, {44, 95}, {45, 95}, {45, 95}, {45, 95}, {46, 95}, {46, 95}, {47, 95}, {47, 95}, {48, 95}, {48, 95}, {49, 95}, {49, 95}, {50, 95}, {50, 95}, {51, 95}, {51, 95}, {52, 95}, {52, 95}, {53, 95}, {53, 95}, {54, 95}, {54, 95}, {55, 95}, {55, 95}, {56, 95}, {56, 95}, {56, 95}, {57, 95}, {57, 94}, {58, 94}, {58, 94}, {59, 94}, {59, 94}, {60, 94}, {60, 94}, {61, 94}, {61, 94}, {62, 93}, {62, 93}, {62, 93}, {63, 93}, {63, 93}, {64, 93}, {64, 92}, {65, 92}, {65, 92}, {66, 92}, {66, 92}, {67, 92}, {67, 91}, {67, 91}, {68, 91}, {68, 91}, {69, 91}, {69, 90}, {70, 90}, {70, 90}, {70, 90}, {71, 89}, {71, 89}, {72, 89}, {72, 89}, {72, 88}, {73, 88}, {73, 88}, {74, 88}, {74, 87}, {75, 87}, {75, 87}, {75, 87}, {76, 86}, {76, 86}, {76, 86}, {77, 85}, {77, 85}, {78, 85}, {78, 85}, {78, 84}, {79, 84}, {79, 84}, {79, 83}, {80, 83}, {80, 83}, {80, 82}, {81, 82}, {81, 82}, {82, 81}, {82, 81}, {82, 81}, {83, 80}, {83, 80}, {83, 80}, {83, 79}, {84, 79}, {84, 79}, {84, 78}, {85, 78}, {85, 77}, {85, 77}, {86, 77}, {86, 76}, {86, 76}, {86, 76}, {87, 75}, {87, 75}, {87, 74}, {88, 74}, {88, 74}, {88, 73}, {88, 73}, {89, 72}, {89, 72}, {89, 71}, {89, 71}, {90, 71}, {90, 70}, {90, 70}, {90, 69}, {90, 69}, {91, 69}, {91, 68}, {91, 68}, {91, 67}, {91, 67}, {92, 66}, {92, 66}, {92, 65}, {92, 65}, {92, 65}, {93, 64}, {93, 64}, {93, 63}, {93, 63}, {93, 62}, {93, 62}, {93, 61}, {94, 61}, {94, 60}, {94, 60}, {94, 60}, {94, 59}, {94, 59}, {94, 58}, {94, 58}, {95, 57}, {95, 57}, {95, 56}, {95, 56}, {95, 55}, {95, 55}, {95, 54}, {95, 54}, {95, 53}, {95, 53}, {95, 52}, {95, 52}, {95, 51}, {95, 51}, {95, 51}, {95, 50}, {95, 50}, {95, 49}, {95, 49}, {95, 48}};


byte textSequence = 0;
byte backgroundSequence = 0;
//int circleSequence = 0;
float a = 0.0;
byte x = 96;
byte y = 48;

void drawBackgroundAnim()
{

  //byte x = pgm_read_byte(&circlePoints[circleSequence][0]);
  //byte y = pgm_read_byte(&circlePoints[circleSequence][1]);

  //if ((x > 94) && (y == 48)) Serial.println();


  if (arduboy.everyXFrames(2))
  {
    x = RADIUS + (RADIUS * sin(2*a));
    y = RADIUS + (RADIUS * sin(-a));
    /*
      Serial.print("{");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.print("}, ");
    */
    backgroundSequence = (++backgroundSequence) % 8;
    a += MAGNITUDE;
    //circleSequence = (++circleSequence) % 628;
  }

  for (byte j = 0; j < LINE_DOUBLE; j++)
  {
    byte k = 0;
    for (byte i = 0; i < LINE_LENGHT; i++)
    {
      if (i == LINE_SKIP) k++;
      sprites.drawPlusMask((i * 16) - (LINE_SUBSTRACT * k) - x, (j * 16) + (k * 8) - y, gridBackground_plus_mask, 0);
    }
  }
}

void stateMenuPlay()
{
  arduboy.fillScreen(WHITE);
  drawBackgroundAnim();
  if (arduboy.everyXFrames(4)) textSequence = (++textSequence) % 8;

  byte coordinateX = pgm_read_byte(&textXY[textSequence][0]);
  byte coordinateY = pgm_read_byte(&textXY[textSequence][1]);

  sprites.drawPlusMask(34 + (coordinateX * (modeSelect - 1)), 12 + (coordinateY * (modeSelect - 1)), textArcade_plus_mask, 0);
  sprites.drawPlusMask(26 + (coordinateX * modeSelect), 36 + (coordinateY * modeSelect), textInfinite_plus_mask, 0);

  if (arduboy.justPressed(UP_BUTTON)) modeSelect = false;
  if (arduboy.justPressed(DOWN_BUTTON)) modeSelect = true;
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_GAME_ARCADE + modeSelect;
};


void stateGameArcade()
{
  gameState = STATE_GAME_NEXT_LEVEL;
}

void stateGameInfinite()
{
  gameState = STATE_GAME_NEXT_LEVEL;
}

void stateGameNextLevel()
{
  clearBalls();
  fillBallQueue();
  shiftBallsDown(true);
  shiftBallsDown(true);
  dropTimer = TIMER_AMOUNT;
  level++;
  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
  // Shift balls down periodically
  if (--dropTimer == 0) {
    shiftBallsDown(true);
    dropTimer = TIMER_AMOUNT;
  }

  if (checkBottomRow())
    gameState = STATE_GAME_OVER;

  checkInputs();
  checkCollisions();

  fallingBalls();
  updateMovingBall();

  arduboy.fillScreen(WHITE);
  drawBackgroundAnim();
  drawBackground();

  // Draw guide
  if (aBall == 255)
    arduboy.drawLine(LAUNCHER_X, LAUNCHER_Y, LAUNCHER_X + cos(radAngle) * 20, LAUNCHER_Y - sin(radAngle) * 20, BLACK);

  // Draw Balls
  drawBalls();
  if (aBall != 255) {
    sprites.drawPlusMask(byte(aBallX) - 3, byte(aBallY) - 3, ballMask_plus_mask, 0);
    sprites.drawErase(byte(aBallX) - 2, byte(aBallY) - 2, sprBalls, aBall);
  }
  drawBallQueue();
  // Draw launcher
  sprites.drawPlusMask(LAUNCHER_X - 6, LAUNCHER_Y - 8, sprLauncher, (launcherAngle - 22) / 28);
  if (ballQueue[0] != 255)
    sprites.drawErase(LAUNCHER_X - 2, LAUNCHER_Y - 3, sprBalls, ballQueue[0]);
};

void stateGamePause()
{
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PLAYING;
};

void stateGameOver()
{
  // game over Screen
  // draw mode
  // draw text score
  // draw score
  if (arduboy.justPressed(B_BUTTON)) gameState = STATE_MENU_INTRO;;
};



#endif
