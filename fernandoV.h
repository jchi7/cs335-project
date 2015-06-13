#ifndef AUDIO_H
#define AUDIO_H

#ifdef ISMAC
    #include <OpenAL/al.h>
    #include <OpenAL/alc.h>
#else
    #include <AL/al.h>
    #include <AL/alc.h>
#endif
#include <AL/alut.h>
void init_openal();
void playJump();
void playDeath();
void playMenuMusic();
void playGameMusic();
void stopMenuMusic();
void stopGameMusic();
void playSavePoint();
void playLaser();
void playStomp();
void playEnemyDeath();
void playSpikeDeath();
void playcrow();
void closeDevices();
#endif
