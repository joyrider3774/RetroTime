#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "CAudio.h"
#include "Platform.h"

using namespace std;

CAudio::CAudio(string AssetsPath)
{
    DataPath = AssetsPath;
    if (SDL_Init(SDL_INIT_AUDIO) == 0) 
    {
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) 
        {
            GlobalSoundEnabled = false;
            SDL_Log("Failed to initialise sound: %s\n", Mix_GetError());
        } 
        else 
        {
            SDL_Log("Audio Succesfully initialised!\n");
        }
    } 
    else 
    {
        GlobalSoundEnabled = false;
        SDL_Log("Failed to initialise sound: %s\n", SDL_GetError());
    }
    
    for (int i=0; i < SND_Max; i++)
        Sounds[i] = nullptr;
    
    for (int i=0; i < MUS_Max; i++)
        Music[i] = nullptr;
}

CAudio::~CAudio()
{
    UnloadSounds();
    UnloadMusics();  
    if (GlobalSoundEnabled)
        Mix_CloseAudio(); 
}

// set the volume of the music
void CAudio::SetVolumeMusic(const int VolumeIn) 
{
    if (GlobalSoundEnabled) 
    {
        VolumeMusic = VolumeIn;
        Mix_VolumeMusic(VolumeIn);
    }
}

// set the volume of sound
void CAudio::SetVolumeSound(const int VolumeIn) 
{
    if (GlobalSoundEnabled) 
    {
        VolumeSound = VolumeIn;
        Mix_Volume(-1, VolumeIn);
    }
}

// increase the music volume with 8
void CAudio::IncVolumeMusic() 
{
    if (GlobalSoundEnabled) 
    {
        if (VolumeMusic < 128) 
        {
            VolumeMusic += 8;
            SetVolumeMusic(VolumeMusic);
        }
    }
}

// increase the sound volume with 8
void CAudio::IncVolumeSound() 
{
    if (GlobalSoundEnabled) 
    {
        if (VolumeSound < 128) 
        {
            VolumeSound += 8;
            SetVolumeSound(VolumeSound);
        }
    }
}

// decrease the volume with 8
void CAudio::DecVolumeMusic() 
{
    if (GlobalSoundEnabled) 
    {
        if (VolumeMusic > 0) 
        {
            VolumeMusic -= 8;
            SetVolumeMusic(VolumeMusic);
        }
    }
}

// decrease the volume with 8
void CAudio::DecVolumeSound() 
{
    if (GlobalSoundEnabled) 
    {
        if (VolumeSound > 0) 
        {
            VolumeSound -= 8;
            SetVolumeSound(VolumeSound);
        }
    }
}

void CAudio::StopMusic()
{
    if(GlobalSoundEnabled)
        Mix_HaltMusic();
}

bool CAudio::IsMusicPlaying()
{
    if(!GlobalSoundEnabled)
        return false;

    return (Mix_PlayingMusic() > 0);
}

void CAudio::UnLoadMusic(int MusicdID)
{
    if ((MusicdID < 0) || (MusicdID > MUS_Max) || !GlobalSoundEnabled)
        return;
    
    if (Music[MusicdID] == nullptr)
        return;
    
    else
    {
        Mix_FreeMusic(Music[MusicdID]);
        Music[MusicdID] = nullptr;
    }
}

void CAudio::UnloadMusics() 
{
    StopMusic();
    for (int i=0; i < MUS_Max; i++)
        UnLoadMusic(i);
}

int CAudio::LoadMusic(string FileName)
{
    if(!GlobalSoundEnabled)
        return -1;

    string FullFileName= DataPath + "music/" + FileName;
    for (int i=0; i < MUS_Max; i++)
        if(Music[i] == nullptr)
        {
            Music[i] = Mix_LoadMUS(FullFileName.c_str());
            return i;
        }
    return -1;
}

int CAudio::MusicSlotsUsed()
{
    int c = 0;
    for (int i=0; i < MUS_Max; i++)
    {
        if(Music[i] != nullptr)
            c++;
    }
    return c;
}

int CAudio::MusicSlotsMax()
{
    return MUS_Max;
}

void CAudio::PlayMusic(int MusicID, int loops)
{
    if ((MusicID < 0) || (MusicID > MUS_Max) || !GlobalSoundEnabled)
        return;

   // Mix_VolumeMusic(VolumeMusic);
    Mix_PlayMusic(Music[MusicID], loops);
}

int CAudio::GetVolumeMusic()
{
    return VolumeMusic;
}

int CAudio::SoundSlotsUsed()
{
    int c = 0;
    for (int i=0; i < SND_Max; i++)
    {
        if(Sounds[i] != nullptr)
            c++;
    }
    return c;
}

int CAudio::SoundSlotsMax()
{
    return SND_Max;
}


void CAudio::PlaySound(int SoundID, int loops)
{
    if ((SoundID < 0) || (SoundID > SND_Max) || !GlobalSoundEnabled)
        return;
    //Mix_Volume(-1, VolumeSound);
    Mix_PlayChannel(-1, Sounds[SoundID], loops);
}

int CAudio::LoadSound(string FileName)
{
    if(!GlobalSoundEnabled)
        return -1;

    string FullFileName = DataPath +  "sound/" + FileName;
    for (int i=0; i < SND_Max; i++)
        if(Sounds[i] == nullptr)
        {
            Sounds[i] = Mix_LoadWAV(FullFileName.c_str());
            return i;
        }
    return -1;
}

void CAudio::UnLoadSound(int SoundID)
{
    if ((SoundID < 0) || (SoundID > SND_Max) || !GlobalSoundEnabled)
        return;
    
    if (Sounds[SoundID] == nullptr)
        return;
    else
    {
        Mix_FreeChunk(Sounds[SoundID]);
        Sounds[SoundID] = nullptr;
    }
}

void CAudio::UnloadSounds() 
{
    StopSound();

    for (int i=0; i < SND_Max; i++)
        UnLoadSound(i);
}

int CAudio::GetVolumeSound()
{
    return VolumeSound;
}

void CAudio::StopSound()
{
    if(GlobalSoundEnabled)
        Mix_HaltChannel(-1);
}