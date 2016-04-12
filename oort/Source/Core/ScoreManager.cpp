#include "ScoreManager.h"

ScoreManager::ScoreManager(void) : gameScore(0), enemyScore(0), gameOverB(false), scoreLabel("SCORE_"), scoreText("Red: "), enemyScoreText("Blue: ") {
	/* The global timer */
	timer = new Ogre::Timer();
	dt = timer->getMilliseconds();

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow(); 

	gameScoreboard = wmgr.createWindow("AlfiskoSkin/Label", "Red");
	gameScoreboard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.92f, 0)),
	CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(1, 0))));
	gameScoreboard->setText("Red: 0");

	opponentScoreboard = wmgr.createWindow("AlfiskoSkin/Label", "Blue");
	opponentScoreboard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.92f, 0)),
	CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0))));
	opponentScoreboard->setText("Blue: 0");

	youWinBoard = wmgr.createWindow("AlfiskoSkin/Editbox", "YouWinBoard");
	youWinBoard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.42f, 0), CEGUI::UDim(0.33f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.58f, 0), CEGUI::UDim(0.4f, 0))));
	youWinBoard->setText("     You Win!");
	youWinBoard->setDisabled(true);
	youWinBoard->setMouseCursor("AlfiskoSkin/MouseArrow");
	youWinBoard->hide();

	youLoseBoard = wmgr.createWindow("AlfiskoSkin/Editbox", "YouLoseBoard");
	youLoseBoard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.42f, 0), CEGUI::UDim(0.33f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.58f, 0), CEGUI::UDim(0.4f, 0))));
	youLoseBoard->setText("    You Lose!");
	youLoseBoard->setDisabled(true);
	youLoseBoard->setMouseCursor("AlfiskoSkin/MouseArrow");
	youLoseBoard->hide();

	sheet->addChild(gameScoreboard);
	sheet->addChild(opponentScoreboard);
	sheet->addChild(youWinBoard);
	sheet->addChild(youLoseBoard);
}

ScoreManager::~ScoreManager(void) {
}

void ScoreManager::addRenderer(CEGUI::OgreRenderer* rend) {
	mRenderer = rend;
}

Ogre::Real ScoreManager::getTime() {
	return timer->getMilliseconds();
}

/* Score Functions */

void ScoreManager::setServer(bool s) {
	server = s;
}

bool ScoreManager::isServer() {
	return server;
}

void ScoreManager::postScore(void) {
	gameScoreboard->setText(scoreText + std::to_string(gameScore));
	opponentScoreboard->setText(enemyScoreText + std::to_string(enemyScore));
}

void ScoreManager::scorePoints(int points) {
	gameScore += points;
	postScore();
}

void ScoreManager::scoreOpponentPoints(int points) {
	enemyScore += points;
	postScore();
}

int ScoreManager::getGameScore() {
	return gameScore;
}

int ScoreManager::getEnemyScore() {
	return enemyScore;
}

void ScoreManager::setScore(int points) {
	gameScore = points;
	postScore();
}

void ScoreManager::setEnemyScore(int points) {
	enemyScore = points;
	postScore();
}

void ScoreManager::resetScore(void) {
	gameScore = 0;
	enemyScore = 0;
	postScore();
}

bool ScoreManager::isGameOver() {
	static int max_score = 11;
	// If a player has more than 20 points && wins by more than one point return true;
	return ( gameScore >= max_score || enemyScore >= max_score ) && ( gameScore != enemyScore && Ogre::Math::Abs(gameScore - enemyScore) > 1 );
}

void ScoreManager::resetGameOver() {
	gameOverB = false;
}

void ScoreManager::showGameOver() {
	(gameScore > enemyScore) ? youWinBoard->show() : youLoseBoard->show();
}

void ScoreManager::hideGameOver() {
	youWinBoard->hide();
	youLoseBoard->hide();
}

void ScoreManager::gameOver() {
	gameOverB = true;
	resetScore();
}