// self explanatory
struct Vec {
    float x, y, z;
};

// Shape structure used to represent dimensions and position on the screen 
struct Shape {
    float width, height;
    float radius;
    Vec center;
    Vec velocity;
};

// Enemy structure holds some basic information about an Enemy.
// Right now the basic enemy uses this structure.
struct Enemy{
    Shape body;
    Shape prevPosition;
    Vec velocity;
    int facing;
    int leftBoundary;
    int rightBoundary;
};

///////////////////////////////////////////////////////////
// The Level structure holds information about the current level.
// 
// horizontalPosition: the level's x-coordinate position in the level grid
//                     (relative to other levels)
// verticalPosition: the level's y-coordinate position in the level grid 
//                   (relative to other levels)
// platform: a pointer to a platform. Platforms are rectangular shapes.
// basicEnemy: a pointer to an Enemy. Basic enemies move left and right within
//             a certain boundary.
// 
struct Level{
    int horizontalPosition;
    int verticalPosition;
    Shape * platform;
    int numPlatforms;
    int numBasicEnemies;
    int currentPlatform;
    int currentBasicEnemy;
    Enemy * basicEnemy;
    Level(int num,int numBasic){
        numPlatforms = num;
        platform = new Shape[num];
        currentPlatform = 0;
        numBasicEnemies = numBasic;
        if (numBasicEnemies > 0)
            basicEnemy = new Enemy[numBasicEnemies];
        currentBasicEnemy = 0;
    }
};


/////////////////////////////////////////////////////////////////
// The Hero structure holds information about the main character
// body: a Shape structure used to identify the hero's coords and dimensions
// prevPosition: holds the hero's previous position to his current one
// velocity: the x and y velocity of the hero
// facing: the direction the hero is facing (used for bullet direction)
// bullet[3]: and array of 3 bullets
// 
struct Hero{
    Shape body;
    Shape prevPosition;
    Vec velocity;
    int facing;
    Shape bullet[3];
    int numBullets, maxBullets, bulletVelocity;
};

// The Game structure holds information about the current level
// and the Hero (user controlled character) of the game.
struct Game {
    Level * level;
    Hero * hero;
    ~Game() {}
    Game(Level * level, Hero * hero){
            this->level = level;
            this->hero = hero;
        }
};

//Function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void movement(Game *game);
void render(Game *game);
void createPlatform(int, int, int ,int, Level*);
void createBasicEnemy(int, int, int, int, Level*);
void createBullet(Game*game);
void initializeLevels();

//External Global Variables
extern int leftPressed ;
extern int rightPressed ;
extern int shootPressed ;
extern int jumpInitiated ;
extern int jumpFinished ;
extern int numCollisions;
extern int currentHorizontalLevel;
extern int currentVerticalLevel;
extern Level ***level;
