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
    currentHorizontalLevel--;
}

void Game::moveRoomRight()
{
    currentHorizontalLevel++;
}

void Game::moveRoomUp()
{
    currentVerticalLevel++;
}

void Game::moveRoomDown()
{
    currentVerticalLevel--;
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
    for (int vert = 0; vert < 6; vert++) {
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


