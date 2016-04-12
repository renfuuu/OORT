#pragma once

#include <OgreRoot.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/FontManager.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "MultiPlatformHelper.h"

class ScoreManager {
public:
	ScoreManager(void);
	~ScoreManager(void);

 	Ogre::Real getTime();
 	void addRenderer(CEGUI::OgreRenderer*);
 	void setServer(bool);
 	bool isServer();
 	void postScore(void);
 	void scorePoints(int);
 	void scoreOpponentPoints(int);
 	int getGameScore();
 	int getEnemyScore();
 	void setScore(int points);
 	void setEnemyScore(int points);
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
 	int enemyScore;
 	int highScore;
 	bool gameOverB;
 	bool server;
 	
	std::string scoreText;
	std::string enemyScoreText;
	Ogre::String scoreLabel;

 	CEGUI::OgreRenderer* mRenderer;
	CEGUI::Window* gameScoreboard;
    CEGUI::Window* opponentScoreboard;
    CEGUI::Window* youWinBoard;
    CEGUI::Window* youLoseBoard;
};