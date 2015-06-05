#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "gameObject.h"

extern bool ptInRect(Shape * r, Vec pt);
extern bool threeHalfSpace(Shape * t, Vec pt);
extern bool collisionRectRect(Shape * r1, Shape * r2);
extern bool collisionRectTri(Shape * r, Shape * t);
extern bool topOnlyCollisionRectRect(Shape * top, Shape * bottom, Vec topPrev);
extern bool lineSegCollision (Vec segAB[2], Vec segCD[2]);
// extern bool collision(Rectangle *, Circle *);
// extern bool collision(Triangle *, Triangle *);
// extern bool collision(Triangle *, Circle *);
// extern bool collision(Triangle *, Rectangle *);
// extern bool collision(Circle *, Circle *);
// extern bool collision(Circle *, Rectangle *);
// extern bool collision(Circle *, Triangle *);

#endif // COLLISIONS_H
