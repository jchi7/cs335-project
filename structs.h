#ifndef STUCTS_H_INCLUDED
#define STUCTS_H_INCLUDED
typedef struct t_rect {
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

typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;

// self explanatory
// X is the first position, Y is the second position and Z is the third position
typedef float Vec[3];

// Shape structure used to represent dimensions and position on the screen
struct Shape {
    float width, height;
    float radius;
    Vec center;
    Vec velocity;
};

//External Global Variables
extern int leftPressed ;
extern int rightPressed ;
extern int shootPressed ;
extern int jumpInitiated ;
extern int jumpFinished ;
extern int numCollisions;
extern int currentHorizontalLevel;
extern int currentVerticalLevel;
#endif // STUCTS_H_INCLUDED
