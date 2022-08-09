#ifndef INPUTS_H
#define INPUTS_H

#include "globals.h"
#include "player.h"

void checkInputs()
{
  // Update launcher
  if (arduboy.pressed(LEFT_BUTTON)) {
    launcherAngle = min(++launcherAngle, 160);
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    launcherAngle = max(--launcherAngle, 20);
  }
  radAngle = (float)launcherAngle * PI / 180.00;

  // Launch Ball
  if (arduboy.justPressed(B_BUTTON) && aBall == 255 && !falling && ballQueue[0] != DEAD_BALL) {
    aBall = ballQueue[0];
    shiftBallQueue(true);
    aBallX = LAUNCHER_X;
    aBallY = LAUNCHER_Y;
    aBallRad = radAngle;
  }

  // DEBUG
  /*if (arduboy.justPressed(A_BUTTON)) {
    shiftBallsDown(true);
  }*/
}


#endif
