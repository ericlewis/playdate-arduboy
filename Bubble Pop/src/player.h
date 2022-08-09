#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

extern byte checkDeath(byte index);
extern void killBall(byte index);
extern void deactivateDead();
extern void checkRoots();
extern byte checkSurrounding(byte index);

class Ball {
public:
  byte state; // state of the ball position
  // 0000 0000
  // |||| |||└-\
  // |||| ||└--\ Type of ball, the sub-image. (6 is dead/falling)
  // |||| |└---/
  // |||| └----/
  // |||└------- Active
  // ||└-------- Connect to root during root expansion
  // |└--------- Unused
  // └---------- Unused
};

byte launcherAngle = 90;
Ball balls[TOTAL_BALLS];

void setBallType(byte ballIndex, byte type) {
  if (type > 6)
    type = 0;
  balls[ballIndex].state &= 0xF0;
  balls[ballIndex].state |= (type & 0x0F);
}

byte getBallType(byte ballIndex) {
  return (balls[ballIndex].state & 0x0F);
}

byte getBallIndex(byte x, byte y) {
  if (x < GAME_LEFT + 3 || x >= GAME_RIGHT - 4)
    return 255;
  byte row = (y) / 5; // 5 is the vertical separation of balls
  byte col;
  if (row % 2 == alignType) {
    col = (x - (GAME_LEFT + 3)) / 6; // 6 is the horizontal separation of balls
    if (col >= TOTAL_COLUMNS - 1)
      return 255;
  }
  else
    col = (x - (GAME_LEFT + 3)) / 6;
  if (row >= 11 || col >= TOTAL_COLUMNS)
    return 255;
  return (col + row * TOTAL_COLUMNS);
}

byte getBall(byte row, byte col) {
  if (row >= 11 || col >= TOTAL_COLUMNS)
    return 255;

  if (row % 2 == alignType && col >= TOTAL_COLUMNS - 1)
    return 255;

  return (col + row * TOTAL_COLUMNS);
}

void fallingBalls() {
  if (falling) {
    if (fallOffset < 64) {
      fallOffset += 2;
    }
    else {
      falling = false;
      fallOffset = 0;
      deactivateDead();
    }
  }
}

void updateMovingBall() {
  if (aBall != 255) {
    if (aBallX <= GAME_LEFT + 5 || aBallX >= GAME_RIGHT - 4) {
      aBallRad = PI - aBallRad; // reflect ball
    }
    aBallX += cos(aBallRad) * aBallSpeed;
    aBallY -= sin(aBallRad) * aBallSpeed;
    //byte forwardIndex = getBallIndex((byte)(aBallX + cos(aBallRad) * 1.0), (byte)(aBallY - sin(aBallRad) * 1.0));
    float abr = aBallRad;
    if (aBallX <= GAME_LEFT + 5 || aBallX >= GAME_RIGHT - 4) {
      abr = PI - abr; // reflect ball
    }
    byte forwardIndex[4];
    forwardIndex[3] = getBallIndex((byte)(aBallX  + 1), (byte)(aBallY + 0));
    forwardIndex[2] = getBallIndex((byte)(aBallX  - 1), (byte)(aBallY + 0));
    forwardIndex[1] = getBallIndex((byte)(aBallX  + 0), (byte)(aBallY - 1));
    forwardIndex[0] = getBallIndex((byte)(aBallX  + 0), (byte)(aBallY + 1));
    byte fd = 255;
    for (byte i = 3; i < 4; i--) {
      if (forwardIndex[i] != 255 && bitRead(balls[forwardIndex[i]].state, ACTIVE_BIT))
        fd &= forwardIndex[i];
    }
    if (fd != 255) {
      byte indx = getBallIndex((byte)aBallX, (byte)aBallY);
      setBallType(indx, aBall);
      bitSet(balls[indx].state, ACTIVE_BIT);
      bitClear(balls[indx].state, ROOT_BIT);
      aBall = 255;
      if (checkSurrounding(indx)) {
        falling = true;
        checkRoots();
      }
    }

    if (aBallY <= 3) {
      byte indx = getBallIndex((byte)aBallX, (byte)aBallY);
      if (indx != 255) {
        setBallType(indx, aBall);
        bitSet(balls[indx].state, ACTIVE_BIT);
        bitClear(balls[indx].state, ROOT_BIT);
      }
      aBall = 255;
      if (checkSurrounding(indx)) {
        falling = true;
        checkRoots();
      }
    }
  }
}

void drawBalls() {
  for (byte i = TOTAL_BALLS-1; i < TOTAL_BALLS; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT)) {
      byte col = i % TOTAL_COLUMNS;
      byte row = i / TOTAL_COLUMNS;
      sprites.drawPlusMask(GAME_LEFT + 2 + col * 6 + ((row % 2 == alignType) ? 3 : 0), 5 * row + ((getBallType(i) == 6) ? fallOffset : 0), ballMask_plus_mask, 0);
      sprites.drawErase(GAME_LEFT + 3 + col * 6 + ((row % 2 == alignType) ? 3 : 0), 5 * row + 1 + ((getBallType(i) == 6) ? fallOffset : 0), sprBalls, getBallType(i));
    }
  }
}

void fillBallQueue() {
  for (byte i = 0; i < 6; i++) {
    ballQueue[i] = generateRandomNumber(6);
  }
}

void shiftBallQueue(bool newball) {
  for (byte i = 0; i < 5; i++) {
    ballQueue[i] = ballQueue[i + 1];
  }
  if (newball)
    ballQueue[5] = generateRandomNumber(6);
  else
    ballQueue[5] = 255;
}

void drawBallQueue() {
  for (byte i = 1; i < 6; i++) {
    if (ballQueue[i] != 255)
      sprites.drawPlusMask(LAUNCHER_X - 7 - (i - 1) * 6, 58, ballMask_plus_mask, 0);
      sprites.drawErase(LAUNCHER_X - 6 - (i - 1) * 6, 59, sprBalls, ballQueue[i]);
  }
}

void shiftBallsDown(bool fillTopRow) {
  alignType ^= 1;
  for (byte i = 10; i > 0; i--) { // row
    for (byte j = TOTAL_COLUMNS - 1; j < TOTAL_COLUMNS; j--) { // column
      balls[j + i * TOTAL_COLUMNS] = balls[j + (i - 1) * TOTAL_COLUMNS];
    }
  }
  if (fillTopRow) {
    for (byte i = TOTAL_COLUMNS-1; i < TOTAL_COLUMNS; i--) {
      balls[i].state = generateRandomNumber(6);
      bitSet(balls[i].state, ACTIVE_BIT);
      bitSet(balls[i].state, ROOT_BIT);
    }
    if (!alignType)
      bitClear(balls[TOTAL_COLUMNS-1].state, ACTIVE_BIT); 
  }
  else {
    for (byte i = TOTAL_COLUMNS-1; i < TOTAL_COLUMNS; i--) {
      bitClear(balls[i].state, ACTIVE_BIT);
    }
  }
}

bool checkBottomRow() {
  for (byte i = TOTAL_BALLS-1; i >= TOTAL_BALLS-12; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT))
      return true;
  }
  return false;
}

void clearBalls() {
  for (byte i = TOTAL_BALLS-1; i < TOTAL_BALLS; i--) {
    balls[i].state = 0;
  }
}

#endif
