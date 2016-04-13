#pragma once

#include "SoundManager.h"
#include "ScoreManager.h"

class SoundScoreManager : public SoundManager, public ScoreManager {
public:
	SoundScoreManager(void);
	~SoundScoreManager(void);
private:

};