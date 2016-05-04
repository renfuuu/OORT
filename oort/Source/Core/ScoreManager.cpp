#include "ScoreManager.h"

ScoreManager::ScoreManager(void) : gameScore(0), highScoreFile(), highScore(0), lives(5), gameOverB(false), scoreLabel("SCORE_"), scoreText("Score: "), highScoreLabel("HIGH_SCORE_"), highScoreText("High Score: "), livesText("Lives: ") {
	/* The global timer */
	timer = new Ogre::Timer();
	dt = timer->getMilliseconds();

	/* Load Score Resources */

	highScoreFile.open("highscore.txt", std::ios::in);

	std::string line;
	while(getline(highScoreFile, line)) {
		if(line == "") {
			highScore = 0;
		}
		else {
			highScore = std::stoi(line);
		}
	}

	highScoreFile.close();
	// Ogre::Overlay* ov = Ogre::OverlayManager::getSingletonPtr()->create("Overlay");
	// ov->show();

	// scoreOverlay = new TextOverlay(ov, scoreLabel, 0.02f, 0.9f, 0.04f);
	// scoreOverlay->setCol(1.0f, 1.0f, 1.0f, 1.0f);
	// highScoreOverlay = new TextOverlay(ov, highScoreLabel, 0.67f, 0.9f, 0.04f);
	// highScoreOverlay->setCol(1.0f, 1.0f, 1.0f, 1.0f);
	// gameOverOverlay = new TextOverlay(ov, gameOverLabel, 0.2f, 0.3f, 0.1f);
	// gameOverOverlay->setCol(1.0f, 0.1f, 0.1f, 1.0f);

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow(); 

	gameScoreboard = wmgr.createWindow("AlfiskoSkin/Label", "Points");
	gameScoreboard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.92f, 0)),
	CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(1, 0))));
	gameScoreboard->setText("Points: 0");

	highscoreBoard = wmgr.createWindow("AlfiskoSkin/Label", "HighScore");
	highscoreBoard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.92f, 0)),
	CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(1, 0))));
	highscoreBoard->setText("High Score: 0");

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
	youLoseBoard->setText("  Game Over!");
	youLoseBoard->setDisabled(true);
	youLoseBoard->setMouseCursor("AlfiskoSkin/MouseArrow");
	youLoseBoard->hide();

	livesBoard = wmgr.createWindow("AlfiskoSkin/Label", "Lives");
	livesBoard->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.92f, 0)),
		CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0))));
	livesBoard->setText("Lives: 5");

	sheet->addChild(gameScoreboard);
	sheet->addChild(livesBoard);
	sheet->addChild(highscoreBoard);
	sheet->addChild(youWinBoard);
	sheet->addChild(youLoseBoard);
}

/* Sound Functions */

ScoreManager::~ScoreManager(void) {
}

void ScoreManager::addRenderer(CEGUI::OgreRenderer* rend) {
	mRenderer = rend;
}

Ogre::Real ScoreManager::getTime() {
	return timer->getMilliseconds();
}

/* Score Functions */

void ScoreManager::postScore(void) {
	// scoreOverlay->showOverlay();
	// scoreOverlay->setText(scoreText + std::to_string(gameScore));
	gameScoreboard->setText(scoreText + std::to_string(gameScore));
	// opponentScoreboard->setText(enemyScoreText + std::to_string(enemyScore));

}

void ScoreManager::postHighScore(void) {
	// highScoreOverlay->showOverlay();
	// highScoreOverlay->setText(highScoreText + std::to_string(highScore));
	highscoreBoard->setText(highScoreText + std::to_string(highScore));
}

void ScoreManager::postLives(void){
	livesBoard->setText(livesText + std::to_string(lives));
}

void ScoreManager::scorePoints(int points) {
	gameScore += points;
	// nonFloorHit();
	postScore();
}


void ScoreManager::nonFloorHit(void) {
	// floorHitCount = 0;
}

// Returns false if the game is over.
bool ScoreManager::floorHit(void) {
	// floorHitCount++;
	// if ( floorHitCount >= 2 ) {
	// 	gameOver();
	// 	return false;
	// }
	// return true;
}

void ScoreManager::resetScore(void) {
	if ( gameScore > highScore ) {
		highScore = gameScore;
		writeScore();
		postHighScore();
	}
	// floorHitCount = 0;
	gameScore = 0;
	lives = 5;
	postScore();
	postLives();
}

bool ScoreManager::isGameOver() {
	return gameOverB;
}

void ScoreManager::resetGameOver() {
	gameOverB = false;
}

void ScoreManager::showGameOver() {
	// gameOverOverlay->showOverlay();
	// (gameScore > enemyScore) ? youWinBoard->show() : youLoseBoard->show();
	youLoseBoard->show();
}

void ScoreManager::hideGameOver() {
	// gameOverOverlay->hideOverlay();
	youWinBoard->hide();
	youLoseBoard->hide();
}


void ScoreManager::gameOver() {
	gameOverB = true;
	// gameOverOverlay->setText(gameOverText);
	// showGameOver();
	resetScore();
}

void ScoreManager::writeScore() {
	// Replace old highscore with new one
	highScoreFile.open("highscore.txt", std::ios::out);
	highScoreFile << std::to_string(highScore) + "\n";
	highScoreFile.close();
}

int ScoreManager::getLives(){
	return lives;
}

void ScoreManager::loseALife(){
	lives--;
	postLives();
}