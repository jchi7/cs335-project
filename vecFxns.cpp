/*
  Jasjot Sumal
  CMPS 371, Fall 2014
*/
#include  "vecFxns.h"

// float vecCrossProduct (Vec v0, Vec v1)
// {
//  return v0[0]*v1[1] - v0[1]*v1[0];
// }

void vecZero (Vec v)
{
	v[0] = v[1] = v[2] = 0.0;
}

void vecMake (float x, float y, Vec dest)
{
	dest[0] = x;
	dest[1] = y;
  dest[2] = 0.0;
}

void vecCopy (Vec source, Vec dest)
{
	dest[0] = source[0];
	dest[1] = source[1];
	dest[2] = 0.0;
}

bool vecEqual (Vec v0, Vec v1)
{
  float eps = 0.01;
  if (
    (abs(v0[0]-v1[1]) < eps) &&
    (abs(v0[1]-v1[1]) < eps))
  {
    return true;
  }
  return false;
}

bool vecSameDirec (Vec v0, Vec v1)
{
  float eps = 0.001;
  if (vecEqual(v0,v1)) return true;
  Vec checkZero;
  vecZero(checkZero);
  if (vecEqual(v0,checkZero) || vecEqual(v1,checkZero)) {
    return false;
  }

  bool signV0x = v0[0] >= 0;
  bool signV0y = v0[1] >= 0;
  bool signV1x = v1[0] >= 0;
  bool signV1y = v1[1] >= 0;
  if (signV0x == signV1x && signV0y == signV1y) {
    if (fabs(v0[0]/v1[0] - v0[1]/v1[1]) < eps) {
      return true;
    }
  }

  return false;
}

float vecDist (Vec v0, Vec v1)
{
  Vec result;
  vecSub(v0, v1, result);
  return vecLength(result);
}

void vecNegate (Vec v)
{
	v[0] = -v[0];
	v[1] = -v[1];
}

float vecDotProduct (Vec v0, Vec v1)
{
	return v0[0]*v1[0] + v0[1]*v1[1];
}

float vecLength (Vec v)
{
	return sqrt(vecDotProduct(v, v));
}

void vecNormalize (Vec v)
{
	float len = vecLength(v);
	if (len == 0.0) {
		vecMake(0,0,v);
		return;
	}
	len = 1.0 / len;
	v[0] *= len;
	v[1] *= len;
  v[2] = 0.0;
}

void vecSub (Vec vTo, Vec vFrom, Vec dest)
{
	dest[0] = vTo[0] - vFrom[0];
	dest[1] = vTo[1] - vFrom[1];
  dest[2] = 0.0;
}

void vecPerpendicular (Vec source, Vec dest)
{
  dest[0] = -source[1];
  dest[1] = source[0];
  dest[2] = 0.0;
}

// void getTriangleNormal (Vec tri[3], Vec norm)
// {
//	Vec v0,v1;
//	vecSub(tri[1], tri[0], v0);
//	vecSub(tri[2], tri[0], v1);
//	vecCrossProduct(v0, v1, norm);
//	vecNormalize(norm);
// }

// void getNormal (Vec v0, Vec v1, Vec v2, Vec norm)
// {
//	Vec t0, t1;
//	vecSub(v0, v1, t0);
//	vecSub(v2, v1, t1);
//	vecCrossProduct(t0, t1, norm);
//	vecNormalize(norm);
// }

float getHighestX (Vec a, Vec b, Vec c)
{
  float max;
  if (a[0] < b[0]) {
    max = b[0];
  }
  else {
    max = a[0];
  }
  if (max < c[0]) {
    max = c[0];
  }
  return max;
}

float getLowestX (Vec a, Vec b, Vec c)
{
  float min;
  if (a[0] > b[0]) {
    min = b[0];
  }
  else {
    min = a[0];
  }
  if (min > c[0]) {
    min = c[0];
  }
  return min;
}

float getHighestY (Vec a, Vec b, Vec c)
{
  float max;
  if (a[1] < b[1]) {
    max = b[1];
  }
  else {
    max = a[1];
  }
  if (max < c[1]) {
    max = c[1];
  }
  return max;
}

float getLowestY (Vec a, Vec b, Vec c)
{
  float min;
  if (a[1] > b[1]) {
    min = b[1];
  }
  else {
    min = a[1];
  }
  if (min > c[1]) {
    min = c[1];
  }
  return min;
}

std::string vecPrint (Vec pt)
{
  ostringstream oss;
  oss << "(" << pt[0] << "," << pt[1] << "," << pt[2] << ")";
  return oss.str();
}



