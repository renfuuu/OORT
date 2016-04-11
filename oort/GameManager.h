#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "BaseApplication.h"
#include "Ball.h"
#include "Room.h"
#include "Paddle.h"
#include "Goal.h"
#include "Simulator.h"
#include "SoundManager.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <string>

#include <OgreMath.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "NetManager.h"

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using std::cout;
using std::endl;

/**
*	Inherites from Ogre BaseApplication.
*	This can be changed by anytime
*
*	Class GameManager
*	This class will override needed functions from BaseApplication and manages the game loop
*/

enum GameState { SINGLE, MULTI, PAUSE, NUM_OF_STATES};


class GameManager : public BaseApplication {
protected:

private:
	Ball* marbleBall;
	Paddle* playerPaddle;
	Goal* theGoal;
	Room* room;
	Simulator* simulator;
	SoundManager* mixer;
  CEGUI::Renderer* mRenderer;


	CEGUI::Window* scoreBox;
	CEGUI::Window* liveBox;
	CEGUI::Window* timeBox;
	CEGUI::Window* infoBox;
	CEGUI::Window* playerBox;
	CEGUI::Window* gameOverBox;
	CEGUI::Window* IPBox;

	CEGUI::Window *join;
	CEGUI::Window *host; 


	Timer timer;
	NetManager* NetMan;


	Ogre::Vector3 hostPos;
	Ogre::Vector3 hostLook;

	Ogre::Vector3 guestPos;
	Ogre::Vector3 guestLook;




	
	bool isGuest;
	bool gameOver;
	bool guiMode;
	
	int currentState;	



	int backgroundSound;
	const static float MOUSE_DZ = 0.1f; // deadzone for mouse control
	const static float MOUSE_MOVE_SPEED = 2.4f; // how fast the paddle moves from mouse controls
	const static float CAMERA_DEFAULT_X = 1850.0f;
	const static float CAMERA_DEFAULT_Y = 1750.0f;
	const static float CAMERA_DEFAULT_Z = 3050.0f;


	/**
	*	Create scene
	*	Overrides from BaseApplication
	*/


	//TODO(koalanis):
	//	createScene
	//	framerenderingqueued
	//	all the keys


  virtual void createFrameListener(void);

	void createScene(void);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void processUnbufferedInput(const Ogre::FrameEvent& evt);
	void createCameras();

	virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );
  // OIS::MouseListener
  virtual bool mouseMoved( const OIS::MouseEvent &arg );
  virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  bool quit(const CEGUI::EventArgs &e);
  bool hostGame(const CEGUI::EventArgs &e);
  bool joinGame(const CEGUI::EventArgs &e);


  bool setupMultiplayer();
  bool handleSinglePlayer(const Ogre::FrameEvent& evt); 
  bool handleMultiPlayer(const Ogre::FrameEvent& evt);  
  bool handleServer(const Ogre::FrameEvent& evt);
  bool handleClient(const Ogre::FrameEvent& evt);
  bool handlePause(const Ogre::FrameEvent& evt);

	std::string createClientPacket();
	std::string createServerPacket();

public:
	/**
	*	Constructor
	*/
	GameManager(void);

	/**
	*	Destructor
	*	Release objects here.
	*/
	~GameManager(void);
};

#endif