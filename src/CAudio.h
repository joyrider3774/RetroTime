#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include "Platform.h"

using namespace std;

constexpr int SND_Max = 100;
constexpr int MUS_Max = 20;

class CAudio {

private:
    bool DebugInfo;
    int VolumeMusic, VolumeSound;
    Mix_Chunk *Sounds[SND_Max];
    Mix_Music *Music[MUS_Max];
    string DataPath;

public:
    bool GlobalSoundEnabled = true;
    CAudio(string AssetsPath, bool ADebugInfo);
    ~CAudio();
    void IncVolumeMusic();
    void DecVolumeMusic();
    void SetVolumeMusic(const int VolumeIn);
    int GetVolumeMusic();

    void IncVolumeSound();
    void DecVolumeSound();
    void SetVolumeSound(const int VolumeIn);
    int GetVolumeSound();

    int LoadMusic(string FileName);
    int LoadSound(string FileName);
    int MusicSlotsUsed();
    int MusicSlotsMax();
    int SoundSlotsUsed();
    int SoundSlotsMax();
    void UnLoadMusic(int MusicdID);
    void UnLoadSound(int SoundID);
    void UnloadSounds();
    void UnloadMusics();
    void PlayMusic(int MusicID, int loops);
    void PlaySound(int SoundID, int loops);
    bool IsMusicPlaying();
    void StopMusic();
    void StopSound();
};