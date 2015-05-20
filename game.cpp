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

void Game::setSavePoint(int index){
    this->savePointHorizontalRoom = this->currentHorizontalLevel;
    this->savePointVerticalRoom = this->currentVerticalLevel;
    this->savePointIndex = index;
}

void Game::respawnAtSavePoint(){
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
        currentHorizontalLevel++;
        hero->body.center[0] = 0 + hero->body.width;
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[0] < 0){
        currentHorizontalLevel--;
        hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] > WINDOW_HEIGHT){
        currentVerticalLevel++;
        hero->body.center[1] = 0 + hero->body.height;
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] < 0){
        currentVerticalLevel--;
        hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
        // DEBUG:
        cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
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
    GameObject * platform = room->platforms[this->resizablePlatformIndex];
    int mouseX = (int)mouse->body.center[0];
    int mouseY = (int)mouse->body.center[1];
    int height = (( ( mouseY - this->resizablePlatformY) / ( platform->textureHeight)) * platform->textureHeight + platform->textureHeight);
    int width = (( ( mouseX - this->resizablePlatformX) / ( platform->textureWidth)) * platform->textureWidth + platform->textureWidth);

    if ( width <= 0){
        platform->body.width = platform->textureWidth;
    }
    else{
        platform->body.width = width;
        platform->horizontalTiles = platform->body.width / platform->textureWidth;
    }
    if ( height <= 0){
        platform->body.height = platform->textureHeight;
    }
    else{
        platform->body.height = height;
        platform->verticalTiles = platform->body.height / platform->textureHeight;
    }
        
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
    int spikeOrientation = 0;

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
                // read a single savePoint
                else if (objType == "SAVEPOINT") {
                    float convVal[4]; //four
                    for (int col = 0; col < 4; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }

                    // create savePoint
                    level[vert][horz].savePoints.push_back(new SavePoint(convVal[0], convVal[1], convVal[2], convVal[3]));
                    level[vert][horz].numSavePoints++;
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
                    getline(iss, val, ',');
                    stringstream converter(val);
                    converter >> spikeOrientation;
                    
                    for (int setZCoord = 0; setZCoord < 3; setZCoord++) {
                        spikePts[setZCoord][2] = 0;
                    }
                    if (spikeOrientation == 0)
                        level[vert][horz].spikes.push_back(new Spike(spikePts,FACING_UP));
                    if (spikeOrientation == 1)
                        level[vert][horz].spikes.push_back(new Spike(spikePts,FACING_LEFT));
                    if (spikeOrientation == 2)
                        level[vert][horz].spikes.push_back(new Spike(spikePts,FACING_DOWN));
                    if (spikeOrientation == 3)
                        level[vert][horz].spikes.push_back(new Spike(spikePts,FACING_RIGHT));
                    level[vert][horz].numSpikes++;
                    // DEBUG:
//                    cout << "Created spike in [" << vert << "][" << horz <<"]: " << vecPrint(spikePts[0]) << ", " << vecPrint(spikePts[1]) << ", " << vecPrint(spikePts[2]) << ", " << spikeOrientation << endl;
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
                }}
            file.close();
        }
    }
    cout << level[1][3].savePoints[0]->body.width << endl;
}

void Game::saveRooms()
{
    string line, roomType;
    string filename = "Rooms/room";
    ofstream file;
    int convVal[4]; //four
    int spikeVal[7]; //four


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
        return;
    }
    else {
        cout << "Writing: " << filename << endl;
    }

    for (unsigned int i = 0; i < level[vert][horz].platforms.size(); i++){

        convVal[0] = level[vert][horz].platforms[i]->body.width;
        convVal[1] = level[vert][horz].platforms[i]->body.height;
        convVal[2] = level[vert][horz].platforms[i]->body.center[0];
        convVal[3] = level[vert][horz].platforms[i]->body.center[1];

        file << "GROUND," << convVal[0] << "," << convVal[1] << "," << convVal[2] << "," << convVal[3] << "\n";
    }
    for (unsigned int i = 0; i < level[vert][horz].savePoints.size(); i++){

        convVal[0] = level[vert][horz].savePoints[i]->body.width;
        convVal[1] = level[vert][horz].savePoints[i]->body.height;
        convVal[2] = level[vert][horz].savePoints[i]->body.center[0];
        convVal[3] = level[vert][horz].savePoints[i]->body.center[1];

        file << "SAVEPOINT," << convVal[0] << "," << convVal[1] << "," << convVal[2] << "," << convVal[3] << "\n";
    }

    for (unsigned int i = 0; i < level[vert][horz].spikes.size(); i++){

        spikeVal[0] = level[vert][horz].spikes[i]->body.corners[0][0];
        spikeVal[1] = level[vert][horz].spikes[i]->body.corners[0][1];
        spikeVal[2] = level[vert][horz].spikes[i]->body.corners[1][0];
        spikeVal[3] = level[vert][horz].spikes[i]->body.corners[1][1];
        spikeVal[4] = level[vert][horz].spikes[i]->body.corners[2][0];
        spikeVal[5] = level[vert][horz].spikes[i]->body.corners[2][1];
        if ( level[vert][horz].spikes[i]->body.orientation == FACING_UP){
            spikeVal[6] = 0;
        }
        if ( level[vert][horz].spikes[i]->body.orientation == FACING_LEFT){
            spikeVal[6] = 1;
        }
        if ( level[vert][horz].spikes[i]->body.orientation == FACING_DOWN){
            spikeVal[6] = 2;
        }
        if ( level[vert][horz].spikes[i]->body.orientation == FACING_RIGHT){
            spikeVal[6] = 3;
        }

        file << "SPIKE," << spikeVal[0] << "," << spikeVal[1] << "," << spikeVal[2] << "," << spikeVal[3]
            << "," << spikeVal[4] << "," << spikeVal[5] << "," << spikeVal[6] << "\n";
    }
    file.close();
    return;
}

void Game::heroShoots() {
    Room* current_level = getRoomPtr();
    if (hero->body.orientation == FACING_RIGHT) {
        printf("Before bullet %d\n", current_level->numBullet);
        current_level->bullet.push_back(new BasicBullet(4, 0, hero->body.center[0] + hero->body.width + 2, hero->body.center[1], HERO));
        current_level->numBullet++;
        printf("After bullet %d\n", current_level->numBullet);
    } else {
        current_level->bullet.push_back(new BasicBullet(-4, 0, hero->body.center[0] - hero->body.width - 2, hero->body.center[1], HERO));
        current_level->numBullet++;
    }
}
