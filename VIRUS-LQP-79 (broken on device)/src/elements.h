#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "globals.h"
#include "bitmaps.h"

extern int mapPositionX;
extern int mapPositionY;

// constants /////////////////////////////////////////////////////////////////

#define SURVIVOR_FRAME_SKIP      10
#define SURVIVOR_FRAME_COUNT     4
#define SURVIVOR_MAX             5

#define SURVIVOR_WIDTH           16
#define SURVIVOR_HEIGHT          16

// structures ////////////////////////////////////////////////////////////////

struct Element
{
  public:
    int x;
    int y;
    byte active;
};


// globals ///////////////////////////////////////////////////////////////////

// list of survivors
extern Element survivors[SURVIVOR_MAX];


// method prototypes /////////////////////////////////////////////////////////

void swapSurvivorPool();
void updateSurvivors();
void drawSurvivors();
bool survivorCollision(Element& obj, int x, int y, char w, char h);
bool collectSurvivor(Element& obj);
void clearSurvivors();
byte countAmountActiveSurvivors();
void drawAmountSurvivors();
void survivorCollide(int x, int y);

#endif
