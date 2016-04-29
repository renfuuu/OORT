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

#include <stdlib.h>
#include <time.h>

#include "GameManager.h"
#include "OISManager.h"
#include "Simulator.h"
#include "Spaceship.h"
#include "Wall.h"
#include "Laser.h"
#include "Asteroid.h"

class Application : public Ogre::FrameListener, public Ogre::WindowEventListener, public Ogre::RenderTargetListener
{
public:
	Application();
	virtual ~Application();

	virtual void init();

	Ogre::Root * mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow * mRenderWindow;
	Ogre::SceneManager * mSceneManager;
	Ogre::Camera * mCamera;
	Ogre::Camera * spaceshipCam;
	Ogre::Camera * camMan;
	OgreBites::SdkCameraMan * cameraMan;
	Ogre::Timer* t1;

	GameManager* _gameManager;
	OISManager* _oisManager;
	Simulator* _simulator;
	GameObject* _theBall;
	GameObject* _theSpaceship;
	Ogre::SceneNode* _camNode;

    CEGUI::OgreRenderer* mRenderer;

    //CEGUI Windows here
	
	std::vector<Ogre::Camera*> cameras;
	std::vector<Laser*> lasers;
	std::vector<Asteroid*> asteroids;

	int points;
	int width;
	int height;

	int camChange;
	int laserCount;
	int asteroidCount;

	double fps = 300.0;

	bool mRunning = true;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
	void createRootEntity(std::string name, std::string mesh, int x, int y, int z);
	void createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z);
	void update(const Ogre::FrameEvent &evt);
	Spaceship* createSpaceship(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Wall* createWall(Ogre::String nme, GameObject::objectType tp, std::string type, int width, int height, Ogre::Vector3 position, Ogre::Vector3 rotate, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Laser* createLaser(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, GameObject* sship, Ogre::Vector3 scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	Asteroid* createAsteroid(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 rotate, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);


	void setupWindowRendererSystem(void);
	void setupResources(void);
	void setupOIS(void);
	void setupCEGUI(void);
	void setupCameras(void);
	void setupGameManager(void);
	void setupLighting(void);
	void createObjects(void);

};
