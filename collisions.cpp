#include "collisions.h"

bool ptInRect(Shape * r, Vec pt)
{
    if (r->type != RECTANGLE) {
        std::cout << "bool ptInRect: ERROR, passed incompatable Shape type\n";
        exit(1);
    }

    if (r->center[0] - r->width <= pt[0] && // left bound
        r->center[0] + r->width >= pt[0] && // right bound
        r->center[1] - r->height <= pt[1] && // lower bound
        r->center[1] + r->height >= pt[1])   // upper bound
    {
        return true;
    }
    return false;
}

bool threeHalfSpace(Shape * t, Vec pt)
{
    if (t->type != TRIANGLE) {
        std::cout << "bool collisionRectTri: ERROR, passed incompatable Shape type\n";
        exit(1);
    }

    // use 3 half-space calculation
    Vec side, ppd, cornerToPt;
    int sum=0;

    for(int i = 0; i < 3; i++) {
        vecSub(t->corners[(i+1)%3], t->corners[i], side);
        vecPerpendicular(side, ppd);
        vecSub(pt, t->corners[i], cornerToPt);
        if(vecDotProduct(ppd, cornerToPt) >= 0) sum += 1;
        else sum -= 1;
    }

	//+3 or -3 indicates pt inside triangle
	return (std::abs(sum)==3);
}

bool collisionRectRect(Shape * r1, Shape * r2)
{
    if (r1->type != RECTANGLE && r2->type != RECTANGLE) { // shoud be OR
        std::cout << "bool collisionRectRect: ERROR, passed incompatable Shape types\n";
        exit(1);
    }

    if (r1->center[0] + r1->width >= r2->center[0] - r2->width &&
        r1->center[0] - r1->width <= r2->center[0] + r2->width &&
        r1->center[1] - r1->height < r2->center[1] + r2->height &&
        r1->center[1] + r1->height > r2->center[1] - r2->height )
    {
        return true;
    }
    return false;
}

bool collisionRectTri(Shape * r, Shape * t)
{
    if (r->type != RECTANGLE && t->type != TRIANGLE) {
        if (r->type == TRIANGLE && t->type == RECTANGLE) {
            Shape * temp;
            temp = r;
            r = t;
            t = temp;
        }
        else {
            std::cout << "bool collisionRectTri: ERROR, passed incompatable Shape types\n";
            exit(1);
        }
    }

    // store rectangle x & y
    float xRectLeft = r->center[0] - r->width;
    float xRectRight = r->center[0] + r->width;
    float yRectLow = r->center[1] - r->height;
    float yRectHigh = r->center[1] + r->height;

    // store bounding rectangle of triangle
    float xTriLeft = getLowestX(t->corners[0], t->corners[1], t->corners[2]);
    float xTriRight = getHighestX(t->corners[0], t->corners[1], t->corners[2]); 
    float yTriHigh = getHighestY(t->corners[0], t->corners[1], t->corners[2]);
    float yTriLow = getLowestY(t->corners[0], t->corners[1], t->corners[2]);

    // check if triangle's bounding rectangle not colliding
    if (xRectRight < xTriLeft ||
        xRectLeft > xTriRight ||
        yRectHigh < yTriLow ||
        yRectLow > yTriHigh)
    {
        return false;
    }

    // check if any of the triangle's points are within the rectangle
    if (ptInRect(r, t->corners[0]) ||
        ptInRect(r, t->corners[1]) ||
        ptInRect(r, t->corners[2]))
    {
        return true;
    }

    // check if any of the rectangle's points are within the triangle
    Vec rLowerLeft, rUpperLeft, rLowerRight, rUpperRight;
    vecMake(xRectLeft, yRectLow, rLowerLeft);
    vecMake(xRectLeft, yRectHigh, rUpperLeft);
    vecMake(xRectRight, yRectLow, rLowerRight);
    vecMake(xRectRight, yRectHigh, rUpperRight);
    if (threeHalfSpace(t, rLowerLeft) || 
        threeHalfSpace(t, rUpperLeft) ||
        threeHalfSpace(t, rLowerRight) ||
        threeHalfSpace(t, rUpperRight))
    {
        return true;
    }
    return false;
}

extern bool topOnlyCollisionRectRect(Shape * top, Shape * bottom, Vec topPrev)
{
    if (top->type != RECTANGLE || bottom->type != RECTANGLE) {
        std::cout << "bool topOnlyCollisionRectRect: ERROR, passed incompatable Shape types\n";
        exit(1);
    }

    if (top->center[1]-top->height < bottom->center[1]+bottom->height) {
      return false;
    }

    bool topLeftCollision=false, topRightCollision=false;
    Vec bottomUpperSegment[2];//, bottomRightSegment[2];
    Vec topRightSegment[2], topLeftSegment[2];
    vecMake(
      bottom->center[0]-bottom->width,
      bottom->center[1]+bottom->height,
      bottomUpperSegment[0]);
    vecMake(
      bottom->center[0]+bottom->width,
      bottom->center[1]+bottom->height,
      bottomUpperSegment[1]);
    vecMake(
      top->center[0]-top->width,
      top->center[1]-top->height,
      topRightSegment[0]);
    vecMake(
      top->center[0]-top->width,
      top->center[1]+top->height,
      topRightSegment[1]);
    vecMake(
      top->center[0]+top->width,
      top->center[1]-top->height,
      topLeftSegment[0]);
    vecMake(
      top->center[0]+top->width,
      top->center[1]+top->height,
      topLeftSegment[1]);

    if (lineSegCollision(bottomUpperSegment, topLeftSegment)) {
      topLeftCollision = true;
    }
    if (lineSegCollision(bottomUpperSegment, topRightSegment)) {
      topRightCollision = true;
    }
    if (!topLeftCollision && !topRightCollision) {
      return false;
    }
    if (topLeftCollision && topRightCollision) {
      return true;
    }

    if(topLeftCollision) {
        if (topPrev[0]-top->width > bottom->center[0]+bottom->width) {
            return false;
        }
        return true;
    }
    else { // topRightCollision
        if (topPrev[0]+top->width > bottom->center[0]-bottom->width) {
            return false;
        }
        return true;
    }
}

extern bool lineSegCollision(Vec segAB[2], Vec segCD[2])
{
  float eps = 0.001;
  float signC, signD;

  signC = ((segAB[1][0]-segAB[0][0])*(segCD[0][1]-segAB[1][1])) -
    ((segAB[1][1]-segAB[0][1])*(segCD[0][0]-segAB[1][0]));

  signD = ((segAB[1][0]-segAB[0][0])*(segCD[1][1]-segAB[1][1])) -
    ((segAB[1][1]-segAB[0][1])*(segCD[1][0]-segAB[1][0]));

  if (fabs(signC) < eps || fabs(signD) < eps) {
    return true;
  }
  if ((signC >= eps && signD >= eps) ||
    (signC <= -eps && signD <= -eps))
  {
    return true;
  }
  return false;
}

// bool collision(Rectangle * r, Circle * c)
// {
// 
// }
//
// bool collision(Triangle * t1, Triangle * t2)
// {
// 
// }
// 
// bool collision(Triangle * t, Circle * c)
// {
// 
// }
// 
// bool collision(Triangle * t, Rectangle * r)
// {
// 
// }
// 
// bool collision(Circle * c1, Circle * c2)
// {
// 
// }
// 
// bool collision(Circle * c, Rectangle * r)
// {
// 
// }
// 
// bool collision(Circle * c, Triangle * t)
// {
// 
// }



