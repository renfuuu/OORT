#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderTargetListener.h> 

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <SdkCameraMan.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <unordered_map>
#include <list>
#include <string>
#include <cstring>
#include <cstdlib>

#include "NetManager.h"
#include "StadiumFlashes.h"
#include "GameManager.h"
#include "OISManager.h"
#include "Simulator.h"
#include "Ball.h"
#include "Paddle.h"
#include "Wall.h"

#include "GameState.h"

class Application : public Ogre::FrameListener, public Ogre::WindowEventListener, public Ogre::RenderTargetListener
{
public:
	Application();
	virtual ~Application();

	virtual void init();

	enum State{ HOME, SINGLE, SERVER, CLIENT, ENDGAME, REPLAY, HOWTO };
	State gameState = HOME;

	Ogre::Root * mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow * mRenderWindow;
	Ogre::SceneManager * mSceneManager;
	Ogre::Camera * mCamera;
	Ogre::Camera * ballCam;
	Ogre::Camera * camMan;
	OgreBites::SdkCameraMan * cameraMan;
	Ogre::Timer* t1;

	NetManager* netManager = nullptr;

	StadiumFlashes* stadiumFlashes;

	GameManager* gameManager;
	OISManager* _oisManager;
	Simulator* _simulator;
	GameObject* _theBall;
	GameObject* _thePaddle;
	GameObject* _otherPaddle;

    CEGUI::OgreRenderer* mRenderer;

    CEGUI::Window* hostServerButton;
    CEGUI::Window* joinServerButton;
    CEGUI::Window* ipBox;
    CEGUI::Window* ipText;
    CEGUI::Window* ipWindow;
    CEGUI::Window* singlePlayerButton;
    CEGUI::Window* homeButton;
    CEGUI::Window* replayButton;
    CEGUI::Window* howToButton;
    CEGUI::Window* howToText;
	
	std::vector<Ogre::Camera*> cameras;
	std::list<GameState> states;

	int points;
	int width;
	int height;

	double fps = 300.0;

	bool begin = false;
	bool mRunning = true;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
	void createRootEntity(std::string name, std::string mesh, int x, int y, int z);
	void createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z);
	bool update(const Ogre::FrameEvent &evt);
	bool handleGUI(const Ogre::FrameEvent &evt);
	void handleAi();
	bool updateServer(const Ogre::FrameEvent &evt);
	bool updateClient(const Ogre::FrameEvent &evt);

	Ball* createBall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Paddle* createPaddle(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Wall* createWall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	void startStadiumFlashes(void);
	void setupWindowRendererSystem(void);
	void setupResources(void);
	void setupOIS(void);
	void setupCEGUI(void);
	void setupCameras(void);
	void setupGM(void);
	void setupLighting(void);
	void createObjects(void);
	bool StartServer(const CEGUI::EventArgs &e);
	bool JoinServer(const CEGUI::EventArgs &e);
	bool Quit(const CEGUI::EventArgs &e);
	bool StartSinglePlayer(const CEGUI::EventArgs &e);
	bool Replay(const CEGUI::EventArgs &e);
	bool Home(const CEGUI::EventArgs &e);
	bool HowTo(const CEGUI::EventArgs &e);

	bool setupNetwork(bool);
	bool error();
	std::unordered_map<std::string, char*> dataParser(char*);
	void hideGui();
	void showGui();
	void showEndGui();
	void resetNetManager();
	void setState(State state);
	void replayData();
};
