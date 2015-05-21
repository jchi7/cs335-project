
#ifndef AUDIO_H
#define AUDIO_H
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
void initOpenal();
void createContent();
void createContentGrunt();
void createContentJump();
void playSound();
void playGruntSound();
void clearBuffer();
void clearBuffer2();
void clearBuffer3();
void playJumpSound();
#endif
