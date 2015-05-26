#include "fernandoV.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <AL/alut.h>
#define FILENAME "sounds/black crows.wav"

#define TEST_ERROR(_msg)        \
    error = alGetError();       \
    if (error != AL_NO_ERROR) { \
        fprintf(stderr, _msg "\n"); \
        return;      \
    }

#define MAX_SOURCES 10

using namespace std;
    ALboolean enumeration;
    const ALCchar *devices;
    const ALCchar *defaultDeviceName;
    int ret;
    char *bufferData;
    ALCdevice *device;
    ALvoid *data;
    ALCcontext *context;
    ALsizei size, freq;
    ALenum format;
    ALuint buffer, source;
    ALuint sources[MAX_SOURCES][2];
    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    ALboolean loop = AL_FALSE;
    ALCenum error;
    ALint source_state;

void initShit(){


    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE)
        fprintf(stderr, "enumeration extension not available\n");

//    list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

    if (!defaultDeviceName)
        defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    device = alcOpenDevice(defaultDeviceName);
    if (!device) {
        fprintf(stderr, "unable to open default device\n");
        return;
    }

    fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

    alGetError();

    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context)) {
        fprintf(stderr, "failed to make default context\n");
        return;
    }
    TEST_ERROR("make default context");

    /* set orientation */
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    TEST_ERROR("listener position");
        alListener3f(AL_VELOCITY, 0, 0, 0);
    TEST_ERROR("listener velocity");
    alListenerfv(AL_ORIENTATION, listenerOri);
    TEST_ERROR("listener orientation");

    for (int i = 0; i < MAX_SOURCES; i++){
        alGenSources((ALuint)1, &sources[i][0]);
        TEST_ERROR("source generation");

        alSourcef(sources[i][0], AL_PITCH, 1);
        TEST_ERROR("source pitch");
        alSourcef(sources[i][0], AL_GAIN, 1);
        TEST_ERROR("source gain");
        alSource3f(sources[i][0], AL_POSITION, 0, 0, 0);
        TEST_ERROR("source position");
        alSource3f(sources[i][0], AL_VELOCITY, 0, 0, 0);
        TEST_ERROR("source velocity");
        alSourcei(sources[i][0], AL_LOOPING, AL_FALSE);
        TEST_ERROR("source looping");
        alGenBuffers(1, &sources[i][1]);
        TEST_ERROR("buffer generation");
    }




    alutLoadWAVFile((ALbyte *)"sounds/jump.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");

    alBufferData(sources[0][1], format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(sources[0][0], AL_BUFFER, sources[0][1]);
    TEST_ERROR("buffer binding");
	////////////

    alSourcef(sources[1][0], AL_GAIN, 0.1);
    alutLoadWAVFile((ALbyte *)"sounds/grunt.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");

    alBufferData(sources[1][1], format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(sources[1][0], AL_BUFFER, sources[1][1]);
    TEST_ERROR("buffer binding");
	////////////

    alSourcef(sources[2][0], AL_GAIN,.20) ;
    alutLoadWAVFile((ALbyte *)"sounds/Opening.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");

    alBufferData(sources[2][1], format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(sources[2][0], AL_BUFFER, sources[2][1]);
    TEST_ERROR("buffer binding");
	///////////
    
    alSourcef(sources[3][0], AL_GAIN, 0.05);
    alutLoadWAVFile((ALbyte *)"sounds/gameSound.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");

    alBufferData(sources[3][1], format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(sources[3][0], AL_BUFFER, sources[3][1]);
    TEST_ERROR("buffer binding");
	//////////
    alSourcef(sources[4][0], AL_GAIN, 0.05);
    alutLoadWAVFile((ALbyte *)"sounds/savepoint.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");

    alBufferData(sources[4][1], format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(sources[4][0], AL_BUFFER, sources[4][1]);
    TEST_ERROR("buffer binding");
	//////////	
    alSourcef(sources[5][0], AL_GAIN, 0.1);
    alutLoadWAVFile((ALbyte *)"sounds/laser.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");

    alBufferData(sources[5][1], format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(sources[5][0], AL_BUFFER, sources[5][1]);
    TEST_ERROR("buffer binding");
	/////	
    alSourcef(sources[6][0], AL_GAIN, 0.1);
    alutLoadWAVFile((ALbyte *)"sounds/pain.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");

    alBufferData(sources[6][1], format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(sources[6][0], AL_BUFFER, sources[6][1]);
    TEST_ERROR("buffer binding");
	/////
    alSourcef(sources[7][0], AL_GAIN, 0.5);
    alutLoadWAVFile((ALbyte *)"sounds/snort.wav", &format, &data, &size, &freq,&loop);
    TEST_ERROR("loading wav file");
    alBufferData(sources[7][1], format, data, size, freq);
    TEST_ERROR("buffer copy");
    alSourcei(sources[7][0], AL_BUFFER, sources[7][1]);



}
void playJump(){
    alSourcePlay(sources[0][0]);
    TEST_ERROR("source playing");

    return;
}

void playDeath(){
    alSourcePlay(sources[1][0]);
    TEST_ERROR("source playing");
    return;
}
void playSpikeDeath(){
    alSourcePlay(sources[7][0]);
    TEST_ERROR("source playing");
    return;
}
void playEnemyDeath(){
    alSourcePlay(sources[6][0]);
    TEST_ERROR("source playing");
    return;
}
void playLaser(){
    alSourcePlay(sources[5][0]);
    TEST_ERROR("source playing");
    return;
}

void playMenuMusic(){
    
	alSourcei(sources[2][0],AL_LOOPING,AL_TRUE);
    alSourcePlay(sources[2][0]);
    TEST_ERROR("source playing");
    return;
}

void playcrow() {
	//crow
    //alSourcePlay(source);
}

void stopMenuMusic(){
    alSourceStop(sources[2][0]);
}

void playGameMusic(){
	alSourcei(sources[3][0],AL_LOOPING,AL_TRUE);
    alSourcePlay(sources[3][0]);
    TEST_ERROR("sourece playing");
}

void stopGameMusic(){
    alSourceStop(sources[3][0]);
}
void playSavePoint(){
    alSourcePlay(sources[4][0]);
    TEST_ERROR("Save point source playing");
}


//close devices 
void closeDevices() {
/* exit context */
	
 for (int i = 0; i < MAX_SOURCES; i++){
	alDeleteSources(1, &sources[i][0]);
	alDeleteBuffers(1, &sources[0][i]);
	}
	cout<<"all devices are closed\n";
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
