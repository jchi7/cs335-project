//Moises Ayala
//These functions handles the texture rendering
//All of these functions were used in main.cpp to render certain textures of our game

#include <GL/glx.h>
#include "game.h"
#include "ppm.h"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

//This Function will render a background texture on our game
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

//This function will render a part of an image file, just have to specify the dimensions of the
//Image that needs to be cropped out
void renderTexture(GLuint imageTexture, float x1,float x2,float y1, float y2, int width, int height)
{
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glPushMatrix();
    glTranslatef(500, 345,0);
    glBindTexture(GL_TEXTURE_2D,imageTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(x1,y2); glVertex2i(-width,-height);
    glTexCoord2f(x1,y1); glVertex2i(-width,height);
    glTexCoord2f(x2,y1); glVertex2i(width,height);
    glTexCoord2f(x2,y2); glVertex2i(width,-height);
    glBindTexture(GL_TEXTURE_2D,0);
    glEnd();
    glPopMatrix();
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
 
 //This function renders the sprite sheet for our hero character,
void renderHero(GLuint heroTexture,Game* game,Coordinates* heroSprite,int index,int w, int h,int mod)
{
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    
    
    glPushMatrix();
    glTranslatef(game->hero->body.center[0], game->hero->body.center[1], game->hero->body.center[2]);
    glBindTexture(GL_TEXTURE_2D,heroTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(heroSprite[index%mod].x1,heroSprite[index%mod].y2); glVertex2i(-w-5,-h-5);
    glTexCoord2f(heroSprite[index%mod].x1,heroSprite[index%mod].y1); glVertex2i(-w-5,h+5); //here
    glTexCoord2f(heroSprite[index%mod].x2,heroSprite[index%mod].y1); glVertex2i(w+5,h+5);
    glTexCoord2f(heroSprite[index%mod].x2,heroSprite[index%mod].y2); glVertex2i(w+5,-h-5);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    glEnd();
    glPopMatrix();
}

void setUpImage(GLuint texture, Ppmimage *picture)
{
    //Setting up the jumpLeft image
    int w = picture->width;
    int y = picture->height;

    glBindTexture(GL_TEXTURE_2D,texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w,y, 0,
            GL_RGB, GL_UNSIGNED_BYTE, picture->data);
}

void convertToRGBA(Ppmimage *picture) {
    int w = picture->width;
    int y = picture->height;

    unsigned char *silhouetteData = buildAlphaData(picture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, y, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    delete [] silhouetteData;
}

//This function loops throug all of the save points and will render the correct texture on them.
void renderSavePoint(GameObject * entity, int index,GLuint keyTexture,GLuint checkPointTexture,int currentSavePoint)
{
    glColor3ub(entity->rgb[0], entity->rgb[1], entity->rgb[2]);
    float w = entity->body.width;
    float h = entity->body.height;
    if(index == currentSavePoint) {
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255,255,255,255);
        glPushMatrix();
        glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
        glBindTexture(GL_TEXTURE_2D, keyTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.1f,1.0f); glVertex2i(-w,-h);
        glTexCoord2f(0.1f,0.0f); glVertex2i(-w,h);
        glTexCoord2f(1.0f,0.0f); glVertex2i(w,h);
        glTexCoord2f(1.0f,1.0f); glVertex2i(w,-h);
        glEnd();
        glPopMatrix();
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glColor4ub(255,255,255,255);
        glPushMatrix();
        glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
        glBindTexture(GL_TEXTURE_2D, checkPointTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.1f,1.0f); glVertex2i(-w,-h);
        glTexCoord2f(0.1f,0.0f); glVertex2i(-w,h);
        glTexCoord2f(1.0f,0.0f); glVertex2i(w,h);
        glTexCoord2f(1.0f,1.0f); glVertex2i(w,-h);
        glEnd();
        glPopMatrix();
    }
}
//This function will render our platform textures
void renderPlatform(GameObject * entity,GLuint *platformTextures)
{
    float w = entity -> textureWidth;
    float h = entity -> textureHeight;

    int cornerX = entity->body.center[0] - entity->body.width;
    int cornerY = entity->body.center[1] + entity -> body.height;

    glColor3ub(entity->rgb[0], entity->rgb[1], entity->rgb[2]);
    for (int row = 0; row < entity->verticalTiles; row++){
        int rowOffset = cornerY - ((row * entity->textureHeight * 2) + entity->textureHeight);

        for (int column = 0; column < entity->horizontalTiles; column++){
            //The follwoing code is to draw the platforms
            int colOffset = cornerX + (column * entity->textureWidth * 2) + entity->textureWidth;
            glEnable(GL_TEXTURE_2D);
            glColor4ub(255,255,255,255);
            glPushMatrix();
            //glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
            glTranslatef(colOffset, rowOffset, entity->body.center[2]);
            glBindTexture(GL_TEXTURE_2D, platformTextures[entity->tex_id]);
            glBegin(GL_QUADS);
            glTexCoord2f(0.1f,1.0f); glVertex2i(-w,-h);
            glTexCoord2f(0.1f,0.0f); glVertex2i(-w,h);
            glTexCoord2f(1.0f,0.0f); glVertex2i(w,h);
            glTexCoord2f(1.0f,1.0f); glVertex2i(w,-h);
            glEnd();
            glPopMatrix();
        }
    }
}

//Function will render our spike attributes
void renderSpike(GameObject * entity,GLuint spikeTexture)
{
    glColor3ub(entity->rgb[0], entity->rgb[1], entity->rgb[2]);
    glEnable(GL_TEXTURE_2D);
    glColor4ub(255,255,255,255);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, spikeTexture);
    glBegin(GL_TRIANGLES);
    //moises
    glTexCoord2f(0.1,.9); glVertex2f(entity->body.corners[0][0],entity->body.corners[0][1]);
    glTexCoord2f(.9,.9); glVertex2f(entity->body.corners[1][0],entity->body.corners[1][1]);
    glTexCoord2f(.5,.5); glVertex2f(entity->body.corners[2][0],entity->body.corners[2][1]);
    glEnd();
    glPopMatrix();
}


//This function renders the bullet for both hero and the shooter enemy
void renderBullet(GameObject * entity, int index,GLuint bulletTexture)
{
    float w = entity->body.width + 4;
    float h = entity->body.height + 4;
    glEnable(GL_TEXTURE_2D);
    glColor4ub(255,255,255,255);
    glPushMatrix();

    glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
    glBindTexture(GL_TEXTURE_2D, bulletTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(((BasicBullet*)entity)->bullet[index].x1,((BasicBullet*)entity)->bullet[index].y2);
    glVertex2i(-w,-h);
    glTexCoord2f(((BasicBullet*)entity)->bullet[index].x1,((BasicBullet*)entity)->bullet[index].y1);
    glVertex2i(-w,h);
    glTexCoord2f(((BasicBullet*)entity)->bullet[index].x2,((BasicBullet*)entity)->bullet[index].y1);
    glVertex2i(w,h);
    glTexCoord2f(((BasicBullet*)entity)->bullet[index].x2,((BasicBullet*)entity)->bullet[index].y2);
    glVertex2i(w,-h);
    glEnd();

    glPopMatrix();
    index = (index + 1)%10;
}
