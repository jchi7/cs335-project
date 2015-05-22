/* Jasjot Sumal
 * CMPS 335, Project, Lab 6 / Homework 3
 */

#ifndef JASJOTS_H  // UnitTests header
#define JASJOTS_H

#include "basicEnemy.h"
#include "collisions.h"
#include "game.h"
#include "gameObject.h"
#include "graphicComponent.h"
#include "hero.h"
#include "inputComponent.h"
#include "platform.h"
#include "room.h"
#include "spike.h"
#include "structs.h"
#include "vecFxns.h"

string printPassFail(bool score);
string printCollision(bool result);
void setStickyFalse(bool * flagToSet, bool condition);

bool testCollisions();
bool testThreeHalfSpace();
bool testPtInRect();
bool singleRunThreeHalfSpace(Shape * t, float x, float y, bool answ);
bool singleRunPtInRect(Shape * r, float x, float y, bool answ);

bool testVecFxns();
bool testVecDotProduct();
bool testVecSub();
bool testVecLength();
bool testVecDist();
bool testVecNormalize();
bool testVecPerpendicular();
bool testGetHighestX();
bool testGetLowestX();
bool testGetHighestY();
bool testGetLowestY();
bool singleRunVecDot(float x1, float y1, float x2, float y2, float expected);
bool singleRunVecSub(float x1, float y1, float x2, float y2, float answX, float answY);
bool singleRunVecLen(float x, float y, float answ);
bool singleRunVecDist(float x1, float y1, float x2, float y2, float answ);
bool singleRunVecNorm(float x1, float y1, float answX, float answY);
bool singleRunVecPpd(float x, float y, float answX, float answY);
bool singleRunGetHighestX(float x1, float y1, float x2, float y2, float x3, float y3, float answ);
bool singleRunGetLowestX(float x1, float y1, float x2, float y2, float x3, float y3, float answ);
bool singleRunGetHighestY(float x1, float y1, float x2, float y2, float x3, float y3, float answ);
bool singleRunGetLowestY(float x1, float y1, float x2, float y2, float x3, float y3, float answ);

bool testBasicEnemy();

#endif
