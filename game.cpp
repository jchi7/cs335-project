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
    this->savePointHorizontalRoom = 3;
    this->savePointVerticalRoom = 1;
    this->savePointIndex = 0;
    this->totalHorizontal = 20;
    this->totalVertical = 5;
    this->state = MAIN_MENU;
    
    this->isPlatformMovable = false;
    this->isPlatformResizable = false;
    this->isSpikeMovable = false;
    this->isSavePointMovable = false;
    
    this->movablePlatformIndex = 0;
    this->movableSpikeIndex = 0;
    this->movableSavePointIndex = 0;
    this->resizablePlatformIndex = 0;
    
    this->resizablePlatformX = 0;
    this->resizablePlatformY = 0;
    this->platformTextureHeight = 15;
    this->platformTextureWidth = 15;
    initLevel();
    fillLevel();
}

Game::~Game()
{
    delete hero;
    //level.erase(level.begin(), level.end());
    //dtor
}

void Game::setSavePoint(int index)
{
    this->savePointHorizontalRoom = this->currentHorizontalLevel;
    this->savePointVerticalRoom = this->currentVerticalLevel;
    this->savePointIndex = index;
}

void Game::respawnAtSavePoint()
{
    Room * currentRoom;

    this->hero->state = JUMPING;
    this->hero->leftPressed = 0;
    this->hero->rightPressed = 0;

    this->currentHorizontalLevel = this->savePointHorizontalRoom;
    this->currentVerticalLevel = this->savePointVerticalRoom;
    currentRoom = getRoomPtr();
    this->hero->body.center[0] = currentRoom->savePoints[this->savePointIndex]->body.center[0];
    this->hero->body.center[1] = currentRoom->savePoints[this->savePointIndex]->body.center[1];
}

void Game::checkRoom()
{
    if (hero->body.center[0] > WINDOW_WIDTH){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable){
            hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
        }
        else{
            currentHorizontalLevel++;
            hero->body.center[0] = 0 + hero->body.width;
        }
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[0] < 0){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable){
            hero->body.center[0] = 0 + hero->body.width;
        }
        else{
            currentHorizontalLevel--;
            hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
        }
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] > WINDOW_HEIGHT){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable){
            hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
        }
        else {
            currentVerticalLevel++;
            hero->body.center[1] = 0 + hero->body.height;
        }
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] < 0){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable){
            hero->body.center[1] = 0 +  hero->body.height;
        }
        else {
            currentVerticalLevel--;
            hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
        }
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }

}

Room * Game::getRoomPtr()
{
    return &level[currentVerticalLevel][currentHorizontalLevel];
}

GameObject * Game::getWorkingPlatformPtr()
{
    if (!this->isPlatformMovable && !this->isPlatformResizable) {
        cout << "ERROR getWorkingPlatformPtr: platform not resizeable or movable\n";
        return NULL;
    }
    Room * room = this->getRoomPtr();
    return room->platforms[this->resizablePlatformIndex];
}

GameObject * Game::getWorkingSpikePtr()
{
    if (!this->isSpikeMovable) {
        cout << "ERROR getWorkingSpikePtr: spike not movable\n";
        return NULL;
    }
    Room * room = this->getRoomPtr();
    return room->spikes[this->movableSpikeIndex];
}

vector<GameObject*> * Game::getPlatformsVPtr()
{
    Room * room = this->getRoomPtr();
    return &(room->platforms);
}

vector<GameObject*> * Game::getSpikesVPtr()
{
    Room * room = this->getRoomPtr();
    return &(room->spikes);
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
    ShapeOrientation orientation;

    // Room file format:
    // file name: roomCCRR.txt, RR = row number, CC = col number
    // lines:  (str)type,<data values for type>

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
                    float convVal[4];
                    for (int col = 0; col < 4; ++col) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }
                    // create platform
                    level[vert][horz].platforms.push_back(new Platform(convVal[0], convVal[1], convVal[2], convVal[3]));
                    level[vert][horz].numPlatforms++;
                    // DEBUG:
//                    cout << "Created platform in " << "[" << vert << "][" << horz <<"]\n";
                }
                // read a single savePoint
                else if (objType == "SAVEPOINT") {
                    float convVal[4];
                    for (int col = 0; col < 4; ++col) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }

                    // create savePoint
                    level[vert][horz].savePoints.push_back(new SavePoint(convVal[0], convVal[1], convVal[2], convVal[3]));
                    level[vert][horz].numSavePoints++;
                }
                // read a single spike
                else if (objType == "SPIKE") {
                    Vec spikePts[3];
                    int spikeFacing;
                    for (int col = 0; col < 6; ++col) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        // DEBUG:
                        //cout << converter.str() << endl;
                        converter >> spikePts[col/2][col%2];
                    }
                    for (int i = 0; i < 3; ++i) {
                        spikePts[i][2] = 0;
                    }
                    getline(iss, val, ',');
                    stringstream converter(val);
                    converter >> spikeFacing;
                    switch (spikeFacing) {
                        case 0:
                            orientation = FACING_UP;
                            break;
                        case 1:
                            orientation = FACING_LEFT;
                            break;
                        case 2:
                            orientation = FACING_DOWN;
                            break;
                        case 3:
                            orientation = FACING_RIGHT;
                            break;
                        case 4:
                        default:
                            cout << "Error: could not read spike orientation\n";
                    }
                    level[vert][horz].spikes.push_back(new Spike(spikePts,orientation));
                    level[vert][horz].numSpikes++;
                    // DEBUG:
//                    cout << "Created spike in " << "[" << vert << "][" << horz <<"]: " << vecPrint(spikePts[0]) << ", " << vecPrint(spikePts[1]) << ", " << vecPrint(spikePts[2]) << ", " << orientation << endl;
                }
                else if (objType == "ENEMY") {
                    float convVal[4]; //four
                    for (int col = 0; col < 4; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }
                    // create Enemy
                    level[vert][horz].enemies.push_back(new BasicEnemy(convVal[0], convVal[1], convVal[2], convVal[3]));
                    level[vert][horz].numBasicEnemies++;
                }
                else if (objType == "SHOOTER") {
                    float convVal[4]; //four
                    for (int col = 0; col < 4; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }
                    // create Enemy
                    level[vert][horz].enemies.push_back(new ShooterEnemy(convVal[0], convVal[1], convVal[2], convVal[3]));
                    level[vert][horz].numBasicEnemies++;
                }
            }
            file.close();
        }
    }
    // DEBUG:
//    cout << level[1][3].savePoints[0]->body.width << endl;
}

void Game::saveRooms()
{
    string line, roomType;
    string filename = "Rooms/room";
    ofstream file;

    const int pathsize = filename.length();
    char roomNum[] = "0000";

    // Room file format:
    // file name: roomCCRR.txt, RR = row number, CC = col number
    // line:  (int)width,(int)height,(int)center-x,(int)center-y,(str)type  

    filename.append(roomNum);
    filename.append(".txt");
    int vert = currentVerticalLevel;
    int horz = currentHorizontalLevel;
    // remove previous room number
    filename.erase(pathsize,4);

    // create actual room number
    roomNum[0] = (char)((horz/10) + 48);
    roomNum[1] = (char)((horz%10) + 48);
    roomNum[2] = (char)((vert/10) + 48);
    roomNum[3] = (char)((vert%10) + 48);

    // insert new room number
    filename.insert(pathsize,roomNum);

    file.open(filename.c_str(),std::ofstream::out);
    if (!file.is_open()) {
        cout << "Error: Could not open input file '" << filename << "'\n";
        return;
    }
    else {
        cout << "Writing: " << filename << endl;
    }

    for (unsigned int i = 0; i < level[vert][horz].platforms.size(); ++i) {
        writePlatform(level[vert][horz].platforms[i], file);
    }
    for (unsigned int i = 0; i < level[vert][horz].savePoints.size(); ++i) {
        writeSavePoint(level[vert][horz].savePoints[i], file);
    }
    for (unsigned int i = 0; i < level[vert][horz].spikes.size(); ++i) {
        writeSpike(level[vert][horz].spikes[i], file);
    }
    file.close();
    return;
}

void Game::writePlatform(GameObject * platform, ofstream & outf)
{
    outf << "GROUND,"
      << platform->body.width << ","
      << platform->body.height << ","
      << platform->body.center[0] << ","
      << platform->body.center[1] << "\n";
}

void Game::writeSavePoint(GameObject * savept, ofstream & outf)
{
    outf << "SAVEPOINT,"
      << savept->body.width << ","
      << savept->body.height << ","
      << savept->body.center[0] << ","
      << savept->body.center[1] << "\n";
}

void Game::writeSpike(GameObject * spike, ofstream & outf)
{
    int spikeFacing = 0;
    switch (spike->body.orientation) {
        case FACING_UP:
            spikeFacing = 0;
            break;
        case FACING_LEFT:
            spikeFacing = 1;
            break;
        case FACING_DOWN:
            spikeFacing = 2;
            break;
        case FACING_RIGHT:
            spikeFacing = 3;
            break;
        case STOP:
        default:
            break;
    }
    outf << "SPIKE,"
      << spike->body.corners[0][0] << ","
      << spike->body.corners[0][1] << ","
      << spike->body.corners[1][0] << ","
      << spike->body.corners[1][1] << ","
      << spike->body.corners[2][0] << ","
      << spike->body.corners[2][1] << ","
      << spikeFacing << "\n";
}

void Game::heroShoots()
{
    Room* current_level = getRoomPtr();
    switch(hero->body.orientation) {
        case FACING_RIGHT:
            current_level->bullet.push_back(new BasicBullet(4, 0, hero->body.center[0] + hero->body.width + 2, hero->body.center[1], HERO));
            current_level->numBullet++;
            break;
        case FACING_LEFT:
            current_level->bullet.push_back(new BasicBullet(-4, 0, hero->body.center[0] - hero->body.width - 2, hero->body.center[1], HERO));
            current_level->numBullet++;
            break;
        default:
            break;
    }
}
