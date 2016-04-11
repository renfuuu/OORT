#include "SoundManager.h"

SoundManager* SoundManager::instance = NULL;

SoundManager::~SoundManager()
{
	delete instance;
}

int SoundManager::onLoad(char* file)
{
	Mix_Chunk* temp = NULL;
	temp = Mix_LoadWAV(file);
	if(!temp)
	{
		printf("Mix_LoadWav:: %s\n", Mix_GetError());
		return -1;
	}

	soundList.push_back(temp);
	return soundList.size()-1;
}

void SoundManager::onCleanup()
{
	for (std::vector<Mix_Chunk*>::iterator i = soundList.begin(); i != soundList.end(); ++i)
	{
		Mix_FreeChunk(*i);
	}
	soundList.clear();
}

void SoundManager::playBackground()
{
	Mix_PlayChannel(0, soundList[0], -1);
	backgroundVolume = 32;
	Mix_Volume(0, backgroundVolume);
}

void SoundManager::play(int id)
{
	if(id < 0 || id >= soundList.size())
		return;
	if(soundList[id]==NULL)
		return;
	if(id != 0 && !mute)
	{
		Mix_PlayChannel(1, soundList[id], 0);
	}
}

void SoundManager::initAudio(){
	//init audio file here
	
	if(SDL_Init(SDL_INIT_AUDIO))
	{}

	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
}

void SoundManager::releaseAudio(){
	//release audio file.
   onCleanup();
    //Quit SDL_mixer
   Mix_CloseAudio();
    //Quit SDL
   SDL_Quit();
}

void SoundManager::toggleMute()
{
	mute = !mute;
}

void SoundManager::toggleBackground()
{
	if (Mix_Paused(0)) Mix_Resume(0);
	else Mix_Pause(0);
}

void SoundManager::backgroundUp()
{
	backgroundVolume += 4;
	Mix_Volume(0, backgroundVolume);
}

void SoundManager::backgroundDown()
{
	backgroundVolume -= 4;
	Mix_Volume(0, backgroundVolume);
}

bool SoundManager::isMuted()
{
	return mute;
}