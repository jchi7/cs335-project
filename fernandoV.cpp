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

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <AL/alut.h>

#define TEST_ERROR(_msg)        \
    error = alGetError();       \
    if (error != AL_NO_ERROR) { \
        fprintf(stderr, _msg "\n"); \
        return;      \
    }

using namespace std;

void initShit(){
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
    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    ALboolean loop = AL_FALSE;
    ALCenum error;
    ALint source_state;


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

    alGenSources((ALuint)1, &source);
    TEST_ERROR("source generation");

    alSourcef(source, AL_PITCH, 1);
    TEST_ERROR("source pitch");
    alSourcef(source, AL_GAIN, 1);
    TEST_ERROR("source gain");
    alSource3f(source, AL_POSITION, 0, 0, 0);
    TEST_ERROR("source position");
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    TEST_ERROR("source velocity");
    alSourcei(source, AL_LOOPING, AL_FALSE);
    TEST_ERROR("source looping");

    alGenBuffers(1, &buffer);
    TEST_ERROR("buffer generation");
    alutLoadWAVFile("test.wav", &format, &data, &size, &freq);
    TEST_ERROR("loading wav file");

    alBufferData(buffer, format, data, size, freq);
    TEST_ERROR("buffer copy");

    alSourcei(source, AL_BUFFER, buffer);
    TEST_ERROR("buffer binding");

    printf("size %d\n",sizeof(buffer));

    int num;
    printf("hello firned");
    scanf("%d",&num);
    alSourcePlay(source);
    TEST_ERROR("source playing");

    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    TEST_ERROR("source state get");
    while (source_state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        TEST_ERROR("source state get");
    }

    /* exit context */
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    return;
}
