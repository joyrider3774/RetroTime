#ifndef CAUDIO_H
#define CAUDIO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include <stdbool.h>
#include "Platform.h"

#define SND_Max 100
#define MUS_Max 20

void CAudio_Init(char* AssetsPath, bool ADebugInfo);
void CAudio_DeInit();
void CAudio_IncVolumeMusic();
void CAudio_DecVolumeMusic();
void CAudio_SetVolumeMusic(const int VolumeIn);
int CAudio_GetVolumeMusic();

void CAudio_IncVolumeSound();
void CAudio_DecVolumeSound();
void CAudio_SetVolumeSound(const int VolumeIn);
int CAudio_GetVolumeSound();

int CAudio_LoadMusic(char* FileName);
int CAudio_LoadSound(char* FileName);
int CAudio_MusicSlotsUsed();
int CAudio_MusicSlotsMax();
int CAudio_SoundSlotsUsed();
int CAudio_SoundSlotsMax();
void CAudio_UnLoadMusic(int MusicdID);
void CAudio_UnLoadSound(int SoundID);
void CAudio_UnloadSounds();
void CAudio_UnloadMusics();
void CAudio_PlayMusic(int MusicID, int loops);
void CAudio_PlaySound(int SoundID, int loops);
bool CAudio_IsMusicPlaying();
void CAudio_StopMusic();
void CAudio_StopSound();

#endif
