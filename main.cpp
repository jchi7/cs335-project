#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <sys/time.h>
#include <typeinfo>
#include "globals.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

#define GRAVITY -0.35

#define HORIZONTAL_LEVELS 20
#define VERTICAL_LEVELS 5

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//Function prototypes
int checkGameQuit(XEvent *e, Game *game);
int checkMoveKey(XEvent *e, Game *game);

//         Global variables

int leftPressed = 0;
int rightPressed = 0;
int shootPressed = 0;

// jumpInitiated is set to 1 when the jump key is pressed
int jumpInitiated = 0;
// jumpFinished is used to prevent the hero from double jumping or
// jumping after falling off of a platform
int jumpFinished = 1;
int numCollisions;

//Create a 2-dimensional array that holds pointers to levels.
//The indexes represent the location of the level relative to other
//levels based on a cartesian sort of mapping. So level[5][2] holds
//a pointer to the level that is at coordinate (5,2).
//Level * level[HORIZONTAL_LEVELS][VERTICAL_LEVELS];
Level *** level;
int currentHorizontalLevel;
int currentVerticalLevel;

// These are used for throttling the game
struct timeval throttle;
int oldMilliSec = 0;
int timeLapse = 0;
int threshold = 10000;

using namespace std;

int main(void)
{
    level = (Level***)malloc(20 * sizeof(Level**));
    int count = 0;
    while (count < 20)
    {
        level[count] = (Level**)malloc( 5 * sizeof(void*));
        count++;
    }
    initializeLevels();
    int done=0;
    srand(time(NULL));
    initXWindows();
    init_opengl();

    Hero *hero = new Hero;
    hero->numBullets = 0;
    hero->maxBullets = 3;
    for (int j = 0; j < hero->maxBullets; j++){
        hero->bullet[j].width = 5;
        hero->bullet[j].height = 5;
    }
    hero->bulletVelocity = 10;
    hero->body.width = 8;
    hero->body.height = 15;
    hero->body.center.x = 400;
    hero->body.center.y = 250;
    hero->body.center.z = 0;
    hero->prevPosition.center.x = 400;
    hero->prevPosition.center.y = 150;
    hero->velocity.x = 0;
    hero->velocity.y = 0;
    hero->facing = 1;
    
    int testTime = 0;
    Game * game = new Game(level[10][2],hero); 
    while(!done) {

        testTime = gettimeofday(&throttle,NULL);
        timeLapse = (throttle.tv_usec >= oldMilliSec) ? throttle.tv_usec - oldMilliSec :
            (1000000 - oldMilliSec) + throttle.tv_usec;
        if (timeLapse >= threshold){
            
            oldMilliSec = throttle.tv_usec;
            while(XPending(dpy)) {
                XEvent e;
                XNextEvent(dpy, &e);
                done = checkGameQuit(&e, game);
                checkMoveKey(&e, game);
            }
            movement(game);
            render(game);
            glXSwapBuffers(dpy, win);
        }
    }
    cleanupXWindows();
    return 0;
}

void createPlatform(int width, int height, int x, int y, Level * level){

    
    ((level->platform)+(level->currentPlatform))->width = width;
    ((level->platform)+(level->currentPlatform))->height = height;
    ((level->platform)+(level->currentPlatform))->center.x = x;
    ((level->platform)+(level->currentPlatform))->center.y = y;
    ((level->platform)+(level->currentPlatform))->center.z = 0;
    level->currentPlatform++;

}

void createBasicEnemy(int x, int y,int left, int right, Level * level){

    ((level->basicEnemy)+(level->currentBasicEnemy))->body.width = 10;
    ((level->basicEnemy)+(level->currentBasicEnemy))->body.height = 20;
    ((level->basicEnemy)+(level->currentBasicEnemy))->body.center.x = x;
    ((level->basicEnemy)+(level->currentBasicEnemy))->body.center.y = y;
    ((level->basicEnemy)+(level->currentBasicEnemy))->body.center.z = 0;
    ((level->basicEnemy)+(level->currentBasicEnemy))->velocity.x = 0.5;
    ((level->basicEnemy)+(level->currentBasicEnemy))->leftBoundary = left;
    ((level->basicEnemy)+(level->currentBasicEnemy))->rightBoundary = right;
    level->currentBasicEnemy++;

}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "CSUB Professor Smash");
}

void cleanupXWindows(void) {
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
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

void init_opengl(void)
{
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
#define rnd() (float) rand() / (float)RAND_MAX

int checkMoveKey(XEvent *e, Game * game){

    if (e->type == KeyPress){
        int key = XLookupKeysym(&e->xkey,0);
        if (key == XK_Left){
            leftPressed = 1;
        }
        if (key == XK_Right){
            rightPressed = 1;
        }
        if ((key == XK_w || key == XK_space) && jumpFinished == 1){
            jumpInitiated = 1;
            jumpFinished = 0;
        }
        if (key == XK_e && shootPressed == 0){
            shootPressed = 5;
        }

    }
    if (e->type == KeyRelease){
        int key = XLookupKeysym(&e->xkey,0);
        if ( key == XK_Left){
            leftPressed = 0;
        }
        if ( key == XK_Right){
            rightPressed = 0;
        }
    }
    return 0;

}

int checkGameQuit(XEvent *e, Game *game)
{
    //IF esc key is pressed return 1 to break the loop in main and terminate
    //the program
    if (e->type == KeyPress) {
        int key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_Escape) {
            return 1;
        }

    }
    return 0;
}
void createBullet(Game *game){
    Shape * body = &game->hero->body;
    if (game->hero->numBullets < game->hero->maxBullets){
        game->hero->bullet[game->hero->numBullets].center.x = body->center.x;
        game->hero->bullet[game->hero->numBullets].center.y = body->center.y;
        game->hero->bullet[game->hero->numBullets].velocity.x = 10 * game->hero->facing;
        game->hero->numBullets++;
    }

}
void movement(Game *game)
{
    //Check what keys are pressed
    numCollisions = 0;
    if (rightPressed == 1){
        game->hero->body.center.x += 3;
        game->hero->facing = 1;
    }
    if (leftPressed == 1){
        game->hero->body.center.x += -3;
        game->hero->facing = -1;
    }
    if (shootPressed == 5){
        createBullet(game);
        shootPressed--;
    }
    if (shootPressed > 0 && shootPressed < 5){
        shootPressed--;
    }

    if (jumpInitiated == 1){
        game->hero->velocity.y = 8;
        jumpInitiated = 0;
    }
 

    Shape *body = &game->hero->body;
    Shape *prevPosition = &game->hero->prevPosition;
    prevPosition->center.x = body->center.x;
    prevPosition->center.y = body->center.y;
    
    game->hero->body.center.y += game->hero->velocity.y;
    game->hero->velocity.y += GRAVITY;
    Enemy * enemy;

    // Move all bullets and check collision with "Basic Enemies"    
    for (int j = 0; j < game->hero->numBullets; j++){
        game->hero->bullet[j].center.x += game->hero->bullet[j].velocity.x;
        for (int k = 0; k < game->level->numBasicEnemies; k++){
            enemy = (game->level->basicEnemy)+k;
            if (game->hero->bullet[j].center.x + game->hero->bullet[j].width >= 
                    enemy->body.center.x - enemy->body.width &&
                game->hero->bullet[j].center.x - game->hero->bullet[j].width <= 
                    enemy->body.center.x + enemy->body.width &&
                game->hero->bullet[j].center.y + game->hero->bullet[j].height < 
                    enemy->body.center.y + enemy->body.height &&
                game->hero->bullet[j].center.y + game->hero->bullet[j].height > 
                    enemy->body.center.y - enemy->body.height)
            {
                enemy->body.center.y = -50;
            }     
            
        }
        if (game->hero->bullet[j].center.x > WINDOW_WIDTH || game->hero->bullet[j].center.x < 0){
            memcpy(&game->hero->bullet[j],&game->hero->bullet[game->hero->numBullets-1],sizeof(Shape));
            game->hero->numBullets--;
        }

    }

    //check collision between the Hero and platforms.
    //also sets the jumpInitiated and jumpFinised variables to prevent
    Shape * platform;
    for (int i = 0; i < game->level->numPlatforms; i++){
        platform = (game->level->platform)+i;
        if (body->center.x + body->width >= platform->center.x - platform->width &&
                body->center.x - body->width <= platform->center.x + platform->width &&
                body->center.y - body->height < platform->center.y + platform->height &&
                body->center.y + body->height > platform->center.y - platform->height )
        {
            if (prevPosition->center.x  < platform->center.x - platform->width)
            {
                body->center.x = platform->center.x - platform->width - body->width;
            }
            else{
                if (prevPosition->center.x  > platform->center.x + platform->width)
                {
                    body->center.x = platform->center.x + platform->width + body->width;
                }
                else{
                    if (prevPosition->center.y < body->center.y ||
                            body->center.y < platform->center.y){
                        game->hero->velocity.y = -2;
                    }
            
                    else{
                    body->center.y = platform->center.y + platform->height + body->height;
                    game->hero->velocity.y = -1;
                    jumpFinished = 1;
                    jumpInitiated = 0;
                    numCollisions++;
                    }
                }
            }
        }
        
    }

    // if numCollisions is set to 0 then this means that the character is falling
    // off of a platform and therefore we set jumpFinished to 0 so that he cannot 
    // jump while in this falling state. We also set jumpInitiated to 0 so that
    // he doenst jump instantly when he hits the ground (I think)
    if (numCollisions == 0){
        jumpFinished = 0;
        jumpInitiated = 0;
    }

    // This next bit of code handles transitioning between levels
    if (body->center.x > WINDOW_WIDTH){
        body->center.x = 0;
        currentHorizontalLevel = game->level->horizontalPosition + 1;
        game->level = level[currentHorizontalLevel][currentVerticalLevel];
        game->hero->numBullets = 0;
    }
    if (body->center.x < 0){
        body->center.x = WINDOW_WIDTH-1;
        currentHorizontalLevel = game->level->horizontalPosition - 1;
        game->level = level[currentHorizontalLevel][currentVerticalLevel];
        game->hero->numBullets = 0;
    }
    if (body->center.y > WINDOW_HEIGHT){
        body->center.y = 0;
        currentVerticalLevel = game->level->verticalPosition + 1;
        game->level = level[currentHorizontalLevel][currentVerticalLevel];
        game->hero->numBullets = 0;
    }
    if (body->center.y < 0){
        body->center.y = WINDOW_HEIGHT;
        currentVerticalLevel = game->level->verticalPosition - 1;
        game->level = level[currentHorizontalLevel][currentVerticalLevel];
        game->hero->numBullets = 0;
    }
   
    // check collision between the hero and "basic enemies"
    for (int k = 0; k < game->level->numBasicEnemies; k++){ 
        enemy = (game->level->basicEnemy)+k;
        if (body->center.x + body->width >= enemy->body.center.x - enemy->body.width &&
            body->center.x - body->width <= enemy->body.center.x + enemy->body.width &&
            body->center.y - body->height < enemy->body.center.y + enemy->body.height &&
            body->center.y + body->height > enemy->body.center.y - enemy->body.height )
        {
            body->center.x = 400;
            body->center.y = 250;
        }
    }
    
    
    for (int k = 0; k < game->level->numBasicEnemies; k++){
        enemy = (game->level->basicEnemy)+k;
        if (enemy->body.center.x > enemy->rightBoundary){
            enemy->velocity.x = -0.5;
        }
        if (enemy->body.center.x < enemy->leftBoundary){
            enemy->velocity.x = 0.5;
        }
        enemy->body.center.x += enemy->velocity.x;
    }

}

// This function is used to draw a circle in openGL. I grabbed it
// off of the web
void DrawCircle(float cx, float cy, float r, int num_segments) 
{
    glPushMatrix(); 
    float theta = 2 * 3.1415926 / float(num_segments); 
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = r;//we start at angle = 0 
    float y = 0; 

    glBegin(GL_LINE_LOOP); 
    for(int ii = 0; ii < num_segments; ii++) 
    { 
        glVertex2f(x + cx, y + cy);//output vertex 

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    } 
    glEnd(); 
    glPopMatrix();
}

void render(Game *game)
{

    glClear(GL_COLOR_BUFFER_BIT);
    float w, h;

    // Draw the Hero to the screen
    glColor3ub(200,200,200);
    glPushMatrix();
    glTranslatef(game->hero->body.center.x, game->hero->body.center.y, game->hero->body.center.z);
    w = game->hero->body.width;
    h = game->hero->body.height;
    glBegin(GL_QUADS);
    glVertex2i(-w,-h); 
    glVertex2i(-w,h); 
    glVertex2i(w,-h); 
    glVertex2i(w,h); 
    glEnd();
    glPopMatrix();


    // Draw all Basic Enemies for the current level to the screen
    Shape * basicEnemy;
    glColor3ub(150,100,100);
    for (int i = 0; i < game->level->numBasicEnemies; i++){
        basicEnemy = &((game->level->basicEnemy)+i)->body;

        glPushMatrix();
        glTranslatef(basicEnemy->center.x, basicEnemy->center.y, basicEnemy->center.z);
        w = basicEnemy->width;
        h = basicEnemy->height;
        glBegin(GL_QUADS);
        glVertex2i(-w,-h);
        glVertex2i(-w, h);
        glVertex2i( w, h);
        glVertex2i( w,-h);
        glEnd();
        glPopMatrix();
    }

    // Draw all Platforms for hte current level to the screen    
    Shape * platform;
    glColor3ub(90,140,90);
    for(int i =0; i < game->level->numPlatforms; i++){
        platform = (game->level->platform)+i;
        
        glPushMatrix();
        glTranslatef(platform->center.x, platform->center.y, platform->center.z);
        w = platform->width;
        h = platform->height;
        glBegin(GL_QUADS);
        glVertex2i(-w,-h);
        glVertex2i(-w, h);
        glVertex2i( w, h);
        glVertex2i( w,-h);
        glEnd();

        glPopMatrix();
    }

    // Draw all Bullets for the current level to the screen. Should be
    // at most 3.
    glColor3ub(250,250,250);
    for (int i = 0; i < game->hero->numBullets; i++){
        glPushMatrix();
        glTranslatef(game->hero->bullet[i].center.x, game->hero->bullet[i].center.y, 0);
        w = game->hero->bullet[i].width;
        h = game->hero->bullet[i].height;
        glBegin(GL_QUADS);
        glVertex2i(-w,-h);
        glVertex2i(-w,h);
        glVertex2i( w,-h);
        glVertex2i( w,h);
        glEnd();
        glPopMatrix();
    }
}

