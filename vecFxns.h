/*
  Jasjot Sumal
  CMPS 371, Fall 2014
*/
#ifndef VECFXNS_H
#define VECFXNS_H

#include  <cmath>
#include  <string>
#include  <sstream>

using namespace std;

typedef float Vec[3];
typedef int iVec[3];

// ---- VECTOR FUNCTIONS ----
// float vecCrossProduct   (Vec, Vec);
float vecDotProduct     (Vec, Vec);
void  vecZero           (Vec);
float vecDist           (Vec, Vec);
void  vecNegate         (Vec);
void  vecMake           (float x, float y, Vec dest);
void  vecCopy           (Vec source, Vec dest);
float vecLength         (Vec);
void  vecNormalize      (Vec);
void  vecSub            (Vec vTo, Vec vFrom, Vec dest);
void  vecPerpendicular  (Vec source, Vec dest);
// void  getTriangleNormal (Vec tri[3], Vec norm);
// void  getNormal         (Vec v0, Vec v1, Vec v2, Vec norm);
float getHighestX       (Vec, Vec, Vec);
float getLowestX        (Vec, Vec, Vec);
float getHighestY       (Vec, Vec, Vec);
float getLowestY        (Vec, Vec, Vec);
string vecPrint         (Vec);

#endif
