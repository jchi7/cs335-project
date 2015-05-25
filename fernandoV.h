#ifndef AUDIO_H
#define AUDIO_H
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
void initOpenal();
void playJump();
void playDeath();
void playMenuMusic();
void playGameMusic();
void stopMenuMusic();
void stopGameMusic();
void playSavePoint();
void playcrow();
#endif
