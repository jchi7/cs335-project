#ifndef STUCTS_H_INCLUDED
#define STUCTS_H_INCLUDED

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <typeinfo>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>

#include "vecFxns.h"

using namespace std;

typedef struct t_rect
{
	int left;
	int top;
	int right;
	int bot;
	int width;
	int height;
	int center;
	int centerx;
	int centery;
} Rect;

typedef struct t_button
{
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;

enum ShapeType {RECTANGLE, TRIANGLE, CIRCLE};

// Shape structure used to represent dimensions and position on the screen
struct Shape
{
  ShapeType type;
  Vec center;
  Vec corners[3];
  float width, height;
  float radius;
};

//External Global Variables
// extern int leftPressed ;
// extern int rightPressed ;
// extern int shootPressed ;
// extern int jumpInitiated ;
// extern int jumpFinished ;
// extern int numCollisions;
// extern int currentHorizontalLevel;
// extern int currentVerticalLevel;
#endif // STUCTS_H_INCLUDED
