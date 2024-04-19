#pragma once

#define MAX_NUM_SOUNDS 255
#define MAX_NUM_PLAYING_SOUNDS 20
#define SOUND_FREQUENCY 44100
#define SOUND_LATENCY 15
#define SOUND_BUFFERED_SECONDS 5

int initAudio();
void loadAudio(const char* name, unsigned int id);
void playAudio(int id, bool loop = false, float volume = 1.0f, float pitch = 1.0f, float pan = 0.5f, float delay = 0.0f);
void updateAudio();
void stopAudio(int id);
void freeAudio(int id);
void shutdownAudio();