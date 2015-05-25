#include "fernando.h"
#include <iostream>
using namespace std;

//create_sounds class
void audio :: create_sounds() {
#ifdef USE_SOUND
	if(fmod_init()) {
		cout << "ERROR\n";
	return;
	}
	if(fmod_createsound("./menu_music.mp3", 0)) {
		cout << "ERROR\n";
	return;
	}
	fmod_setmode(0,FMOD_LOOP_OFF);
#endif
}
/*
 void audio :: music_play() {
	fmod_playsound(0);
}*/
