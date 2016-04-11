#include "Application.h"
#include "CoreConfig.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <string>
#include "btBulletDynamicsCommon.h"
#include "OISManager.h"
#include "OgreMotionState.h"
#include "Simulator.h"
#include "GameObject.h"
#include <cstring>

using namespace Ogre;

std::string instructions = "- To play alone, select Single Player\n- Choose to Host a game or join a game (requires two computers)\n- To join a game, one computer must be hosting and you must enter the public ip address of the machine that is hosting and press join game\n- Move the mouse up/down and left/right on the game window to control the paddle\n- Strike the ball with the paddle to propel it through the room and attempt to bypass your opponent's paddle\n- M mutes the sounds\n- Keys 1,2,3, and 4 change the camera angle (front, side,ball chase view, and free camera)\n- Free camera works as a camera man (WSAD or Arrow keys to move) hold shift to speed up\n- Escape key to quit or click the quit button";

Application::Application() : states()
{
}

Application::~Application()
{
}

void Application::init()
{
	// This is really just a debugging try-catch block for catching and printing exceptions
	try {

		t1 = new Timer();

		_simulator = new Simulator();

		setupWindowRendererSystem();

		setupOIS();

		netManager = new NetManager();
		setupCEGUI();

		setupCameras();

		setupGM();

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		setupLighting();

		createObjects();

	}
	catch (Exception e) {
		std::cout << "Exception Caught: " << e.what() << std::endl;
	}
}

/* 
* Update Methods 
*/
bool Application::frameRenderingQueued(const FrameEvent &evt)
{

	static float gameOverTime = 0.0f;
	static float dTime = t1->getMilliseconds();
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	if (!mRunning)
	{
		return false;
	}
	try {
		_oisManager->capture();
	}
	catch (Exception e) {

	}

	/* Begun split this into three areas, Setup & GUI, server, client */
	/* This line has been left here for the sake of closing the window. Maybe we'll move it later. */
	if (mRenderWindow->isClosed())
	{
		return false;
	}

	// Code per frame in fixed FPS
	float temp = t1->getMilliseconds();
	if ((temp - dTime) >= (1.0 / fps)*1000.0) {
		if( gameManager->isGameOver() ) {
			gameOverTime += (temp - dTime);
		}
		// This update method gets called once per frame and is called whether or not we are a server or a client
		update(evt);
		dTime = temp;
	}

	// Constrains the ball's speed
	static int maxSpeed = 4000;
	btVector3 velocity = _theBall->getBody()->getLinearVelocity();
    btScalar speed = velocity.length();
    if(speed > maxSpeed) {
        velocity *= maxSpeed/speed;
        _theBall->getBody()->setLinearVelocity(velocity);
    }

	ballCam->lookAt(_theBall->getNode()->getPosition());

	cameraMan->frameRenderingQueued(evt);

	return true;
}

// Called once per predefined frame
bool Application::update(const FrameEvent &evt) {

	OIS::KeyCode lastKey = _oisManager->lastKeyPressed();

	if (lastKey == OIS::KC_M) {
		gameManager->mute();
	}
	else if (lastKey == OIS::KC_1 || lastKey == OIS::KC_2 || lastKey == OIS::KC_3 || lastKey == OIS::KC_4) {
		int index = lastKey - 2;
		if (gameState != HOME && index >= 0 && index < cameras.size()) {
			mRenderWindow->removeAllViewports();
			mRenderWindow->addViewport(cameras[index]);
		}
	}
	else if(lastKey == OIS::KC_ESCAPE) {
		// close window when ESC is pressed
		mRunning = false;
	}

	// All logic is now controlled by a state machine
	switch(gameState) {
		case HOME:
			handleGUI(evt);
			return true;
			break;
		case SERVER:
			updateServer(evt);
			if(netManager->getClients() == 0)
				return true;
			break;
		case CLIENT:
			updateClient(evt);
			break;
		case SINGLE:
			handleAi();
			break;
		case ENDGAME:
			return true;
			break;
		case REPLAY:
			replayData();
			break;
		case HOWTO:
			return true;
			break;
	}

	float realX = (float)_oisManager->getMouseXAxis();
	float realY = (float)_oisManager->getMouseYAxis();

	if(gameState == SERVER || gameState == CLIENT || gameState == SINGLE)
		_thePaddle->movePaddle(_oisManager, height, width, realX, realY);

	Ogre::Quaternion qt1 = _thePaddle->getNode()->getOrientation();
	Ogre::Quaternion qt2 = _otherPaddle->getNode()->getOrientation();
	Ogre::Vector3 pPos = _thePaddle->getNode()->getPosition();
	Ogre::Vector3 oPos = _otherPaddle->getNode()->getPosition();
	Ogre::Vector3 pos = _theBall->getNode()->getPosition();
	btVector3 vel = _theBall->getBody()->getLinearVelocity();
	int gScore = gameManager->getGameScore();
	int oScore = gameManager->getEnemyScore();

	states.push_back(GameState(qt1,qt2,pPos,oPos,pos,vel,gScore,oScore));

	if ( !(gameManager->isGameOver()) ) {
		_simulator->stepSimulation(evt.timeSinceLastFrame, 1, 1.0 / fps);
	}
	else {
		if ( gameManager->getEnemyScore() < gameManager->getGameScore() )
			gameManager->playSound(GameManager::GAME_WIN);
		else
			gameManager->playSound(GameManager::GAME_LOSS);
		// Make sure the state of the entire game is reset before doing this. Ie all scores set to 0 and all network connections closed.
		setState(ENDGAME); // We may want to change this to an end game menu
	}

	// Small pull toward the closest paddle to make it easier for the player to hit the ball
	GameObject* pullPaddle = (_thePaddle->getNode()->getPosition() - _theBall->getNode()->getPosition()).length() <= (_otherPaddle->getNode()->getPosition() - _theBall->getNode()->getPosition()).length() ? _thePaddle : _otherPaddle;

	int pull = 500;
	Ogre::Vector3 paddleAttract = (pullPaddle->getNode()->getPosition() - _theBall->getNode()->getPosition()).normalisedCopy();
	_theBall->applyForce(paddleAttract.x * pull, paddleAttract.y * pull, paddleAttract.z * pull);

	// Check if the ball has been ejected from the scene
	if(_theBall->getNode()->getPosition().length() > 4000) {
		_theBall->reset();
	}

	return true;
}

bool Application::handleGUI(const FrameEvent &evt) {

	if(!begin) {
		_oisManager->capture();

		OIS::KeyCode lastKey = _oisManager->lastKeyPressed();

		if(lastKey == OIS::KC_ESCAPE) {
			// close window when ESC is pressed
			mRunning = false;
		}
		return true;
	}
	else {
		return false;
	}
}

void Application::handleAi() {

	// For now AI just mirrors the player's paddle
	Ogre::Vector3 pos = _thePaddle->getNode()->getPosition();
	Ogre::Quaternion qt = _thePaddle->getNode()->getOrientation();

	Ogre::Vector3 ballPos = _theBall->getNode()->getPosition();

	// The ball is on the opponent's side of the net
	_otherPaddle->movePaddle(_oisManager, height, width, (float)ballPos.x, (float)ballPos.z + 500);

	Ogre::Vector3 ballNormal = _theBall->getNode()->getPosition() - _otherPaddle->getNode()->getPosition();

	if(ballNormal.length() <= 200) {
		ballNormal.normalise();
		ballNormal += Ogre::Vector3(0,0,0.5);
		int power = 3000;

		btVector3 impulse = btVector3(ballNormal.x*power, ballNormal.y*power, ballNormal.z*power);
		btVector3 rel_pos = btVector3(0,0,0);
		_theBall->applyImpulse(impulse, rel_pos);
	}
}

bool Application::updateServer(const FrameEvent &evt) {
	
	static float previousTime = t1->getMilliseconds();

	if ( netManager->pollForActivity(1) ) {
		previousTime = t1->getMilliseconds();
		// Only accept one connection at a time.
		netManager->denyConnections();
		std::unordered_map<std::string, char*> pairs = dataParser(netManager->udpClientData[0]->output);

		if(pairs["PDW"] == NULL || pairs["PDX"] == NULL || pairs["PDY"] == NULL || 
		   pairs["PDZ"] == NULL || pairs["PPX"] == NULL || pairs["PPY"] == NULL || 
		   pairs["PPZ"] == NULL ) {
		   		std::cout << "Paddle data integrity was not guaranteed." << std::endl;
		   		setState(HOME);
		   		return true;
		}
		else {
			float w = atof(pairs["PDW"]);
			float x = atof(pairs["PDX"]);
			float y = atof(pairs["PDY"]);
			float z = atof(pairs["PDZ"]);
			float paddleX = atof(pairs["PPX"]);
			float paddleY = atof(pairs["PPY"]);
			float paddleZ = atof(pairs["PPZ"]);

			Ogre::Quaternion qt(w,x,y,z);
			_otherPaddle->setOrientation(qt);
			_otherPaddle->setPosition(-paddleX, paddleY, -paddleZ);
			_otherPaddle->reflect();

			std::string ballCoords = _theBall->getCoordinates() + "\n" + _thePaddle->getCoordinates() + "\n" + _theBall->getPoints();
			netManager->messageClients(PROTOCOL_UDP, ballCoords.c_str(), ballCoords.length() + 1);
		}

	}
	else {
		float dt = t1->getMilliseconds() - previousTime;
		if(dt >= 2000 && netManager->getClients() > 0){
			setState(HOME);
			return false;
		}
	}
	return true;
}

/* All logic is now in update client. Update server will be implemented soon. */
bool Application::updateClient(const FrameEvent &evt) {

	static float previousTime = t1->getMilliseconds();
	
	if ( netManager->pollForActivity(1)) {
		std::unordered_map<std::string, char*> pairs = dataParser(netManager->udpServerData[0].output);

		if(pairs["BPX"] == NULL || pairs["BPY"] == NULL || 
			pairs["BPZ"] == NULL || pairs["BVX"] == NULL || pairs["BVY"] == NULL || 
			pairs["BVZ"] == NULL || pairs["PDW"] == NULL || pairs["PDX"] == NULL || pairs["PDY"] == NULL || 
		   	pairs["PDZ"] == NULL || pairs["PPX"] == NULL || pairs["PPY"] == NULL || 
		   	pairs["PPZ"] == NULL || pairs["SCS"] == NULL || pairs["SCC"] == NULL ) {
			   	std::cout << "Ball data integrity was not guaranteed." << std::endl;
			   	setState(HOME);
		   		return true;
		}
		else {
			float x = atof(pairs["BPX"]);
			float y = atof(pairs["BPY"]);
			float z = atof(pairs["BPZ"]);
			float vx = atof(pairs["BVX"]);
			float vy = atof(pairs["BVY"]);
			float vz = atof(pairs["BVZ"]);

			_theBall->setPosition(-x, y ,-(z + 0));
			_theBall->setVelocity(-vx, vy, -vz);

			float w = atof(pairs["PDW"]);
			x = atof(pairs["PDX"]);
			y = atof(pairs["PDY"]);
			z = atof(pairs["PDZ"]);
			float paddleX = atof(pairs["PPX"]);
			float paddleY = atof(pairs["PPY"]);
			float paddleZ = atof(pairs["PPZ"]);

			Ogre::Quaternion qt(w,x,y,z);
			_otherPaddle->setOrientation(qt);
			_otherPaddle->setPosition(-paddleX, paddleY, -paddleZ);
			_otherPaddle->reflect();

			float scs = atof(pairs["SCS"]); //Server Score
			float scc = atof(pairs["SCC"]); //Client Score
			gameManager->setScore(scc);
			gameManager->setEnemyScore(scs);
			gameManager->postScore();
			
		}
	}


	std::string t = _thePaddle->getCoordinates() + "\n" + _theBall->getPoints();

		if(t.length() - 1 > NetManager::MESSAGE_LENGTH) {
			std::cout << "Message was too large." << std::endl;
			return error();
		}

	netManager->messageServer(PROTOCOL_UDP, t.c_str(), t.length() + 1);

    return true;
}
/* 
* End Update Methods 
*/


/* 
* Create Object Methods 
*/
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

Ball* Application::createBall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {

	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale,scale,scale);
	ent->setMaterialName("ball");

	Ball* obj = new Ball(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Paddle* Application::createPaddle(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {

	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	sn->setScale(scale,scale,scale);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Paddle* obj = new Paddle(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Wall* Application::createWall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {

	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale.x, scale.y, scale.z);

	sn->pitch(pitch);
	sn->yaw(yaw);
	sn->roll(roll);

	Wall* obj = new Wall(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}
/*
* End Create Object Methods 
*/


/* 
* Initialization Methods
*/
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

void Application::setupResources(void) {

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

void Application::setupCEGUI(void) {

	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*mRenderWindow);
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("AlfiskoSkin/MouseArrow");

	CEGUI::FontManager &fmg = CEGUI::FontManager::getSingleton();
	CEGUI::Font &font = fmg.createFreeTypeFont("arial", 20, true, "arial.ttf");
	
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "_MasterRoot");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

	CEGUI::Window* quitButton = wmgr.createWindow("AlfiskoSkin/Button", "QuitButton");
	quitButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.05f, 0))));
	quitButton->setText("Quit");

	ipWindow = wmgr.createWindow("AlfiskoSkin/Label", "ipWindow");
	ipWindow->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.92f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(1, 0))));
	ipWindow->setText(netManager->getIPstring());

	singlePlayerButton = wmgr.createWindow("AlfiskoSkin/Button", "SinglePlayerButton");
	singlePlayerButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.35f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.4f, 0))));
	singlePlayerButton->setText("Single Player");

	hostServerButton = wmgr.createWindow("AlfiskoSkin/Button", "HostButton");
	hostServerButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.4f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.45f, 0))));
	hostServerButton->setText("Host Game");

	ipText = wmgr.createWindow("AlfiskoSkin/Label", "Ip Label");
	ipText->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.525f, 0), CEGUI::UDim(0.45f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.725f, 0), CEGUI::UDim(0.5f, 0))));
	ipText->setText("IP Address");

	ipBox = wmgr.createWindow("AlfiskoSkin/Editbox", "Ip Box");
	ipBox->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.45f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.5f, 0))));

	joinServerButton = wmgr.createWindow("AlfiskoSkin/Button", "JoinButton");
	joinServerButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.5f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.55f, 0))));
	joinServerButton->setText("Join Game");

	howToButton = wmgr.createWindow("AlfiskoSkin/Button", "HowToButton");
	howToButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.3f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.35f, 0))));
	howToButton->setText("How To Play");

	howToText = wmgr.createWindow("AlfiskoSkin/MultiLineEditbox", "Instructions");
	howToText->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.125f, 0), CEGUI::UDim(0.35f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.875f, 0), CEGUI::UDim(0.75f, 0))));
	howToText->setText(instructions);
	static_cast<CEGUI::MultiLineEditbox*>(howToText)->setReadOnly(true);

	homeButton = wmgr.createWindow("AlfiskoSkin/Button", "HomeButton");
	homeButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.0f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.05f, 0))));
	homeButton->setText("Home");

	replayButton = wmgr.createWindow("AlfiskoSkin/Button", "ReplayButton");
	replayButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.4f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.45f, 0))));
	replayButton->setText("Watch Replay");

	sheet->addChild(singlePlayerButton);
	sheet->addChild(hostServerButton);
	sheet->addChild(joinServerButton);
	sheet->addChild(quitButton);
	sheet->addChild(ipBox);
	sheet->addChild(ipText);
	sheet->addChild(ipWindow);
	sheet->addChild(homeButton);
	sheet->addChild(replayButton);
	sheet->addChild(howToButton);
	sheet->addChild(howToText);

	replayButton->hide();
	howToText->hide();

	singlePlayerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::StartSinglePlayer, this));
	hostServerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::StartServer, this));
	joinServerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::JoinServer, this));
	quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::Quit, this));
	homeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::Home, this));
	replayButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::Replay, this));
	howToButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::HowTo, this));

}

void Application::setupCameras(void) {

	mCamera = mSceneManager->createCamera("Main Camera");
	Ogre::Camera* cam2 = mSceneManager->createCamera("Cam2");
	ballCam = mSceneManager->createCamera("Ball Cam");
	camMan = mSceneManager->createCamera("Camera Man");

	// Add viewport and cameras
	mRenderWindow->addViewport(mCamera);

	mCamera->setAutoAspectRatio(true);
	mCamera->setPosition(0, 120, 1800);

	cam2->setAutoAspectRatio(true);
	cam2->setPosition(1350, 0, -400);
	cam2->yaw(Ogre::Degree(90));
	cam2->pitch(Ogre::Degree(15));

	ballCam->setAutoAspectRatio(true);
	ballCam->setPosition(0, 120, 1800);

	camMan->setAutoAspectRatio(true);
	camMan->setPosition(0,120,400);
	camMan->lookAt(0,120,1800);

	cameras = std::vector<Ogre::Camera*>();
	cameras.push_back(mCamera);
	cameras.push_back(cam2);
	cameras.push_back(ballCam);
	cameras.push_back(camMan);

	cameraMan = new OgreBites::SdkCameraMan(camMan);
	_oisManager->setupCameraMan(cameraMan);

}

/* Setup GameManager */
void Application::setupGM(void) {

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	gameManager = new GameManager();
	gameManager->addRenderer(mRenderer);
	gameManager->startMusic();

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

}

void Application::createObjects(void) {
	
	mSceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	// This paddle gets a negative Z coordinate that becomes positive in the function on movepaddle
	_thePaddle = createPaddle("paddle", GameObject::objectType::PADDLE_OBJECT, "paddle.mesh", 0, 0, -825, 100, mSceneManager, gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);
	_otherPaddle = createPaddle("other_paddle", GameObject::objectType::PADDLE_OBJECT, "paddle-blue.mesh", 0, 0, 825, 100, mSceneManager, gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);
	_theBall = createBall("ball", GameObject::objectType::BALL_OBJECT, "sphere.mesh", 5, 300, 800, .35, mSceneManager, gameManager, 1.0f, 1.0f, 0.8f, false, _simulator);

	createWall("floor", GameObject::objectType::FLOOR_OBJECT, "floor.mesh", 0, -100, 0, Ogre::Vector3(120, 240, 240), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("ceiling", GameObject::objectType::WALL_OBJECT, "ceiling.mesh", 0, 500, 0, Ogre::Vector3(120, 240, 240), Ogre::Degree(180), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, gameManager, 0.0f, 0.5f, 0.8f, false, _simulator);
	createWall("backwall", GameObject::objectType::BACK_WALL_OBJECT, "backwall.mesh", 0, 300, -1200, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(0), mSceneManager, gameManager, 0.0f, 0.8f, 0.8f, false, _simulator);
	createWall("leftwall", GameObject::objectType::WALL_OBJECT, "leftwall.mesh", 600, 0, -430, Ogre::Vector3(120, 120, 400), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(90), mSceneManager, gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("rightwall", GameObject::objectType::WALL_OBJECT, "rightwall.mesh", -600, 0, -430, Ogre::Vector3(120, 120, 400), Ogre::Degree(0), Ogre::Degree(0), Ogre::Degree(-90), mSceneManager, gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("frontwall", GameObject::objectType::FRONT_WALL_OBJECT, "backwall.mesh", 0, 300, 1200, Ogre::Vector3(120, 120, 120), Ogre::Degree(90), Ogre::Degree(0), Ogre::Degree(180), mSceneManager, gameManager, 0.0f, 0.9f, 0.8f, false, _simulator);

	createRootEntity("stadium", "stadium2.mesh", 0, -592, 0);
	mSceneManager->getSceneNode("stadium")->setScale(100,100,100);
	mSceneManager->getSceneNode("stadium")->yaw(Ogre::Degree(90));
	stadiumFlashes = new StadiumFlashes(mSceneManager);

	_theBall->startScore();

}
/* 
* End Initialization Methods
*/


/* 
*CEGUI Button Callbacks 
*/

bool Application::StartSinglePlayer(const CEGUI::EventArgs &e) {

	begin = true;
	setState(SINGLE);

	gameManager->setServer(true);
	return true;
}

bool Application::StartServer(const CEGUI::EventArgs& e) {

	begin = true;
	setState(SERVER);

	gameManager->setServer(gameState == SERVER);

	if (!setupNetwork(gameState == SERVER)) {
		return error();
	}
	else {
		// Server will not bind connecting UDP clients without this method
		netManager->acceptConnections();
		return true;
	}
}

bool Application::JoinServer(const CEGUI::EventArgs& e) {

	begin = true;
	setState(CLIENT);

	gameManager->setServer(gameState == SERVER);

	if(!setupNetwork(gameState == SERVER)) {
		setState(HOME);
		return true;
	}
	else {
		return true;
	}
}

bool Application::Quit(const CEGUI::EventArgs& e) {

	mRunning = false;
    return true;
}

bool Application::Replay(const CEGUI::EventArgs &e) {

	setState(REPLAY);
	return true;
}

bool Application::Home(const CEGUI::EventArgs &e) {

	setState(HOME);
	return true;
}

bool Application::HowTo(const CEGUI::EventArgs &e) {
	setState(HOWTO);
	return true;
}

bool Application::setupNetwork(bool isServer) {

	netManager = new NetManager();

	if(!netManager->initNetManager()) {
		std::cout << "Failed to init the server!" << std::endl;
		return false;
	}
	else {
		// Opens a connection on port 51215
		netManager->addNetworkInfo(PROTOCOL_UDP, isServer ? NULL : ipBox->getText().c_str(), 51215);
	}
	if (isServer) {
		if(!netManager->startServer()) {
			std::cout << "Failed to start the server!" << std::endl;
			return false;
		}
		return true;
	}
	else {
		if(!netManager->startClient()) {
			std::cout << "Failed to start the client!" << std::endl;
			return false;
		}
		return true;
	}

}

bool Application::error() {

	mRunning = false;
	return false;
}

std::unordered_map<std::string, char*> Application::dataParser(char* buf) {

	if(buf == NULL) {
		std::cout << "Buffer is empty!" << std::endl;
		return std::unordered_map<std::string, char*>();
	}

	std::unordered_map<std::string, char*> kvpairs;
    char *end_str;

    char *token = MultiPlatformHelper::strtok(buf, "\n", &end_str);

    while (token != NULL)
    {
        char *end_token;
		char *token2 = MultiPlatformHelper::strtok(token, " ", &end_token);

        std::vector<char*> info;
        while (token2 != NULL)
        {
    		info.push_back(token2);
			token2 = MultiPlatformHelper::strtok(NULL, " ", &end_token);
        }

        char* key = info[0];
        char* value = info[1];
        kvpairs[std::string(key)] = value;

		token = MultiPlatformHelper::strtok(NULL, "\n", &end_str);
    }

    return kvpairs;
}

void Application::hideGui() {

	singlePlayerButton->hide();
	hostServerButton->hide();
	joinServerButton->hide();
	ipBox->hide();
	ipText->hide();
	ipWindow->hide();
	gameManager->hideGameOver();
	replayButton->hide();
	howToButton->hide();
	howToText->hide();
}

void Application::showGui() {

	singlePlayerButton->show();
	hostServerButton->show();
	joinServerButton->show();
	ipBox->show();
	ipText->show();
	ipWindow->show();
	howToButton->show();
}

void Application::showEndGui() {
	gameManager->showGameOver();
	replayButton->show();
}

void Application::resetNetManager() {
	if(netManager) {
		delete netManager;
		netManager = new NetManager();
	} 
}

// This method is to ensure that the entire state is correct before transitioning to that new state
void Application::setState(State state) {

	switch(state) {
		case HOME:
			resetNetManager();
			states.clear();
			hideGui();
			showGui();
			gameManager->resetScore();
			mRenderWindow->removeAllViewports();
			mRenderWindow->addViewport(cameras[0]);
			_theBall->reset();
			gameState = HOME;
			break;
		case SERVER:
			resetNetManager();
			hideGui();
			gameState = SERVER;
			break;
		case CLIENT:
			resetNetManager();
			hideGui();
			gameState = CLIENT;
			break;
		case SINGLE:
			hideGui();
			gameState = SINGLE;
			break;
		case ENDGAME:
			hideGui();
			showEndGui();
			gameState = ENDGAME;
			break;
		case REPLAY:
			gameManager->resetScore();
			hideGui();
			gameState = REPLAY;
			break;
		case HOWTO:
			hideGui();
			gameState = HOWTO;
			howToText->show();
			break;
	}
}

void Application::replayData() {

	GameState& s = states.front();
	_thePaddle->setOrientation(s._thePaddle);
	_otherPaddle->setOrientation(s._otherPaddle);
	_thePaddle->setPosition(s._thePaddlePos);
	_otherPaddle->setPosition(s._otherPaddlePos);
	_theBall->setPosition(s._ballPos);
	_theBall->setVelocity(s._velocity.x(), s._velocity.y(), s._velocity.z());
	gameManager->setScore(s._gameScore);
	gameManager->setEnemyScore(s._opponentScore);

	if(!states.empty())
		states.pop_front();
}