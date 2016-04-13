#pragma once

#include <OgreRoot.h>

#include "TextOverlay.h"
#include "MultiPlatformHelper.h"

class ScoreManager {
public:
	ScoreManager(void);
	~ScoreManager(void);

 	Ogre::Real getTime();
 	void postScore(void);
 	void postHighScore(void);
 	void scorePoints(int);
 	void nonFloorHit(void);
 	bool floorHit(void);
 	void resetScore(void);
 	void gameOver(void);
 	void writeScore();
	bool isGameOver();
	void resetGameOver();
	void showGameOver();
	void hideGameOver();

protected:

	// Timing Info
	Ogre::Timer* timer;
	Ogre::Real dt;

	// Score Data
 	int gameScore;
 	int highScore;
 	int floorHitCount;
 	bool gameOverB;
 	Ogre::OverlayContainer* overlayContainer;
 	TextOverlay* scoreOverlay;
 	TextOverlay* highScoreOverlay;
 	TextOverlay* gameOverOverlay;
	std::string scoreText;
	Ogre::String scoreLabel;
	std::string highScoreText;
	Ogre::String highScoreLabel;
	Ogre::String gameOverText;
	Ogre::String gameOverLabel;
	std::fstream highScoreFile;
};