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
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[0] < 0){
        currentHorizontalLevel--;
        hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] > WINDOW_HEIGHT){
        currentVerticalLevel++;
        hero->body.center[1] = 0 + hero->body.height;
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] < 0){
        currentVerticalLevel--;
        hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
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
    for (int vertical = 0; vertical < totalVertical; vertical++) {
        for (int horizontal = 0; horizontal < totalHorizontal; horizontal++) {
            tempRow.push_back(Room());
        }
        level.push_back(tempRow);
        tempRow.clear();
    }
}

void Game::fillLevel()
{
    string line, objType;
    string filename = "Rooms/room";
    ifstream file;
    const int pathsize = filename.length();
    char roomNum[] = "0000";

    // Room file format:
    // file name: roomCCRR.txt, RR = row number, CC = col number
    // line:  (int)width,(int)height,(int)center-x,(int)center-y,(str)type  

    filename.append(roomNum);
    filename.append(".txt");
    for (int vert = 0; vert < totalVertical; vert++) {
        for (int horz = 0; horz < totalHorizontal; horz++) {
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
                // DEBUG:
                cout << "Error: Could not open input file '" << filename << "'\n";
                continue;
            }
            else {
                // DEBUG:
                //cout << "Reading: " << filename << endl;
            }

            while (getline(file, line)) {

                if (!file.good()) {
                    break;
                }

                stringstream iss(line);

                string val;
                getline (iss, val, ',');
                stringstream typeConv(val);
                objType = typeConv.str();

                // read a single platform
                if (objType == "GROUND") {
                    float convVal[4]; //four
                    for (int col = 0; col < 4; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }

                    // create platform
                    level[vert][horz].platforms.push_back(new Platform(convVal[0], convVal[1], convVal[2], convVal[3]));
                    level[vert][horz].numPlatforms++;
                    // DEBUG:
//                    cout << "Created platform in [" << vert << "][" << horz <<"]\n";
                }
                else if (objType == "SPIKE") {
                    Vec spikePts[3];
                    for (int col = 0; col < 6; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        // DEBUG:
                        //cout << converter.str() << endl;
                        converter >> spikePts[col/2][col%2];
                    }
                    for (int setZCoord = 0; setZCoord < 3; setZCoord++) {
                        spikePts[setZCoord][2] = 0;
                    }
                    level[vert][horz].spikes.push_back(new Spike(spikePts));
                    level[vert][horz].numSpikes++;
                    // DEBUG:
                    cout << "Created spike in [" << vert << "][" << horz <<"]: " << vecPrint(spikePts[0]) << ", " << vecPrint(spikePts[1]) << ", " << vecPrint(spikePts[2]) << endl;
                }
            }
            file.close();
        }
    }
}


