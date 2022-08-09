#ifndef LEVELS_H
#define LEVELS_H

#include "globals.h"
#include "player.h"

void rootExpand(byte index);

void checkCollisions()
{

}

/*
   checkDeath
   takes: nothing
   returns: bool - true if a ball was killed
                   false if no ball was killed
    this method checks every ball for whether or not it is touching 3 balls.
    a better approach may be to only check the balls that the last launched ball
    ends up touching to allow levels to start out with 3 or more balls touching already.
*/
/*bool checkDeath() {
  for (byte i = TOTAL_BALLS-1; i < TOTAL_BALLS; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT)) {
      byte type = getBallType(i);
      byte row = i / 11;
      byte col = i % 11;
      byte connections = 0;
      byte colOffset = 1;
      if (row % 2 == alignType)
        colOffset = 0;
      // Top left
      byte nball = getBall(row-1, col - colOffset);
      if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
        connections++;
      // Top right
      nball = getBall(row-1, col + 1 - colOffset);
      if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
        connections++;
      // Left
      nball = getBall(row, col - 1);
      if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
        connections++;
      // Right
      nball = getBall(row, col + 1);
      if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
        connections++;
      // Bottom right
      nball = getBall(row + 1, col + 1 - colOffset);
      if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
        connections++;
      // Bottom left
      nball = getBall(row + 1, col - colOffset);
      if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
        connections++;

      //Serial.print("Ball: "); Serial.print(i); Serial.print(" connections: "); Serial.println(connections);

      if (connections >= 2) {
        killBall(i);
        return true;
      }
    }
  }
  return false;
  }*/

/*
   checkSurrounding
   takes: index of ball to check (the ball that just landed)
   returns: whether ball death occurred
   checks death for the surrounding balls of the same type.
*/
byte checkSurrounding(byte index) {
  if (checkDeath(index))
    return true;

  byte type = getBallType(index);
  byte row = index / TOTAL_COLUMNS;
  byte col = index % TOTAL_COLUMNS;
  byte connections = 0;
  byte colOffset = 1;
  byte ret = 0;
  if (row % 2 == alignType)
    colOffset = 0;

  // Top left
  byte nball = getBall(row - 1, col - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    ret |= checkDeath(nball);
  // Top right
  nball = getBall(row - 1, col + 1 - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    ret |= checkDeath(nball);
  // Left
  nball = getBall(row, col - 1);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    ret |= checkDeath(nball);
  // Right
  nball = getBall(row, col + 1);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    ret |= checkDeath(nball);
  // Bottom right
  nball = getBall(row + 1, col + 1 - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    ret |= checkDeath(nball);
  // Bottom left
  nball = getBall(row + 1, col - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    ret |= checkDeath(nball);

  return ret;
}

/*
   checkDeath
   takes: index of ball
   returns: bool - true if a ball was killed
                   false if no ball was killed
    this method checks a single ball for surrounding connections being greater than or equal to 2
    and kills the ball if true.
*/
byte checkDeath(byte index) {
  if (bitRead(balls[index].state, ACTIVE_BIT)) {
    byte type = getBallType(index);
    byte row = index / TOTAL_COLUMNS;
    byte col = index % TOTAL_COLUMNS;
    byte connections = 0;
    byte colOffset = 1;
    if (row % 2 == alignType)
      colOffset = 0;
    // Top left
    byte nball = getBall(row - 1, col - colOffset);
    if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
      connections++;
    // Top right
    nball = getBall(row - 1, col + 1 - colOffset);
    if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
      connections++;
    // Left
    nball = getBall(row, col - 1);
    if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
      connections++;
    // Right
    nball = getBall(row, col + 1);
    if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
      connections++;
    // Bottom right
    nball = getBall(row + 1, col + 1 - colOffset);
    if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
      connections++;
    // Bottom left
    nball = getBall(row + 1, col - colOffset);
    if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
      connections++;

    //Serial.print("Ball: "); Serial.print(i); Serial.print(" connections: "); Serial.println(connections);

    if (connections >= 2) {
      killBall(index);
      return 1;
    }
  }
  return 0;
}

/*
   killBall
   takes: the index of the ball to kill
   returns: nothing
   this function kills the indexed ball and any ball touching it of
   the same type.
*/
void killBall(byte index) {
  byte type = getBallType(index);
  setBallType(index, DEAD_BALL); // dead black ball
  byte col = index % TOTAL_COLUMNS;
  byte row = index / TOTAL_COLUMNS;
  byte colOffset = 1;
  if (row % 2 == alignType)
    colOffset = 0;

  // Top left
  byte nball = getBall(row - 1, col - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    killBall(nball);
  // Top right
  nball = getBall(row - 1, col + 1 - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    killBall(nball);
  // Left
  nball = getBall(row, col - 1);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    killBall(nball);
  // Right
  nball = getBall(row, col + 1);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    killBall(nball);
  // Bottom right
  nball = getBall(row + 1, col + 1 - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    killBall(nball);
  // Bottom left
  nball = getBall(row + 1, col - colOffset);
  if (nball != 255 && getBallType(nball) == type && bitRead(balls[nball].state, ACTIVE_BIT))
    killBall(nball);
}

/*
   deactivateDead
   takes: nothing
   returns: nothing
   checks every ball for whether or not it is dead.
   if a ball is found to be dead, it will be deactivated.
*/
void deactivateDead() {
  for (byte i = TOTAL_BALLS - 1; i < TOTAL_BALLS; i--) {
    if (getBallType(i) == DEAD_BALL) {
      bitClear(balls[i].state, ACTIVE_BIT);
    }
  }
}

/*
   checkRoots
   takes: nothing
   returns: nothing
   begins by setting every balls root to false (not rooted). !!! THIS SHOULD PROBABLY BE CHANGED !!!
   then sets the top balls to be rooted, and recursively roots all connected balls that are
   not already rooted.
   finally it kills all floating (non-rooted) balls.

 * *note: Ideally, the approach will be changed so a variable will track the top row as it moves down (not implemented yet).
   this way they don't all day once it is shifted. This will not apply to an infinite mode that fills the top row in randomly
   as it is shifted.
*/
void checkRoots() {
  for (byte i = TOTAL_BALLS - 1; i < TOTAL_BALLS; i--) {
    bitClear(balls[i].state, ROOT_BIT);
  }
  for (byte i = TOTAL_COLUMNS - 1; i < TOTAL_COLUMNS; i--) {
    if (bitRead(balls[i].state, ACTIVE_BIT) && getBallType(i) != 6) {
      byte col = i % TOTAL_COLUMNS;
      byte row = i / TOTAL_COLUMNS;
      byte colOffset = 1;
      if (row % 2 == alignType)
        colOffset = 0;
      bitSet(balls[i].state, ROOT_BIT);
      // Bottom right
      byte nball = getBall(row + 1, col + 1 - colOffset);
      if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != 6)
        rootExpand(nball);
      // Bottom left
      nball = getBall(row + 1, col - colOffset);
      if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != 6)
        rootExpand(nball);
    }
  }

  // Kill non rooted
  for (byte i = TOTAL_BALLS - 1; i < TOTAL_BALLS; i--) {
    if (!bitRead(balls[i].state, ROOT_BIT)) {
      setBallType(i, 6);
    }
  }
}

/*
   rootExpand
   takes: index of the ball to set as rooted
   returns: nothing
   this is a recursive function that checks surrounding balls for whether or not they are active,
   not already rooted, and not already dead, then sets them as rooted.
   think of the roots as growing from the top to the bottom just like the roots of a tree.
*/
void rootExpand(byte index) {
  bitSet(balls[index].state, ROOT_BIT);
  byte col = index % TOTAL_COLUMNS;
  byte row = index / TOTAL_COLUMNS;
  byte colOffset = 1;
  if (row % 2 == alignType)
    colOffset = 0;

  // Top left
  byte nball = getBall(row - 1, col - colOffset);
  if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != DEAD_BALL)
    rootExpand(nball);
  // Top right
  nball = getBall(row - 1, col + 1 - colOffset);
  if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != DEAD_BALL)
    rootExpand(nball);
  // Left
  nball = getBall(row, col - 1);
  if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != DEAD_BALL)
    rootExpand(nball);
  // Right
  nball = getBall(row, col + 1);
  if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != DEAD_BALL)
    rootExpand(nball);
  // Bottom right
  nball = getBall(row + 1, col + 1 - colOffset);
  if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != DEAD_BALL)
    rootExpand(nball);
  // Bottom left
  nball = getBall(row + 1, col - colOffset);
  if (nball != 255 && bitRead(balls[nball].state, ACTIVE_BIT) && !bitRead(balls[nball].state, ROOT_BIT) && getBallType(nball) != DEAD_BALL)
    rootExpand(nball);
}

/*
   drawBackground
   takes: nothing
   returns: nothing
   draws all the background elements of the board.
*/
void drawBackground() {
  
  for (byte i = 0; i < 8; i++)
  {
    sprites.drawPlusMask (GAME_BORDER_LEFT, GAME_TOP + (i * 8), playFieldBorder_plus_mask, 0);
    sprites.drawPlusMask (GAME_RIGHT, GAME_TOP + (i * 8), playFieldBorder_plus_mask, 0);
    for (byte k = 0; k < 7 ;k++)
    {
      sprites.drawErase (GAME_BORDER_RIGHT+(k*8), i*8,playFieldMask_mask, 0);
    }
  }

  sprites.drawSelfMasked(80, 3, textScore, 0);
  /*
    for (byte i = GAME_LEFT - 1; i < GAME_LEFT; i--) {
    for (byte j = 6; j < 7; j--) {
      sprites.drawOverwrite(i, j * 9 + i / 9, sprWallPattern, 0);
      sprites.drawOverwrite(127 - i, j * 9 + i / 9, sprWallPattern, 0);
    }
    }

    Score things should be changed.
    for (byte i = 23; i < 24; i--) {
    sprites.drawOverwrite(101 + i, 14, sprScoreBottom, 0);
    }

    sprites.drawErase(100, 14, sprVerticalWall, 0);
    sprites.drawErase(126, 14, sprVerticalWall, 0);
    sprites.drawOverwrite(100, 3, sprScore, 0);


    for (byte i = 7; i < 8; i--) {
    sprites.drawErase(GAME_LEFT, i * 8, sprVerticalWall, 0);
    sprites.drawErase(GAME_RIGHT, i * 8, sprVerticalWall, 0);
    }

    for (byte i = (GAME_RIGHT - GAME_LEFT) / 2 - 1; i < (GAME_RIGHT - GAME_LEFT) / 2; i--) {
    sprites.drawOverwrite(GAME_LEFT + 1 + i * 2, 53, sprBottomWall, 0);
    }
    for (byte i = (GAME_RIGHT - LAUNCHER_X + 6) / 3 - 1; i < (GAME_RIGHT - 70) / 3; i--) {
    sprites.drawOverwrite(LAUNCHER_X + 6 + i * 3, 58, sprBottomWallPattern, 0);
    }
  */
}

#endif
