#include "structs.h"
#include "gameObject.h"
#include "hero.h"
#include "basicEnemy.h"
#include "platform.h"
#include "spike.h"
#include "room.h"
#include "ppm.h"
#include "game.h"
#include "collisions.h"
#include <chrono>
#include <GL/glx.h>
#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define GRAVITY -0.35
#define MAXBUTTONS 4

using namespace std;

void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void set_title(void);
void renderBackground(GLuint);
void init_MainMenuButtons(void);
void render_MainMenu(void);
void check_menu_button(XEvent *e, Game * game);
void check_game_input(XEvent *e, Game * game);
void movePlatform(XEvent *e, Game * game);
void physics(Game * game);
void render_game(Game* game);

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

Button button[MAXBUTTONS];
int nbuttons=0;

GameState g_gamestate = MAIN_MENU;

int numCollisions;

struct timeval Gthrottle;
int GoldMilliSec = 0;
int GtimeLapse = 0;
int Gthreshold = 15000;
//Following Declarations are for  Image importing...
unsigned char *buildAlphaData(Ppmimage *img);
void renderHero(GLuint heroTexture,Game* game  ,Coordinates* heroSprite,int index,int w, int h);
GLuint getBMP(const char *path);
Ppmimage *guiBackgroundImage = NULL;
Ppmimage *rockImage = NULL;
Ppmimage *heroImage = NULL;
Ppmimage *backgroundImage = NULL;
Ppmimage *mainMenuButtonsImage = NULL;
Ppmimage *mainMenuButtonsExitImage = NULL;
GLuint guiBackgroundTexture;
GLuint rockTexture;
GLuint heroTexture;
GLuint forestTexture;
GLuint mainMenuButtonsTexture;
GLuint mainMenuButtonsExitTexture;
bool forestBackgroundSet=true;
CharacterState prevPosition;
int numAnimation = 0;
auto start = std::chrono::high_resolution_clock::now();
//End

GameObject mouse;

int main()
{
    initXWindows();
    init_opengl();
    init_MainMenuButtons();
    //Game newgame();  //says newgame is non-class type 'Game()'
    Game newgame;
    newgame.hero = new Hero();

    bool render = true;
    bool doPhysics = true;

    while(g_gamestate != EXIT_GAME) {
        gettimeofday(&Gthrottle, NULL);
        GtimeLapse = (Gthrottle.tv_usec >= GoldMilliSec) ? Gthrottle.tv_usec - GoldMilliSec :
            (1000000 - GoldMilliSec) + Gthrottle.tv_usec;
        if (GtimeLapse >= Gthreshold){
            GoldMilliSec = Gthrottle.tv_usec;
            render = true;
        }
        else{
            render = false;
        }
 
        switch (g_gamestate) {
            case MAIN_MENU:
                while(XPending(dpy)) {
                    XEvent e;
                    XNextEvent(dpy, &e);
                    check_menu_button(&e,&newgame);
                }
                if (render == true){
                    render_MainMenu();
                    glXSwapBuffers(dpy, win);
                }
                break;
            case PLAYING:
                while(XPending(dpy)) {
                    XEvent e;
                    XNextEvent(dpy, &e);
                    check_game_input(&e, &newgame);
                }
                if (doPhysics == true){
                    physics(&newgame);
                    doPhysics = false;
                }
                if (render == true){
                    doPhysics = true;
                    render_game(&newgame);
                    glXSwapBuffers(dpy, win);
                }
                break;
            case LEVEL_EDITOR:
                while(XPending(dpy)) {
                    XEvent e;
                    XNextEvent(dpy, &e);
                    check_game_input(&e, &newgame);
                    if (newgame.isPlatformMovable == true)
                        movePlatform(&e, &newgame);
                }
                if (doPhysics == true){
                    physics(&newgame);
                    doPhysics = false;

                }
                if (render == true){
                    doPhysics = true;
                    render_game(&newgame);
                    glXSwapBuffers(dpy, win);
                }
                break;
            case EXIT_GAME:
                break;
            default:
                break;
        }
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

    //Importing Images

    heroImage = ppm6GetImage("./images/HeroSpriteSheet.ppm");
    backgroundImage = ppm6GetImage("./images/Background1.ppm");
    rockImage = ppm6GetImage("./images/Rock.ppm");
    mainMenuButtonsImage = ppm6GetImage("./images/start.ppm");
    guiBackgroundImage = ppm6GetImage("./images/GuiBackground.ppm");
    mainMenuButtonsExitImage = ppm6GetImage("./images/exit.ppm");
    glGenTextures(1, &heroTexture);
    glGenTextures(1, &forestTexture);
    glGenTextures(1, &rockTexture);
    glGenTextures(1, &mainMenuButtonsTexture);
    glGenTextures(1, &guiBackgroundTexture);
    glGenTextures(1, &mainMenuButtonsExitTexture);

    int w = heroImage->width;
    int y = heroImage->height;
    //Setting up the hero textures
    glBindTexture(GL_TEXTURE_2D,heroTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w,y, 0,
            GL_RGB, GL_UNSIGNED_BYTE, heroImage->data);

    unsigned char *silhouetteData = buildAlphaData(heroImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, y, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    delete [] silhouetteData;


    //Setting up the Main menu buttons texture...
    w = mainMenuButtonsImage->width;
    y = mainMenuButtonsImage->height;
    glBindTexture(GL_TEXTURE_2D,mainMenuButtonsTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, y, 0,
            GL_RGB, GL_UNSIGNED_BYTE, mainMenuButtonsImage->data);

    //Setting up the exit button image texture....
    w = mainMenuButtonsExitImage->width;
    y = mainMenuButtonsExitImage->height;
    glBindTexture(GL_TEXTURE_2D,mainMenuButtonsExitTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, y, 0,
            GL_RGB, GL_UNSIGNED_BYTE, mainMenuButtonsExitImage->data);


    //Setting up the Rock Platforms Texture....
    w = rockImage->width;
    y = rockImage->height;
    glBindTexture(GL_TEXTURE_2D,rockTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, y, 0,
            GL_RGB, GL_UNSIGNED_BYTE, rockImage->data);

    //Setting up the background image
    glBindTexture(GL_TEXTURE_2D,forestTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, backgroundImage->width, backgroundImage->height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, backgroundImage->data);

    //Setting up the Gui Background image.
    glBindTexture(GL_TEXTURE_2D,guiBackgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, guiBackgroundImage->width, guiBackgroundImage->height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, guiBackgroundImage->data);
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
    button[nbuttons].r.width = 100;
    button[nbuttons].r.height = 40;
    button[nbuttons].r.left = 290;
    button[nbuttons].r.bot = 480;
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
    button[nbuttons].r.width = 100;
    button[nbuttons].r.height = 40;
    button[nbuttons].r.left = 290;
    button[nbuttons].r.bot = 320;
    button[nbuttons].r.right = button[nbuttons].r.left + button[nbuttons].r.width;
    button[nbuttons].r.top = button[nbuttons].r.bot + button[nbuttons].r.height;
    button[nbuttons].r.centerx = (button[nbuttons].r.left + button[nbuttons].r.right) / 2;
    button[nbuttons].r.centery = (button[nbuttons].r.bot + button[nbuttons].r.top) / 2;
    strcpy(button[nbuttons].text, "Level Editor");
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
    button[nbuttons].r.width = 100;
    button[nbuttons].r.height = 40;
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
  //glColor3ub(200,200,200);
  glColor3f(1.0,1.0,1.0);

  glEnable(GL_TEXTURE_2D);
  renderBackground(guiBackgroundTexture);
  glDisable(GL_TEXTURE_2D);
  
  glPushMatrix();
  for (int i=0; i<nbuttons; i++) {
		if (button[i].over) {
			int w=2;
			glColor3f(1.0f, 1.0f, 0.0f);
			//Ba highlight around button
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
        
        //Rendering the menu Items
        if (i == 0) {

            glEnable(GL_TEXTURE_2D);
            glColor4ub(255,255,255,255);
            glPushMatrix();
            glTranslatef(button[i].r.centerx,button[i].r.centery, 0.0f);
            glBindTexture(GL_TEXTURE_2D,mainMenuButtonsTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(0.1f,.9f); glVertex2i(-50,-20);
            glTexCoord2f(0.1f,0.1f); glVertex2i(-50,20); //smenu
            glTexCoord2f(.9f,0.1f); glVertex2i(50,20);
            glTexCoord2f(.9f,.9f); glVertex2i(50,-20);
            glEnd();
            glPopMatrix();
            //BmainMenuButtonsTexture,0.1,0.9,0.1,0.9, 200, 30);
        }

        if(i == 2) {
            //This will be a call to the function to render.
            //BmainMenuButtonsExitTexture,0.1,0.9,0.1,0.9,200,30);
            glEnable(GL_TEXTURE_2D);
            glColor4ub(255,255,255,255);
            glPushMatrix();
            glTranslatef(button[i].r.centerx,button[i].r.centery, 0.0f);
            glBindTexture(GL_TEXTURE_2D,mainMenuButtonsExitTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(0.1f,.9f); glVertex2i(-50,-20);
            glTexCoord2f(0.1f,0.1f); glVertex2i(-50,20); //smenu
            glTexCoord2f(.9f,0.1f); glVertex2i(50,20);
            glTexCoord2f(.9f,.9f); glVertex2i(50,-20);
            glEnd();
            glPopMatrix();
        }

	}
}
void movePlatform(XEvent *e, Game *game){


    Room * currentRoom = game->getRoomPtr();
    currentRoom->platforms[game->movablePlatformIndex]->body.center[0] = e->xbutton.x;
    currentRoom->platforms[game->movablePlatformIndex]->body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
}

void check_menu_button(XEvent *e, Game * game)
{
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
              game->state = PLAYING;
							break;
						case 1:
							g_gamestate = LEVEL_EDITOR;
              game->state = LEVEL_EDITOR;
							break;
            case 2:
              g_gamestate = EXIT_GAME;
              break;
					}
				}
			}
		}
	}
	return;
}

void check_game_input(XEvent *e, Game *game)
{
    mouse.body.type = RECTANGLE;
    mouse.body.width = 1;
    mouse.body.height = 1;

    if (e->type == KeyPress){
        //cout << e->xbutton.x << endl;
        int key = XLookupKeysym(&e->xkey,0);
        if (key == XK_Left){
            game->hero->leftPressed = 1;
        }
        if (key == XK_Right){
            game->hero->rightPressed = 1;
        }
        if (key == XK_Escape){
            g_gamestate = MAIN_MENU;
        }
        
        if ((key == XK_Up || key == XK_w || key == XK_space) && game->hero->jumpRelease == 0){
            if (game->hero->state == WALKING || game->hero->state == STANDING){
                game->hero->initialJump = 1;
            }
            if (game->hero->state == JUMPING && game->hero->jumpCount < 2){
                game->hero->secondJump = 1;
            }
        }
        if (game->state == LEVEL_EDITOR){
            if (key == XK_b){
                game->saveRooms();
            }
            if (key == XK_j){
                game->moveRoomLeft();
            }
            if (key == XK_l){
                game->moveRoomRight();
            }
            if (key == XK_k){
                game->moveRoomDown();
            }
            if (key == XK_i){
                game->moveRoomUp();
            }
            if (key == XK_5){
                game->hero->body.center[0] = e->xbutton.x;
                game->hero->body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
            }
            if (key == XK_Shift_L){
                if (!game->isPlatformMovable && game->isPlatformResizable == false){
                    Room * room = game->getRoomPtr();
                    room->platforms.push_back(new Platform(game->platformTextureWidth,game->platformTextureHeight,e->xbutton.x, WINDOW_HEIGHT - e->xbutton.y));
                    room->numPlatforms++;
                    game->isPlatformMovable = true;
                    game->movablePlatformIndex = room->platforms.size() - 1;
                }
            }
            if (key == XK_s){
                Room * room = game->getRoomPtr();
                Vec spike[3];
                spike[0][0] = e->xbutton.x;
                spike[0][1] = WINDOW_HEIGHT - e->xbutton.y;
                spike[0][2] = 0;
                spike[1][0] = e->xbutton.x + 40;
                spike[1][1] = WINDOW_HEIGHT - e->xbutton.y;
                spike[1][2] = 0;
                spike[2][0] = e->xbutton.x + 20;
                // (sqrt(3) / 2 ) * width  = height for equilateral triangle
                spike[2][1] = WINDOW_HEIGHT - e->xbutton.y + 34.641;
                spike[2][2] = 0;
                room->spikes.push_back(new Spike(spike));
                room->numSpikes++;
            }
            if (key == XK_x){
                if (game->isPlatformMovable && game->isPlatformResizable == false){
                    game->isPlatformMovable = false;
                }
            }
            if (key == XK_z){
                if (!game->isPlatformMovable && game->isPlatformResizable == false){
                    Room * room = game->getRoomPtr();
                    mouse.body.center[0] = e->xbutton.x;
                    mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
            //        cout << mouse.body.center[0] << " " << mouse.body.center[1] << endl;
                    for (unsigned int k = 0; k < room->platforms.size(); k++){
                        if (collisionRectRect(&mouse.body,&room->platforms[k]->body)){
                            game->movablePlatformIndex = k;
                            game->isPlatformMovable = true;
                            cout << mouse.body.center[0] << " " << mouse.body.center[1] << endl;
                            cout << k << endl;
                        }
                    }
                }   
            }
            if (key == XK_c && game->isPlatformMovable == false && game->isPlatformMovable == false){
                Room * room = game->getRoomPtr();
                mouse.body.center[0] = e->xbutton.x;
                mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
                for (unsigned int k = 0; k < room->platforms.size(); k++){
                    if (collisionRectRect(&mouse.body,&room->platforms[k]->body)){
                        game->resizablePlatformIndex = k;
                        game->isPlatformResizable = true;
                        game->resizablePlatformX = room->platforms[k]->body.center[0];
                        game->resizablePlatformY = room->platforms[k]->body.center[1];
                    }
                }
            }
            if (key == XK_v && game->isPlatformMovable == false && game->isPlatformResizable == true){
                game->isPlatformResizable = false;
            }
            if (key == XK_d && game->isPlatformMovable == false && game->isPlatformResizable == false){
                Room * room = game->getRoomPtr();
                int platformToRemove = 0;
                bool isCollision = false;
                mouse.body.center[0] = e->xbutton.x;
                mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
                for (unsigned int k = 0; k < room->platforms.size(); k++){
                    if (collisionRectRect(&mouse.body,&room->platforms[k]->body)){
                        platformToRemove = k;
                        isCollision = true;
                    }
                }
                if (isCollision){
                    room->platforms.erase(room->platforms.begin() + platformToRemove);
                    room->numPlatforms--;
                }
            }
        
        }
    
        /*
        if (key == XK_e && shootPressed == 0){
            game->hero->shootPressed = 5;
        }*/

    }
    if (e->type == KeyRelease){
        int key = XLookupKeysym(&e->xkey,0);
        if ( key == XK_Left){
            game->hero->leftPressed = 0;
        }
        if ( key == XK_Right){
            game->hero->rightPressed = 0;
        }
        if ( key == XK_w || key == XK_Up || key == XK_space){
            game->hero->jumpRelease = 4;
        }
    }

    if (game->isPlatformResizable){
        //Room * room = game->getRoomPtr(); // UNUSED!
        mouse.body.center[0] = e->xbutton.x;
        mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
        game->resizePlatform(&mouse);
    }
}

void physics(Game * game)
{
    bool isCollision = false;
    Room * room = game->getRoomPtr();

    game->hero->movement();
    for (int i = 0; i < room->numPlatforms; i++) {
        isCollision = collisionRectRect(&game->hero->body, &room->platforms[i]->body);
        if (isCollision == true) {
            game->hero->onCollision(room->platforms[i]);
        }
    }
    if (isCollision == false) {
        for (int i = 0; i < room->numSpikes; i++) {
            isCollision = collisionRectTri(&game->hero->body, &room->spikes[i]->body);
            if (isCollision == true) {
                game->hero->onCollision(room->spikes[i]);
            }
        }
    }

    if (game->hero->state == DEATH) {
        // TEMPORARY: return hero to start
        game->hero->state = JUMPING;
        game->hero->jumpInitiated = 0;
        game->hero->initialJump = 0;
        game->hero->secondJump = 0;
        game->hero->jumpCount = 0;
        game->hero->jumpRelease = 1;
        game->hero->jumpFinished = 0;
        game->hero->body.center[0] = 400;
        game->hero->body.center[1] = 250;
        game->hero->body.center[2] = 0;
        game->hero->prevPosition[0] = 400;
        game->hero->prevPosition[1] = 250;
        game->hero->prevPosition[2] = 0;
        game->hero->velocity[0] = 0;
        game->hero->velocity[1] = 0;
        game->currentHorizontalLevel = 3;
        game->currentVerticalLevel = 1;
    }
    game->checkRoom();
}

void render_game(Game* game)
{
    Room* current_level = game->getRoomPtr();

    glClear(GL_COLOR_BUFFER_BIT);
    float w, h;
    
    glColor3f(1.0,1.0,1.0);
    if( forestBackgroundSet == true ) {
        renderBackground(forestTexture);
    }


    // Draw the Hero to the screen
    w = game->hero->body.width;
    h = game->hero->body.height;
 
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    if(microseconds > 80000) {
        if (game->hero->state == WALKING && game->hero->rightPressed && game->hero->leftPressed == 0) {
            renderHero(heroTexture,game ,game->hero->heroWalkingR,numAnimation,w, h);
        }
        else if (game->hero->state == WALKING && game->hero->leftPressed && game -> hero->rightPressed == 0) {
            renderHero(heroTexture,game  ,game->hero->heroWalkingL,numAnimation,w, h);
        }
        else if (game->hero->state == JUMPING) {
            renderHero(heroTexture,game  ,game->hero->heroJump,numAnimation,w, h);
        }
        else {
            renderHero(heroTexture,game  ,game->hero->heroIdleR,numAnimation,w, h);
        }
        numAnimation = (numAnimation + 1) % 10;
        start = std::chrono::high_resolution_clock::now();
    }

    else {
        if (game->hero->state == WALKING && game->hero->rightPressed && game->hero->leftPressed == 0) {
            renderHero(heroTexture,game  ,game->hero->heroWalkingR,numAnimation,w, h);
            std::cout<<"Num: "<<numAnimation<<endl;
        }
        else if (game->hero->state == WALKING && game->hero->leftPressed && game -> hero->rightPressed == 0) {
            renderHero(heroTexture,game  ,game->hero->heroWalkingL,numAnimation,w, h);
        }
        else if (game->hero->state == JUMPING) {
            renderHero(heroTexture,game  ,game->hero->heroJump,numAnimation,w, h);
        }
        else {
            renderHero(heroTexture,game  ,game->hero->heroIdleR,numAnimation,w, h);
        }
    }

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

    for(auto entity : current_level->platforms) {
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

        w = entity -> body.width;
        h = entity -> body.height;

        //The follwoing code is to draw the platforms
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255,255,255,255);
        glPushMatrix();
        glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
        glBindTexture(GL_TEXTURE_2D, rockTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.1f,1.0f); glVertex2i(-w,-h);
        glTexCoord2f(0.1f,0.0f); glVertex2i(-w,h);
        glTexCoord2f(1.0f,0.0f); glVertex2i(w,h);
        glTexCoord2f(1.0f,1.0f); glVertex2i(w,-h);
        glEnd();
        glPopMatrix();

    }

    for(auto entity : current_level->spikes) {
        glColor3ub(entity->rgb[0], entity->rgb[1], entity->rgb[2]);
        glPushMatrix();
 //     glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
        glBegin(GL_TRIANGLES);
            glVertex3fv(entity->body.corners[0]);
            glVertex3fv(entity->body.corners[1]);
            glVertex3fv(entity->body.corners[2]);
        glEnd();
        glPopMatrix();
    }
}

void renderBackground(GLuint backgroundTexture)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,backgroundTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,1.0f); glVertex2i(0,0);
    glTexCoord2f(0.0,0.0); glVertex2i(0,WINDOW_HEIGHT);
    glTexCoord2f(1.0,0.0); glVertex2i(WINDOW_WIDTH,WINDOW_HEIGHT);
    glTexCoord2f(1.0f,1.0f); glVertex2i(WINDOW_WIDTH,0);
    glPopMatrix();
    glEnd();
}

void renderTexture(GLuint imageTexture, float x1,float x2,float y1, float y2, int width, int height)
  {
      glEnable(GL_TEXTURE_2D);
      glColor4ub(255,255,255,255);
      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D,imageTexture);
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER,0.0f);
      glBegin(GL_QUADS);
      glTexCoord2f(x1,y1); glVertex2i(width,height);
      glTexCoord2f(x1,y2); glVertex2i(-width,height);
      glTexCoord2f(x2,y2); glVertex2i(width,height);
      glTexCoord2f(x2,y1); glVertex2i(width,-height);
      glBindTexture(GL_TEXTURE_2D,0);
      glPopMatrix();
      glEnd();
  }

 //Grabbed this code from Gordons rainforest program.
unsigned char *buildAlphaData(Ppmimage *img)
{
    //add 4th component to RGB stream...
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    //newdata = (unsigned char *)malloc(img->width * img->height * 4);
    newdata = new unsigned char[img->width * img->height * 4];
    ptr = newdata;
    for (int i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        //
        //get the alpha value
        //
        //original code
        //get largest color component...
        //*(ptr+3) = (unsigned char)((
        //      (int)*(ptr+0) +
        //      (int)*(ptr+1) +
        //      (int)*(ptr+2)) / 3);
        //d = a;
        //if (b >= a && b >= c) d = b;
        //if (c >= a && c >= b) d = c;
        //*(ptr+3) = d;
        //
        //new code, suggested by Chris Smith, Fall 2013
        *(ptr+3) = (a|b|c);
        //
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void renderHero(GLuint heroTexture,Game* game,Coordinates* heroSprite,int index,int w, int h)
{
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

    glPushMatrix();
    glTranslatef(game->hero->body.center[0], game->hero->body.center[1], game->hero->body.center[2]);
    glBindTexture(GL_TEXTURE_2D,heroTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(heroSprite[index%10].x1,heroSprite[index%10].y2); glVertex2i(-w-5,-h-5);
    glTexCoord2f(heroSprite[index%10].x1,heroSprite[index%10].y1); glVertex2i(-w-5,h+5); //here
    glTexCoord2f(heroSprite[index%10].x2,heroSprite[index%10].y1); glVertex2i(w+5,h+5);
    glTexCoord2f(heroSprite[index%10].x2,heroSprite[index%10].y2); glVertex2i(w+5,-h-5);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    glEnd();
    glPopMatrix();
}







