#include "fernandoV.h"
#include <iostream>
#define FILENAME1 "sounds/menu_music.wav"
#define FILENAME2 "sounds/grunt.wav"
#define FILENAME3 "sounds/jump.wav"
using namespace std;
//global var//buffer is menu
//buffer2 grunt
//buffer3 jump
	ALuint buffer,buffer2,buffer3,source, source2,source3;
	ALint state;

void initOpenal() {
	//initialize
	alutInit(0,0);	
	alGetError();
}

//content functions
void createContent() {
	buffer = alutCreateBufferFromFile(FILENAME1);
	

}
void createContentGrunt() {

	buffer2 = alutCreateBufferFromFile(FILENAME2);


}

void createContentJump() {

	buffer3 = alutCreateBufferFromFile(FILENAME3);


}



//sound functions
void playSound() {
	//cout << "debugPLAY\n";
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	//playsound
	alSourcePlay(source);	
	//wait for song to complete	
	alGetSourcei(source, AL_SOURCE_STATE, &state);


}

void playGruntSound() {
	//cout << "debugPLAY\n";
	alGenSources(1, &source2);
	alSourcei(source2, AL_BUFFER, buffer2);
	//playsound
	alSourcePlay(source2);
	//wait for song to complete	
	alGetSourcei(source2, AL_SOURCE_STATE, &state);


}
//jump sound
void playJumpSound() {
	//init openal
	alutInit(0,0);
	//create 	
	buffer3 = alutCreateBufferFromFile(FILENAME3);
	alGenSources(1, &source3);
	alSourcei(source3, AL_BUFFER, buffer3);
	//playsound
	alSourcePlay(source3);
	//wait for song to complete	
	alGetSourcei(source3, AL_SOURCE_STATE, &state);
	//loop for game
	//alSourcei(source3, AL_LOOPING, AL_TRUE);
	//test
	//noway----clearBuffer3();




}



void clearBuffer() {
	//for menu

	alDeleteSources(1, &source);
	alDeleteBuffers(1,&buffer);
	alutExit();


}
void clearBuffer2() {

//AudioFileClose(FILENAME1); did not work
//for grunt

	alDeleteSources(1, &source2);
	alDeleteBuffers(1,&buffer2);
	alutExit();
}

void clearBuffer3() {

//AudioFileClose(FILENAME1); did not work
//for grunt

	alDeleteSources(1, &source3);
	alDeleteBuffers(1,&buffer3);
	//alutExit();
}
