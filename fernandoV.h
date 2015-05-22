
#ifndef AUDIO_H
#define AUDIO_H
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AL/alut.h>
void initShit();
void playJump();
void playDeath();
void playMenuMusic();
void playGameMusic();
void stopMenuMusic();
void stopGameMusic();
void playSavePoint();
#endif
