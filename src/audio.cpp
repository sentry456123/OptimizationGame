#include "audio.h"
#include <iostream>

#define AUDIO_BGM_PATH "res/bgm.mp3"

namespace audio
{
    namespace BGM
    {
        static Mix_Music* bgm;
    }
}

void audio::Init(void)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "Error: " << Mix_GetError() << std::endl;
		return;
	}

    BGM::Init();
}

void audio::Quit(void)
{
    BGM::Quit();
}

void audio::BGM::Init(void)
{
    bgm = Mix_LoadMUS(AUDIO_BGM_PATH);
    Mix_VolumeMusic(50);
}

void audio::BGM::Quit(void)
{
    Mix_FreeMusic(bgm);
}

void audio::BGM::Loop(void)
{
    Mix_PlayMusic(bgm, -1);
}

void audio::BGM::Stop(void)
{
    Mix_HaltMusic();
}
