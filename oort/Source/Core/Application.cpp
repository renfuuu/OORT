#include "Application.h"
#include "CoreConfig.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"
#include <Overlay/OgreOverlaySystem.h>

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <string>
#include "btBulletDynamicsCommon.h"
#include "OISManager.h"
#include "OgreMotionState.h"
#include "Simulator.h"
#include "GameObject.h"

using namespace Ogre;

Application::Application()
{
}

Application::~Application()
{
}

void Application::init()
{

	try{
		t1 = new Timer();

		_simulator = new Simulator();
		
		setupWindowRendererSystem();

		setupOIS();

		setupCameras();

		Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
		mSceneManager->addRenderQueueListener(pOverlaySystem);

		setupGameManager();

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		setupLighting();

		createObjects();

	}
	catch (Exception e) {
		std::cout << "Exception Caught: " << e.what() << std::endl;
	}

}


bool Application::frameRenderingQueued(const FrameEvent &evt)
{
	static float gameOverTime = 0.0f;
	static float dTime = t1->getMilliseconds();
	
	if (mRenderWindow->isClosed())
	{
		return false;
	}

	if (!mRunning)
	{
		return false;
	}
		try {
			_oisManager->capture();
			_theSpaceship->moveSpaceship(_oisManager, height, width);

		// close window when ESC is pressed
		if(_oisManager->getKeyPressed() == OIS::KC_ESCAPE)
			mRunning = false;
	}
	catch (Exception e) {

	}
	// Code per frame in fixed FPS
	float temp = t1->getMilliseconds();
	if ((temp - dTime) >= (1.0 / fps)*1000.0) {
		if( _soundScoreManager->isGameOver() ) {
			gameOverTime += (temp - dTime);
		}
		update(evt);
		dTime = temp;
	}

	if ( !(_soundScoreManager->isGameOver()) ) {
		_simulator->stepSimulation(evt.timeSinceLastFrame, 1, 1.0 / fps);
	}
	else {
		// _soundScoreManager->showGameOver();
		// if(gameOverTime > 2000) {
		// 	_soundScoreManager->resetGameOver();
		// 	_soundScoreManager->hideGameOver();
		// 	gameOverTime = 0.0f;
		// }
	}

	// // Constrains the ball's speed
	// static int maxSpeed = 4000;
	// btVector3 velocity = _theBall->getBody()->getLinearVelocity();
 //    btScalar speed = velocity.length();
 //    if(speed > maxSpeed) {
 //        velocity *= maxSpeed/speed;
 //        _theBall->getBody()->setLinearVelocity(velocity);
 //    }

	return true;
}

// Called once per predefined frame
void Application::update(const FrameEvent &evt) {

	OIS::KeyCode lastKey = _oisManager->lastKeyPressed();

	if(lastKey == OIS::KC_SPACE) {
		// _theBall->resetScore();

	}
	else if (lastKey == OIS::KC_M) {
		_soundScoreManager->mute();
	}
	else if (lastKey == OIS::KC_1 || lastKey == OIS::KC_2 || lastKey == OIS::KC_3) {
		int index = lastKey - 2;
		if (index >= 0 && index < cameras.size()) {
			mRenderWindow->removeAllViewports();
			mRenderWindow->addViewport(cameras[index]);
		}
	}

	// spaceshipCam->lookAt(_theBall->getNode()->getPosition());

	// Small pull toward paddle to make it easier for the player to hit the ball
	// int pull = 500;
	// Ogre::Vector3 paddleAttract = (_theSpaceship->getNode()->getPosition() - _theBall->getNode()->getPosition()).normalisedCopy();
	// _theBall->applyForce(paddleAttract.x * pull, paddleAttract.y * pull, paddleAttract.z * pull);
}


void Application::createRootEntity(std::string name, std::string mesh, int x, int y, int z) {
	Ogre::Entity* ogreEntity = mSceneManager->createEntity(name, mesh);
	ogreEntity->setCastShadows(true);
	Ogre::SceneNode* ogreNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name);
	ogreNode->attachObject(ogreEntity);
	ogreNode->setPosition(x, y, z);
}

void Application::createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z) {
	Ogre::Entity* ogreEntity = mSceneManager->createEntity(name, mesh);
	ogreEntity->setCastShadows(true);
	Ogre::SceneNode* ogreNode = sceneNode->createChildSceneNode(name);
	ogreNode->attachObject(ogreEntity);
	ogreNode->setPosition(x, y, z);
}

Spaceship* Application::createSpaceship(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	sn->setScale(scale,scale,scale);
	// sn->showBoundingBox(true);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Spaceship* obj = new Spaceship(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Wall* Application::createWall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale.x, scale.y, scale.z);

	if(meshName != "floor.mesh") {
		ent->setMaterialName("wall");
	}

	sn->pitch(pitch);
	sn->yaw(yaw);
	sn->roll(roll);

	Wall* obj = new Wall(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

void Application::setupWindowRendererSystem(void) {

	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";

	NameValuePairList params;
	// Initialization
	mRoot = new Root(mPluginsCfg);

	setupResources();

	// load plugins
#ifdef _WIN32
	mRoot->loadPlugin("RenderSystem_GL_d");
	mRoot->loadPlugin("Plugin_ParticleFX_d");
#endif
#ifdef __linux__
	mRoot->loadPlugin("/lusr/opt/ogre-1.9/lib/OGRE/RenderSystem_GL");
#endif

	// Select render system
	const RenderSystemList &renderers = mRoot->getAvailableRenderers();
	RenderSystem * renderSystem = nullptr;
	LogManager::getSingletonPtr()->logMessage("Getting available renderers");
	for (auto renderer = renderers.begin(); renderer != renderers.end(); renderer++)
	{
		String name = (*renderer)->getName();
		LogManager::getSingletonPtr()->logMessage(name);
		renderSystem = *renderer;
	}
	if (renderSystem)
	{
		LogManager::getSingletonPtr()->logMessage("Using renderer " + renderSystem->getName());
		mRoot->setRenderSystem(renderSystem);
	}
	else
	{
		LogManager::getSingletonPtr()->logMessage(LML_CRITICAL, "Initializing render system failed. No renderers available.");
	}

	// Initialize with render system, no new window (yet)
	mRoot->initialise(false);

	// Create scene manager, render window, and camera
	mSceneManager = mRoot->createSceneManager(ST_GENERIC);
	mRenderWindow = mRoot->createRenderWindow(PROJECT_NAME, width = 1200, height = 900, false, &params);
}

void Application::setupResources(void){
	// Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
	#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void Application::setupOIS(void) {

    _oisManager = OISManager::getSingletonPtr();
    _oisManager->initialise( mRenderWindow );
    _oisManager->addKeyListener( (OIS::KeyListener*)_oisManager, "keyboardListener" );
    _oisManager->addMouseListener( (OIS::MouseListener*)_oisManager, "mouseListener" );

}

void Application::setupCameras(void) {

	mCamera = mSceneManager->createCamera("Main Camera");
	Ogre::Camera* cam2 = mSceneManager->createCamera("Cam2");
	spaceshipCam = mSceneManager->createCamera("Ball Cam");
	// camMan = mSceneManager->createCamera("Camera Man");

	// Add viewport and cameras
	mRenderWindow->addViewport(mCamera);

	mCamera->setAutoAspectRatio(true);
	mCamera->setPosition(0, 120, 1800);

	cam2->setAutoAspectRatio(true);
	cam2->setPosition(1350, 0, -400);
	cam2->yaw(Ogre::Degree(90));
	cam2->pitch(Ogre::Degree(15));

	spaceshipCam->setAutoAspectRatio(true);
	spaceshipCam->setPosition(0, 120, 1800);

	// camMan->setAutoAspectRatio(true);
	// camMan->setPosition(0,120,400);
	// camMan->lookAt(0,120,1800);

	cameras = std::vector<Ogre::Camera*>();
	cameras.push_back(mCamera);
	cameras.push_back(cam2);
	cameras.push_back(spaceshipCam);
	// cameras.push_back(camMan);

	// cameraMan = new OgreBites::SdkCameraMan(camMan);
	// _oisManager->setupCameraMan(cameraMan);

}

void Application::setupGameManager(void) {

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	_soundScoreManager = new SoundScoreManager();
	_soundScoreManager->startMusic();

}


void Application::setupLighting(void) {

	mSceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Light* directionalLight = mSceneManager->createLight("Sun");
	directionalLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	directionalLight->setCastShadows(true);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.8, .8, .8));
	directionalLight->setSpecularColour(Ogre::ColourValue(.8, .8, .8));

	directionalLight->setDirection(Ogre::Vector3(0, -1, .1));

	// // Add some light
	// mSceneManager->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
	// mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Ogre::Light* light = mSceneManager->createLight("MainLight");
	// light->setCastShadows(true);
	// light->setPosition(0, 500, 0);
	// light->setType(Ogre::Light::LightTypes::LT_POINT);

}

void Application::createObjects(void) {
	
	mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	// This paddle gets a negative Z coordinate that becomes positive in the function on movepaddle
	_theSpaceship = createSpaceship("spaceship", GameObject::objectType::SPACESHIP_OBJECT, "Plane.mesh", 0, 0, -825, 100, mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, true, _simulator);
	// _otherPaddle = createPaddle("other_paddle", GameObject::objectType::PADDLE_OBJECT, "paddle-blue.mesh", 0, 0, 825, 100, mSceneManager, gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);
	// _theBall = createBall("ball", GameObject::objectType::BALL_OBJECT, "sphere.mesh", 5, 300, 800, .35, mSceneManager, _soundScoreManager, 1.0f, 1.0f, 0.8f, false, _simulator);

	// createWall("floor", GameObject::objectType::FLOOR_OBJECT, "floor.mesh", 0, -100, 0, Ogre::Vector3(120, 240, 240), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	// createWall("ceiling", GameObject::objectType::WALL_OBJECT, "ceiling.mesh", 0, 500, 0, Ogre::Vector3(120, 240, 240), Ogre::Degree(180), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.5f, 0.8f, false, _simulator);
	// createWall("backwall", GameObject::objectType::BACK_WALL_OBJECT, "backwall.mesh", 0, 300, -1200, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.8f, 0.8f, false, _simulator);
	// createWall("leftwall", GameObject::objectType::WALL_OBJECT, "leftwall.mesh", 600, 0, -430, Ogre::Vector3(120, 120, 400), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	// createWall("rightwall", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 400), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	// createWall("frontwall", GameObject::objectType::FRONT_WALL_OBJECT, "backwall.mesh", 0, 300, 1200, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(180), mSceneManager, _soundScoreManager, 0.0f, 0.9f, 0.8f, false, _simulator);

	// createRootEntity("stadium", "stadium2.mesh", 0, -592, 0);
	// mSceneManager->getSceneNode("stadium")->setScale(100,100,100);
	// mSceneManager->getSceneNode("stadium")->yaw(Ogre::Degree(90));
	// stadiumFlashes = new StadiumFlashes(mSceneManager);

	// _theBall->startScore();

	// mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	// // Test Bullet
	// _theSpaceship = createPaddle("paddle", GameObject::objectType::PADDLE_OBJECT, "paddle.mesh", 0, 0, 0, 100, mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, true, mySim);

	// _theBall = createBall("ball", GameObject::objectType::BALL_OBJECT, "sphere.mesh", 5, 300, 0, .35, mSceneManager, _soundScoreManager, 1.0f, 1.0f, 0.8f, false, mySim);
	// createWall("floor", GameObject::objectType::FLOOR_OBJECT, "floor.mesh", 0, -100, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	// createWall("ceiling", GameObject::objectType::WALL_OBJECT, "ceiling.mesh", 0, 600, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(180), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.5f, 0.8f, false, mySim);
	// createWall("backwall", GameObject::objectType::BACK_WALL_OBJECT, "backwall.mesh", 0, 300, -1350, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, _soundScoreManager, 0.0f, 0.8f, 0.8f, false, mySim);
	// createWall("leftwall", GameObject::objectType::WALL_OBJECT, "leftwall.mesh", 600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	// createWall("rightwall", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	// createWall("ceiling?", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 200), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, _soundScoreManager, 0.0f, 1.0f, 0.8f, false, mySim);
	// createWall("frontwall?", GameObject::objectType::FRONT_WALL_OBJECT, "backwall.mesh", 0, 300, 500, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(180), mSceneManager, _soundScoreManager, 0.0f, 0.9f, 0.8f, false, mySim);


	// _theBall->startScore();

}