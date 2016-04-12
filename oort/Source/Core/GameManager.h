#pragma once

#include "SoundManager.h"
#include "ScoreManager.h"

class GameManager : public SoundManager, public ScoreManager {
public:
	GameManager(void);
	~GameManager(void);
private:

};