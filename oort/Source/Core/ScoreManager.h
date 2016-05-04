#pragma once

#include <OgreRoot.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/FontManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

// #include "TextOverlay.h"
#include "MultiPlatformHelper.h"

class ScoreManager {
public:
	ScoreManager(void);
	~ScoreManager(void);

 	Ogre::Real getTime();
 	void addRenderer(CEGUI::OgreRenderer*);
 	void postScore(void);
 	void postHighScore(void);
 	void postLives(void);
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
	int getLives();
	void loseALife();
	// void updateLives(Spaceship* ss);

protected:

	// Timing Info
	Ogre::Timer* timer;
	Ogre::Real dt;

	// Score Data
 	int gameScore;
 	int gameLives;
 	int highScore;
 	int lives;
 	int floorHitCount;
 	bool gameOverB;

	std::string scoreText;
	Ogre::String scoreLabel;
	std::string highScoreText;
	Ogre::String highScoreLabel;
	std::string livesText;
	std::fstream highScoreFile;

	CEGUI::OgreRenderer* mRenderer;
	CEGUI::Window* gameScoreboard;
    CEGUI::Window* opponentScoreboard;
    CEGUI::Window* youWinBoard;
    CEGUI::Window* youLoseBoard;
    CEGUI::Window* livesBoard;
    CEGUI::Window* highscoreBoard;
};