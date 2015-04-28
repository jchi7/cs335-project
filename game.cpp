#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "game.h"
using namespace std;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

Game::Game()
{
    this->leftPressed = 0;
    this->rightPressed = 0;
    this->shootPressed = 0;
    this->currentHorizontalLevel = 3;
    this->currentVerticalLevel = 1;
    this->totalHorizontal = 20;
    this->totalVertical = 5;
    this->state = MAIN_MENU;
    this->isPlatformMovable = false;
    this->isPlatformResizable = false;
    this->movablePlatformIndex = 0;
    this->resizablePlatformIndex = 0;
    this->resizablePlatformX = 0;
    this->resizablePlatformY = 0;
    this->textureHeight = 15;
    this->textureWidth = 15;
    initLevel();
    fillLevel();
}

Game::~Game()
{
    delete hero;
    //dtor
}

void Game::checkRoom()
{
    if (hero->body.center[0] > WINDOW_WIDTH){
        currentHorizontalLevel++;
        hero->body.center[0] = 0 + hero->body.width;
    }
    if (hero->body.center[0] < 0){
        currentHorizontalLevel--;
        hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
    }
    if (hero->body.center[1] > WINDOW_HEIGHT){
        currentVerticalLevel++;
        hero->body.center[1] = 0 + hero->body.height;
    }
    if (hero->body.center[1] < 0){
        currentVerticalLevel--;
        hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
    }

}

Room * Game::getRoomPtr()
{
    return &level[currentVerticalLevel][currentHorizontalLevel];
}

void Game::moveRoomLeft()
{
    if (currentHorizontalLevel > 0)
        currentHorizontalLevel--;
}

void Game::moveRoomRight()
{
    if (currentHorizontalLevel < 19)
        currentHorizontalLevel++;
}

void Game::moveRoomUp()
{
    if (currentVerticalLevel < 4)
        currentVerticalLevel++;
}

void Game::moveRoomDown()
{
    if (currentVerticalLevel > 0 )
        currentVerticalLevel--;
}
void Game::resizePlatform(GameObject * mouse)
{
    Room * room = this->getRoomPtr();
    int mouseX = (int)mouse->body.center[0];
    int mouseY = (int)mouse->body.center[1];
    int height = (( ( mouseY - this->resizablePlatformY) / ( this->textureHeight)) * this->textureHeight + this->textureHeight);
    int width = (( ( mouseX - this->resizablePlatformX) / ( this->textureWidth)) * this->textureWidth + this->textureWidth);

    if ( width <= 0){
        room->objects[this->resizablePlatformIndex]->body.width = this->textureWidth;
    }
    else{
        room->objects[this->resizablePlatformIndex]->body.width = width;
    }
    if ( height <= 0){
        room->objects[this->resizablePlatformIndex]->body.height = this->textureHeight;
    }
    else{
        room->objects[this->resizablePlatformIndex]->body.height = height;
    }
        
}
void Game::initLevel()
{
    std::vector<Room> tempRow;
    for (int vertical = 0; vertical < 6; vertical++) {
        for (int horizontal = 0; horizontal < 20; horizontal++) {
            tempRow.push_back(Room());
        }
        level.push_back(tempRow);
        tempRow.clear();
    }
}

void Game::fillLevel()
{
    string line, roomType;
    string filename = "Rooms/room";
    ifstream file;
    int convVal[4]; //four
    const int pathsize = filename.length();
    char roomNum[] = "0000";

    // Room file format:
    // file name: roomCCRR.txt, RR = row number, CC = col number
    // line:  (int)width,(int)height,(int)center-x,(int)center-y,(str)type  

    filename.append(roomNum);
    filename.append(".txt");
    for (int vert = 0; vert < 5; vert++) {
        for (int horz = 0; horz < 20; horz++) {
            // remove previous room number
            filename.erase(pathsize,4);

            // increment room numbers
            roomNum[0] = (char)((horz/10) + 48);
            roomNum[1] = (char)((horz%10) + 48);
            roomNum[2] = (char)((vert/10) + 48);
            roomNum[3] = (char)((vert%10) + 48);

            // insert new room number
            filename.insert(pathsize,roomNum);

            file.open(filename.c_str());
            if (!file.is_open()) {
                cout << "Error: Could not open input file '" << filename << "'\n";
                continue;
            }
            else {
                cout << "Reading: " << filename << endl;
            }
            while (getline(file, line)) {

                if (!file.good()) {
                    break;
                }

                stringstream iss(line);

                // read a single platform
                for (int col = 0; col < 5; ++col) {
                    string val;
                    getline(iss, val, ',');

                    stringstream converter(val);
                    if (col != 4)
                        converter >> convVal[col];
                    else
                        roomType = converter.str();
                }

                // create platform
                level[vert][horz].objects.push_back(new Platform(convVal[0], convVal[1], convVal[2], convVal[3], roomType.c_str()));
                level[vert][horz].numPlatforms++;
cout << "Created [" << vert << "][" << horz <<"]\n";
            }
            file.close();
        }
    }
}

void Game::saveRooms()
{
    string line, roomType;
    string filename = "Rooms/room";
    ofstream file;
    int convVal[4]; //four
    const int pathsize = filename.length();
    char roomNum[] = "0000";

    // Room file format:
    // file name: roomCCRR.txt, RR = row number, CC = col number
    // line:  (int)width,(int)height,(int)center-x,(int)center-y,(str)type  

    filename.append(roomNum);
    filename.append(".txt");
    for (int vert = 0; vert < 1; vert++) {
        for (int horz = 0; horz < 1; horz++) {
            // remove previous room number
            filename.erase(pathsize,4);

            // increment room numbers
            roomNum[0] = (char)((horz/10) + 48);
            roomNum[1] = (char)((horz%10) + 48);
            roomNum[2] = (char)((vert/10) + 48);
            roomNum[3] = (char)((vert%10) + 48);

            // insert new room number
            filename.insert(pathsize,roomNum);

            file.open(filename.c_str(),std::ofstream::out);
            if (!file.is_open()) {
                cout << "Error: Could not open input file '" << filename << "'\n";
                continue;
            }
            else {
                cout << "Writing: " << filename << endl;
            }

            for (int i = 0; i < level[vert][horz].objects.size(); i++){

                convVal[0] = level[vert][horz].objects[i]->body.width;
                convVal[1] = level[vert][horz].objects[i]->body.height;
                convVal[2] = level[vert][horz].objects[i]->body.center[0];
                convVal[3] = level[vert][horz].objects[i]->body.center[1];
  
                file << convVal[0] << "," << convVal[1] << "," << convVal[2] << "," << convVal[3] << ","
                    << "GROUND" << '\n';

            
            }
            file.close();
        }
    }
}

