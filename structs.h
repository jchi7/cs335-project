#ifndef STUCTS_H_INCLUDED
#define STUCTS_H_INCLUDED

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <GL/glx.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <typeinfo>
#include <vector>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "vecFxns.h"

using namespace std;

enum ShapeType {RECTANGLE, TRIANGLE/*, CIRCLE*/};
enum GameState {MAIN_MENU, PLAYING, EXIT_GAME, LEVEL_EDITOR};
enum ObjectType {HERO, ENEMY, SPIKE, PLATFORM, SAVEPOINT, SHOOTERENEMY, MELEEENEMY, HBULLET, EBULLET};
enum CharacterState {STANDING, WALKING, JUMPING, DEATH};
enum ShapeOrientation {FACING_UP, FACING_LEFT, FACING_DOWN, FACING_RIGHT};

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

// Shape structure used to represent dimensions and position on the screen
struct Shape
{
  ShapeType type;
  Vec center;
  Vec corners[3];
  ShapeOrientation orientation;
  float width, height;
  float radius;
};

struct Coordinates {
    float x1,x2,y1,y2;
};

//External Global Variables
extern GameState g_gamestate;
// extern int leftPressed ;
// extern int rightPressed ;
// extern int shootPressed ;
// extern int jumpInitiated ;
// extern int jumpFinished ;
// extern int numCollisions;
// extern int currentHorizontalLevel;
// extern int currentVerticalLevel;
#endif // STUCTS_H_INCLUDED
