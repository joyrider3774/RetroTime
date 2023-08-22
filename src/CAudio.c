#include <SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "CAudio.h"
#include "Platform.h"

bool CAudio_DebugInfo;
int CAudio_VolumeMusic, CAudio_VolumeSound;
Mix_Chunk* CAudio_Sounds[SND_Max];
Mix_Music* CAudio_Music[MUS_Max];
char CAudio_DataPath[500];
bool CAudio_GlobalSoundEnabled = true;

void CAudio_Init(char* AssetsPath, bool ADebugInfo)
{
	strcpy(CAudio_DataPath, AssetsPath);
	CAudio_DebugInfo = ADebugInfo;
	if (SDL_Init(SDL_INIT_AUDIO) == 0)
	{
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
		{
			CAudio_GlobalSoundEnabled = false;
			SDL_Log("Failed to initialise sound: %s\n", Mix_GetError());
		}
		else
		{
			SDL_Log("Audio Succesfully initialised!\n");
		}
	}
	else
	{
		CAudio_GlobalSoundEnabled = false;
		SDL_Log("Failed to initialise sound: %s\n", SDL_GetError());
	}

	for (int i=0; i < SND_Max; i++)
		CAudio_Sounds[i] = NULL;

	for (int i=0; i < MUS_Max; i++)
		CAudio_Music[i] = NULL;
}

void CAudio_DeInit()
{
	CAudio_UnloadSounds();
	CAudio_UnloadMusics();
	if (CAudio_GlobalSoundEnabled)
		Mix_CloseAudio();
}

// set the volume of the music
void CAudio_SetVolumeMusic(const int VolumeIn)
{
	if (CAudio_GlobalSoundEnabled)
	{
		CAudio_VolumeMusic = VolumeIn;
		Mix_VolumeMusic(VolumeIn);
	}
}

// set the volume of sound
void CAudio_SetVolumeSound(const int VolumeIn)
{
	if (CAudio_GlobalSoundEnabled)
	{
		CAudio_VolumeSound = VolumeIn;
		Mix_Volume(-1, VolumeIn);
	}
}

// increase the music volume with 4
void CAudio_IncVolumeMusic()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeMusic < 128)
		{
			CAudio_VolumeMusic += 4;
			CAudio_SetVolumeMusic(CAudio_VolumeMusic);
		}
	}
}

// increase the sound volume with 4
void CAudio_IncVolumeSound()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeSound < 128)
		{
			CAudio_VolumeSound += 4;
			CAudio_SetVolumeSound(CAudio_VolumeSound);
		}
	}
}

// decrease the volume with 4
void CAudio_DecVolumeMusic()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeMusic > 0)
		{
			CAudio_VolumeMusic -= 4;
			CAudio_SetVolumeMusic(CAudio_VolumeMusic);
		}
	}
}

// decrease the volume with 4
void CAudio_DecVolumeSound()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeSound > 0)
		{
			CAudio_VolumeSound -= 4;
			CAudio_SetVolumeSound(CAudio_VolumeSound);
		}
	}
}

void CAudio_StopMusic()
{
	if(CAudio_GlobalSoundEnabled)
		Mix_HaltMusic();
}

bool CAudio_IsMusicPlaying()
{
	if(!CAudio_GlobalSoundEnabled)
		return false;

	return (Mix_PlayingMusic() > 0);
}

void CAudio_UnLoadMusic(int MusicdID)
{
	if ((MusicdID < 0) || (MusicdID > MUS_Max) || !CAudio_GlobalSoundEnabled)
		return;

	if (CAudio_Music[MusicdID] == NULL)
		return;

	else
	{
		Mix_FreeMusic(CAudio_Music[MusicdID]);
		CAudio_Music[MusicdID] = NULL;
	}
}

void CAudio_UnloadMusics()
{
	CAudio_StopMusic();
	for (int i=0; i < MUS_Max; i++)
		CAudio_UnLoadMusic(i);
}

int CAudio_LoadMusic(char* FileName)
{
	if(!CAudio_GlobalSoundEnabled)
		return -1;

	char FullFileName[700];
	sprintf(FullFileName, "%smusic/%s",CAudio_DataPath,FileName);
	for (int i=0; i < MUS_Max; i++)
		if(CAudio_Music[i] == NULL)
		{
			Mix_Music* Tmp = Mix_LoadMUS(FullFileName);
			if(!Tmp)
				SDL_Log("Failed Loading Music %s\n", FullFileName);
			else
			{
				CAudio_Music[i] = Tmp;
				if(CAudio_DebugInfo)
					SDL_Log("Loaded Music %s\n", FullFileName);
				return i;
			}

		}
	return -1;
}

int CAudio_MusicSlotsUsed()
{
	int c = 0;
	for (int i=0; i < MUS_Max; i++)
	{
		if(CAudio_Music[i] != NULL)
			c++;
	}
	return c;
}

int CAudio_MusicSlotsMax()
{
	return MUS_Max;
}

void CAudio_PlayMusic(int MusicID, int loops)
{
	if ((MusicID < 0) || (MusicID > MUS_Max) || !CAudio_GlobalSoundEnabled)
		return;

	// Mix_VolumeMusic(VolumeMusic);
	Mix_PlayMusic(CAudio_Music[MusicID], loops);
}

int CAudio_GetVolumeMusic()
{
	return CAudio_VolumeMusic;
}

int CAudio_SoundSlotsUsed()
{
	int c = 0;
	for (int i=0; i < SND_Max; i++)
	{
		if(CAudio_Sounds[i] != NULL)
			c++;
	}
	return c;
}

int CAudio_SoundSlotsMax()
{
	return SND_Max;
}


void CAudio_PlaySound(int SoundID, int loops)
{
	if ((SoundID < 0) || (SoundID > SND_Max) || !CAudio_GlobalSoundEnabled)
		return;
	//Mix_Volume(-1, VolumeSound);
	Mix_PlayChannel(-1, CAudio_Sounds[SoundID], loops);
}

int CAudio_LoadSound(char* FileName)
{
	if(!CAudio_GlobalSoundEnabled)
		return -1;

	char FullFileName[700];
	sprintf(FullFileName, "%ssound/%s",CAudio_DataPath,FileName);
	for (int i=0; i < SND_Max; i++)
		if(CAudio_Sounds[i] == NULL)
		{
			Mix_Chunk *Tmp = Mix_LoadWAV(FullFileName);
			if(!Tmp)
				SDL_Log("Failed Loading Sound %s\n", FullFileName);
			else
			{
				CAudio_Sounds[i] = Tmp;
				if(CAudio_DebugInfo)
					SDL_Log("Loaded Sound %s\n", FullFileName);
				return i;
			}
		}
	return -1;
}

void CAudio_UnLoadSound(int SoundID)
{
	if ((SoundID < 0) || (SoundID > SND_Max) || !CAudio_GlobalSoundEnabled)
		return;

	if (CAudio_Sounds[SoundID] == NULL)
		return;
	else
	{
		Mix_FreeChunk(CAudio_Sounds[SoundID]);
		CAudio_Sounds[SoundID] = NULL;
	}
}

void CAudio_UnloadSounds()
{
	CAudio_StopSound();

	for (int i=0; i < SND_Max; i++)
		CAudio_UnLoadSound(i);
}

int CAudio_GetVolumeSound()
{
	return CAudio_VolumeSound;
}

void CAudio_StopSound()
{
	if(CAudio_GlobalSoundEnabled)
		Mix_HaltChannel(-1);
}