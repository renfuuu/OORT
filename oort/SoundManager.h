#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

//SDL
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <vector>

class SoundManager{
// protected:
private:

	bool mute;
	int backgroundVolume;

	static SoundManager* instance;
	std::vector<Mix_Chunk*> soundList;

	SoundManager() : 
		sampleAudioFile(NULL)
	{
		mute = false;
	}

	void onCleanup();

	//Audio File
	Mix_Chunk *sampleAudioFile;
public:	
	static SoundManager* getInstance(){
		if(instance == NULL)
			instance = new SoundManager;
		return instance;
	};

	//prevent copying singleton instance
	SoundManager(SoundManager const&);
	
	~SoundManager();

	void operator=(SoundManager const&);

	int onLoad(char* File);

	void playBackground();

	void play(int id);

	void initAudio();

	void releaseAudio();

	void toggleMute();

	void toggleBackground();

	void backgroundUp();

	void backgroundDown();

	bool isMuted();


};


#endif