#ifndef AUDIO_H
#define AUDIO_H
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
void initShit();
void playJump();
void playDeath();
void playMenuMusic();
void playGameMusic();
void stopMenuMusic();
void stopGameMusic();
void playSavePoint();
void playLaser();
#endif
