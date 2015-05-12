#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "gameObject.h"

// extern bool lineSegIntersect(Vec *, Vec *, Vec);
extern bool ptInRect(Shape * r, Vec pt);
extern bool threeHalfSpace(Shape * t, Vec pt);
extern bool collisionRectRect(Shape * r1, Shape * r2);
extern bool collisionRectTri(Shape * r, Shape * t);
// extern bool collision(Rectangle *, Circle *);
// extern bool collision(Triangle *, Triangle *);
// extern bool collision(Triangle *, Circle *);
// extern bool collision(Triangle *, Rectangle *);
// extern bool collision(Circle *, Circle *);
// extern bool collision(Circle *, Rectangle *);
// extern bool collision(Circle *, Triangle *);

#endif // COLLISIONS_H
