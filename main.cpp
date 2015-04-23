#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <sys/time.h>
#include <typeinfo>
#include <vector>
#include "structs.h"
#include "gameObject.h"
#include "hero.h"
#include "basicEnemy.h"
#include "platform.h"
#include "Level.h"
#include "game.h"
//#include "initializeLevels.h"


#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define GRAVITY -0.35
#define MAXBUTTONS 4

using namespace std;

void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void set_title(void);
void init_MainMenuButtons(void);
void render_MainMenu(void);
void check_menu_button(XEvent *e);
void render_game(game* game);
Level*** initializeLevels();

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

Button button[MAXBUTTONS];
int nbuttons=0;

enum GameState {MAIN_MENU, PLAYING, EXIT_GAME};
GameState g_gamestate = MAIN_MENU;

int numCollisions;

int main()
{
    initXWindows();
    init_opengl();
    init_MainMenuButtons();
    Level*** levels = initializeLevels();
    game newgame(levels);
    newgame.hero = new hero();

    while(g_gamestate != EXIT_GAME) {
        switch (g_gamestate) {
            case MAIN_MENU:
                while(XPending(dpy)) {
                    XEvent e;
                    XNextEvent(dpy, &e);
                    check_menu_button(&e);
                }
                render_MainMenu();
                break;
            case PLAYING:
                render_game(&newgame);
                break;
            case EXIT_GAME:
                break;
            default:
                break;
        }
        glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    return 0;
}

void initXWindows(void) {
    //do not change
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w=WINDOW_WIDTH, h=WINDOW_HEIGHT;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        std::cout << "\n\tcannot connect to X server\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) {
        std::cout << "\n\tno appropriate visual found\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPress | ButtonReleaseMask |
        PointerMotionMask |
        StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
            InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void init_opengl(void) {
    //OpenGL initialization
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void cleanupXWindows(void) {
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "CSUB Professor Smash");
}

void init_MainMenuButtons(void) {
	//initialize buttons...
	nbuttons=0;
	//size and position
	button[nbuttons].r.width = 380;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 290;
	button[nbuttons].r.bot = 320;
	button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Reset");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.4f;
	button[nbuttons].color[1] = 0.4f;
	button[nbuttons].color[2] = 0.7f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
	button[nbuttons].r.width = 380;
	button[nbuttons].r.height = 60;
	button[nbuttons].r.left = 290;
	button[nbuttons].r.bot = 160;
	button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
	button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
	button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
	button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
	strcpy(button[nbuttons].text, "Quit");
	button[nbuttons].down = 0;
	button[nbuttons].click = 0;
	button[nbuttons].color[0] = 0.3f;
	button[nbuttons].color[1] = 0.3f;
	button[nbuttons].color[2] = 0.6f;
	button[nbuttons].dcolor[0] = button[nbuttons].color[0] * 0.5f;
	button[nbuttons].dcolor[1] = button[nbuttons].color[1] * 0.5f;
	button[nbuttons].dcolor[2] = button[nbuttons].color[2] * 0.5f;
	button[nbuttons].text_color = 0x00ffffff;
	nbuttons++;
}

void render_MainMenu(void) {
    //Rect r;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(200,200,200);
    glPushMatrix();
    for (int i=0; i<nbuttons; i++) {
		if (button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(3);
			glBegin(GL_LINE_LOOP);
				glVertex2i(button[i].r.left-w,  button[i].r.bot-w);
				glVertex2i(button[i].r.left-w,  button[i].r.top+w);
				glVertex2i(button[i].r.right+w, button[i].r.top+w);
				glVertex2i(button[i].r.right+w, button[i].r.bot-w);
				glVertex2i(button[i].r.left-w,  button[i].r.bot-w);
			glEnd();
			glLineWidth(1);
		}
		if (button[i].down) {
			glColor3fv(button[i].dcolor);
		} else {
			glColor3fv(button[i].color);
		}
		glBegin(GL_QUADS);
			glVertex2i(button[i].r.left,  button[i].r.bot);
			glVertex2i(button[i].r.left,  button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();
		//r.left = button[i].r.centerx;
		//r.bot  = button[i].r.centery-8;
		//r.center = 1;
		/*
		if (button[i].down) {
			ggprint16(&r, 0, button[i].text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, button[i].text_color, button[i].text);
		}
		*/
		glPopMatrix();
	}
}

void check_menu_button(XEvent *e) {
    static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	//int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		//if (e->xbutton.button==3) {
			//Right button is down
			//rbutton=1;
		//}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = WINDOW_HEIGHT - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<nbuttons; i++) {
		button[i].over=0;
		if (x >= button[i].r.left &&
			x <= button[i].r.right &&
			y >= button[i].r.bot &&
			y <= button[i].r.top) {
			button[i].over=1;
			if (button[i].over) {
				if (lbutton) {
					switch(i) {
						case 0:
							g_gamestate = PLAYING;
							break;
						case 1:
							g_gamestate = EXIT_GAME;
							break;
					}
				}
			}
		}
	}
	return;
}

void render_game(game* game)
{
    Level* current_level = game->level[game->currentHorizontalLevel][game->currentVerticalLevel];

    glClear(GL_COLOR_BUFFER_BIT);
    float w, h;

    // Draw the Hero to the screen
    glColor3ub(200,200,200);
    glPushMatrix();
    glTranslatef(game->hero->body.center[0], game->hero->body.center[1], game->hero->body.center[2]);
    w = game->hero->body.width;
    h = game->hero->body.height;
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w,h);
    glVertex2i(w,-h);
    glVertex2i(w,h);
    glEnd();
    glPopMatrix();

    for(auto &entity : current_level->enemies) {
        glColor3ub(entity->rgb[0], entity->rgb[1], entity->rgb[2]);
        glPushMatrix();
        glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
        glBegin(GL_QUADS);
        glVertex2i(-entity->body.width,-entity->body.height);
        glVertex2i(-entity->body.width,entity->body.height);
        glVertex2i(entity->body.width,-entity->body.height);
        glVertex2i(entity->body.width,entity->body.height);
        glEnd();
        glPopMatrix();
    }

    for(auto entity : current_level->bullet) {
        glColor3ub(entity->rgb[0], entity->rgb[1], entity->rgb[2]);
        glPushMatrix();
        glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
        glBegin(GL_QUADS);
        glVertex2i(-entity->body.width,-entity->body.height);
        glVertex2i(-entity->body.width,entity->body.height);
        glVertex2i(entity->body.width,entity->body.height);
        glVertex2i(entity->body.width,-entity->body.height);
        glEnd();
        glPopMatrix();
    }

    for(auto entity : current_level->objects) {
        glColor3ub(entity->rgb[0], entity->rgb[1], entity->rgb[2]);
        glPushMatrix();
        glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
        glBegin(GL_QUADS);
        glVertex2i(-entity->body.width,-entity->body.height);
        glVertex2i(-entity->body.width,entity->body.height);
        glVertex2i(entity->body.width,entity->body.height);
        glVertex2i(entity->body.width,-entity->body.height);
        glEnd();
        glPopMatrix();
    }
}

Level*** initializeLevels()
{
    Level*** room = (Level***)malloc(20 * sizeof(Level**));
    int count = 0;
    while (count < 20)
    {
        room[count] = (Level**)malloc( 5 * sizeof(void*));
        count++;
    }
    //Level* temp;
    //temp = new Level(13,1);
    room[10][2] = new Level(13,1);
    room[11][2] = new Level(7,0);
    room[9][2] = new Level(7,0);
    room[10][3] = new Level(1,0);
    room[10][2]->horizontalPosition = 10;
    room[10][2]->verticalPosition = 2;
    room[11][2]->horizontalPosition = 11;
    room[11][2]->verticalPosition = 2;
    room[9][2]->horizontalPosition = 9;
    room[9][2]->verticalPosition = 2;
    room[10][3]->horizontalPosition = 10;
    room[10][3]->verticalPosition = 3;

    room[10][2]->objects.push_back(new platform(500,50,450,50));
    room[10][2]->objects.push_back(new platform(100,25,200,150));
    room[10][2]->objects.push_back(new platform(100,25,500,150));
    room[10][2]->objects.push_back(new platform(100,15,600,180));
    room[10][2]->objects.push_back(new platform(15,15,650,260));

    return room;
}