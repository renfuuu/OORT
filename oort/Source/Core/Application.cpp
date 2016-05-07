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

std::string instructions = "* Press C - change the cameras\n* Press M - mute music\n* Press ESC to quit the game\n* Press W & S control the pitch of the Spaceship\n* Press A & D control the yaw  of the Spaceship\n* Press Q & E control the roll of the Spaceship\n* Press left mouse button to fire laser\n* Press Mouse Scroll to throttle up and down the Spaceship's trust to move it along it's current trajectory";

#define MIN_NUM_ASTEROIDS 20

Application::Application():
	camChange(0),
	laserCount(0),
	asteroidCount(0),
	respawnN(MIN_NUM_ASTEROIDS)
{
}

Application::~Application()
{
}

void Application::init()
{

	try{
		t1 = new Timer();

		
		srand(time(0));

		_simulator = new Simulator();
		
		setupWindowRendererSystem();

		setupOIS();

		setupCEGUI();

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



	// std::ifstream file( "../Assets/Scripts/dream.xml" );
 //    std::stringstream buffer;

 //    if ( file )
 //    {

 //        buffer << file.rdbuf();

 //        file.close();

 //        // operations on the buffer...
 //    }
 //    else
 //    {
 //    	std::cout << "Could not find file" << std::endl;
 //    }
	// std::cout << "********************" << std::endl;

    // std::cout << buffer.str().size();

	mSlicer = new MeshSlicer(NULL);
	
	XML_Mesh* ms  = new XML_Mesh;
	std::string meshfile = "../Assets/Asteroid/Stone_01.mesh.xml";
	
	ms->loadFromXMLFile(meshfile);
	ms->toFile("savedmesh1");

	mSlicer->loadMesh(ms);
	std::vector<XML_Mesh*> meshes;
	mSlicer->sliceByPlane(meshes, vec3f(0.0f,0.0f,0.0f), vec3f(0.0f, 0.0f, 1.0f));


	meshes[0]->toFile("slicedsavedmesh1");
}


bool Application::frameRenderingQueued(const FrameEvent &evt)
{
	static float gameOverTime = 0.0f;
	static float dTime = t1->getMilliseconds();
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

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
		// close window when ESC is pressed
	}
	catch (Exception e) {

	}
	// Code per frame in fixed FPS
	float temp = t1->getMilliseconds();
	if ((temp - dTime) >= (1.0 / fps)*1000.0) {
		if( _gameManager->isGameOver() ) {
			gameOverTime += (temp - dTime);
		}
		update(evt);
		dTime = temp;
	}



	// Constrains the laser's speed
	static int maxSpeed = 4000;
	for (std::vector<Laser*>::iterator i = lasers.begin(); i != lasers.end(); ++i)
	{
		btVector3 velocity = (*i)->getBody()->getLinearVelocity();
	    btScalar speed = velocity.length();
	    if(speed > maxSpeed) {
	        velocity *= maxSpeed/speed;
	        (*i)->getBody()->setLinearVelocity(velocity);
	    }
	}

	cameraMan->frameRenderingQueued(evt);


	return true;
}

// Called once per predefined frame
bool Application::update(const FrameEvent &evt) {

	static float dTime = t1->getMilliseconds();
	static float dTme = t1->getMilliseconds();

	OIS::KeyCode lastKey = _oisManager->lastKeyPressed();

		if(lastKey == OIS::KC_SPACE) {
		// _theBall->resetScore();
	}

	else if (lastKey == OIS::KC_M) {
		_gameManager->mute();
	}
	else if (lastKey == OIS::KC_C) {
		camChange ++;
		mRenderWindow->removeAllViewports();
		mRenderWindow->addViewport(cameras[camChange % cameras.size()]);
		if(cameras[camChange % cameras.size()]->getName() == "Camera Man"){
			camMan->setPosition(_theSpaceship->getNode()->getPosition() + Ogre::Vector3(0,0,450));
			camMan->lookAt(_theSpaceship->getNode()->getPosition());
		}
	}
	else if(lastKey == OIS::KC_ESCAPE)
			mRunning = false;

	switch(gameState) {
		case HOME:
			handleGUI(evt);
			return true;
			break;
		case SINGLE:
			break;
		case ENDGAME:
			return true;
			break;
		case HOWTO:
			return true;
			break;
	}

	if ( !(_gameManager->isGameOver()) ) {
		_simulator->stepSimulation(evt.timeSinceLastFrame, 1, 1.0 / fps);
	}
	else {
		// _gameManager->showGameOver();
		// if(gameOverTime > 2000) {
		// 	_gameManager->resetGameOver();
		// 	_gameManager->hideGameOver();
		// 	gameOverTime = 0.0f;
		// }
		setState(ENDGAME);
	}



	if(cameras[camChange % cameras.size()]->getName() == "Spaceship Cam"){
		_theSpaceship->moveSpaceship(_oisManager, height, width, _camNode);
	

		//Limit the time to shoot the lasers
		float temp = t1->getMilliseconds();
		float tts = 250.0;
		if ((temp - dTime) >= tts) {
			if(_oisManager->getMouse()->getMouseState().buttonDown(OIS::MB_Left)){
				laserCount ++;
				_gameManager->playSound(SoundManager::LASER_SHOT);
				Ogre::String name = "Laser_" + std::to_string(laserCount);
				createLaser(name, GameObject::objectType::LASER_OBJECT, "RectLaser.mesh", _theSpaceship, Ogre::Vector3(10, 45, 60), mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);
				dTime = temp;
			}
		}


		for (int li = 0; li < lasers.size(); li++){
			if(lasers[li]->alive && lasers[li]->getNode()->getPosition().length() < 15000){
				lasers[li]->moveLaser();
			}
			else{
				// Delete the laser 
				Ogre::String entName = lasers[li]->getName();
				lasers[li]->getNode()->detachAllObjects();
				lasers[li]->getNode()->removeAndDestroyAllChildren();
				mSceneManager->destroyEntity(entName);
				mSceneManager->destroyEntity("Particle_"+entName);
				lasers.erase(lasers.begin() + li);
				li--;
			}
		}


		float tmp = t1->getMilliseconds();
		float ttl = 3000;
		for (int ai = 0; ai < asteroids.size(); ai++){
			if(asteroids[ai]->alive){
				asteroids[ai]->moveAsteroid(_theSpaceship->getNode());
			}
			else{
				if((tmp - dTme) >= ttl){
					// Delete the asteroid from scene 
					Ogre::String entName = asteroids[ai]->getName();
					asteroids[ai]->getNode()->detachAllObjects();
					mSceneManager->destroyEntity(entName);
					asteroids.erase(asteroids.begin() + ai);
					ai--;
					dTme = tmp;
				}
			}
		}

		//Spawn new Asteroids
		if(asteroids.size() <= 5){
			respawnN ++;
			for(int i = 0; i < respawnN; i++){
				asteroidCount++;
				float posX = (float)(-10000 + (rand() % (int)(15000 - (-10000) + 1)));
				float posY = (float)(rand() % (int)(15000 + 1));
				float posZ = (float)(-10000 + (rand() % (int)(15000 - (-10000) + 1)));

				// Ogre::Vector3 loc(posX,posY,posZ);
				// std::cout << "Respawn Asteroid Location: " << loc << std::endl; 

				Ogre::Vector3 rotate((float)(rand() % 181),(float)(rand() % 181),(float)(rand() % 181));

				// if(asteroidCount % 2 == 0){
					createAsteroid("Asteroid_" + std::to_string(asteroidCount), GameObject::objectType::ASTEROID_OBJECT, "Stone_01.mesh", Ogre::Vector3(posX, posY, posZ), rotate, 15, mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);
				// }
				// else{
					// createAsteroid("Asteroid_" + std::to_string(asteroidCount), GameObject::objectType::ASTEROID_OBJECT, "Stone_04.mesh", Ogre::Vector3(posX, posY, posZ), rotate, 15, mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);
				// }
			}
		}

	}


	if(!_theSpaceship->alive){
		setState(ENDGAME);
		_gameManager->playSound(SoundManager::GAME_LOSS);
		Ogre::String entName = _theSpaceship->getName();
	}	
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

Spaceship* Application::createSpaceship(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, x, y, z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	sn->setScale(scale,scale,scale);
	sn->yaw(Ogre::Degree(180));
	// sn->showBoundingBox(true);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Spaceship* obj = new Spaceship(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Wall* Application::createWall(Ogre::String nme, GameObject::objectType tp, std::string type, int width, int height, Ogre::Vector3 position, Ogre::Vector3 rotate, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {


	//create a mesh blueprint
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    //create mesh
    if(type == "upDown"){
	    Ogre::MeshManager::getSingleton().createPlane("upDown",
	      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	      plane, 
	      width, height, 20, 20, 
	      true, 
	      1, 5, 5, 
	      Ogre::Vector3::UNIT_Z);
    }else{
	    plane.normal = Ogre::Vector3::UNIT_Z;

	    Ogre::MeshManager::getSingleton().createPlane("sides",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	     plane,
	     width, height,20,20,
	     true,
	     1,5,5,
	     Ogre::Vector3::UNIT_Y);
    }

    Ogre::Entity* ent = mSceneManager->createEntity(type);
	ent->setMaterialName("wall");
	ent->setCastShadows(true);
	Ogre::SceneNode* sn = mSceneManager->getRootSceneNode()->createChildSceneNode(nme);
	sn->attachObject(ent);
	sn->setPosition(position.x, position.y, position.z);

	Ogre::Vector3 n = plane.normal;

	Ogre::Quaternion qrx(Ogre::Degree(rotate.x), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion qry(Ogre::Degree(rotate.y), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion qrz(Ogre::Degree(rotate.z), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion qr = qrx * qry * qrz;

	Ogre::Matrix3 transform;
	qr.ToRotationMatrix(transform);

	n = transform * n;

	sn->pitch(Ogre::Degree(rotate.x));
	sn->yaw(Ogre::Degree(rotate.y));
	sn->roll(Ogre::Degree(rotate.z));

	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Ogre::Vector3 scale(1,1,1);

	Wall* obj = new Wall(nme, tp, mSceneManager, ssm, sn, ent, n, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();

	return obj;
}

Laser* Application::createLaser(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, GameObject* sship, Ogre::Vector3 scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {

	auto sspos = sship->getNode()->getPosition();
	createRootEntity(nme, meshName, sspos.x, sspos.y, sspos.z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);
	sn->setScale(scale.x, scale.y, scale.z);
	sn->roll(dynamic_cast<Spaceship*>(sship)->rollAngle);
	sn->yaw(dynamic_cast<Spaceship*>(sship)->yawAngle);
	sn->pitch(dynamic_cast<Spaceship*>(sship)->pitchAngle);
	Ogre::Vector3 look = sship->getNode()->getOrientation().zAxis();
	sn->translate(175.0f * look);
	ent->setMaterialName("Laser");

	// sn->showBoundingBox(true);

	Laser* obj = new Laser(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();
	obj->velocity = _theSpaceship->getNode()->getOrientation().zAxis();
	lasers.push_back(obj);

	return obj;
}

Asteroid* Application::createAsteroid(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 rotate, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim) {
	createRootEntity(nme, meshName, position.x, position.y, position.z);
	Ogre::SceneNode* sn = mSceneManager->getSceneNode(nme);
	Ogre::Entity* ent = SceneHelper::getEntity(mSceneManager, nme, 0);
	sn->setScale(scale,scale,scale);
	sn->roll(Ogre::Degree(rotate.x));
	sn->yaw(Ogre::Degree(rotate.y));
	sn->pitch(Ogre::Degree(rotate.z));
	// sn->showBoundingBox(true);
	const btTransform pos;
	OgreMotionState* ms = new OgreMotionState(pos, sn);

	Asteroid* obj = new Asteroid(nme, tp, mSceneManager, ssm, sn, ent, ms, mySim, mss, rest, frict, scale, kinematic);
	obj->addToSimulator();
	asteroids.push_back(obj);

	return obj;
}

void Application::generateAsteroids(int n){
	for(int i = 0; i < n; i++){
		asteroidCount++;
		float posX = (float)(rand() % 14750 - 7500);
		float posY = (float)(rand() % 14750);
		float posZ = (float)(rand() % 14750 - 7500);

		Ogre::Vector3 rotate((float)(rand() % 181),(float)(rand() % 181),(float)(rand() % 181));

		// if(asteroidCount % 2 == 0){
			createAsteroid("Asteroid_" + std::to_string(asteroidCount), GameObject::objectType::ASTEROID_OBJECT, "Stone_01.mesh", Ogre::Vector3(posX, posY, posZ), rotate, 15, mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);
		// }
		// else
			// createAsteroid("Asteroid_" + std::to_string(asteroidCount), GameObject::objectType::ASTEROID_OBJECT, "Stone_04.mesh", Ogre::Vector3(posX, posY, posZ), rotate, 15, mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);

	}
}

void Application::clearAsteroids(){
	for (std::vector<Asteroid*>::iterator i = asteroids.begin(); i != asteroids.end(); ++i)
	{
		// Delete the asteroid
		Ogre::String entName = (*i)->getName();
		(*i)->getNode()->detachAllObjects();
		mSceneManager->destroyEntity(entName);
	}

	// asteroidCount = 0;
	asteroids.clear();
}

void Application::clearLasers(){
	for (std::vector<Laser*>::iterator i = lasers.begin(); i != lasers.end(); ++i)
	{
		// Delete the laser
		Ogre::String entName = (*i)->getName();
		(*i)->getNode()->detachAllObjects();
		(*i)->getNode()->removeAndDestroyAllChildren();
		mSceneManager->destroyEntity(entName);
		mSceneManager->destroyEntity("Particle_"+entName);

	}

	lasers.clear();
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


	singlePlayerButton = wmgr.createWindow("AlfiskoSkin/Button", "SinglePlayerButton");
	singlePlayerButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.3f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.35f, 0))));
	singlePlayerButton->setText("Play the Game");


	howToButton = wmgr.createWindow("AlfiskoSkin/Button", "HowToButton");
	howToButton->setArea(CEGUI::URect(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.35f, 0)),
		CEGUI::UVector2(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.4f, 0))));
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


	sheet->addChild(singlePlayerButton);
	sheet->addChild(quitButton);
	sheet->addChild(homeButton);
	sheet->addChild(howToButton);
	sheet->addChild(howToText);

	howToText->hide();

	singlePlayerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::StartSinglePlayer, this));
	quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::Quit, this));
	homeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::Home, this));
	howToButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::HowTo, this));

}

void Application::setupCameras(void) {

	spaceshipCam = mSceneManager->createCamera("Spaceship Cam");
	camMan = mSceneManager->createCamera("Camera Man");


	// Add viewport and cameras
	mRenderWindow->addViewport(spaceshipCam);

	spaceshipCam->setAutoAspectRatio(true);
	spaceshipCam->setPosition(0, 120, 1800);


	camMan->setAutoAspectRatio(true);
	camMan->setPosition(0,120,400);
	camMan->lookAt(0,120,1800);

	cameras = std::vector<Ogre::Camera*>();
	cameras.push_back(spaceshipCam);
	cameras.push_back(camMan);

	cameraMan = new OgreBites::SdkCameraMan(camMan);
	_oisManager->setupCameraMan(cameraMan);

}

void Application::setupGameManager(void) {

	mRoot->addFrameListener(this);
	WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
	mRenderWindow->addListener(this);

	_gameManager = new GameManager();
	_gameManager->addRenderer(mRenderer);
	_gameManager->startMusic();

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
	
	mSceneManager->setSkyDome(true, "Examples/SpaceSkyPlane", 5, 8);

	_theSpaceship = createSpaceship("spaceship", GameObject::objectType::SPACESHIP_OBJECT, "Plane.mesh", 0, 600, 600, 50, mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, true, _simulator);

	//setup a forever orbit camera scenenode
	_camNode = _theSpaceship->getNode()->createChildSceneNode("Cam Node");
	_camNode->attachObject(spaceshipCam);
	_camNode->translate(Ogre::Vector3(0,15,-75));
	spaceshipCam->lookAt(_theSpaceship->getNode()->getPosition());

	//creating walls
	createWall("floor", GameObject::objectType::UP_DOWN_WALL_OBJECT, "upDown", 15000, 15000, Ogre::Vector3(0,0,0), Ogre::Vector3(0,0,0), mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("ceiling", GameObject::objectType::UP_DOWN_WALL_OBJECT, "upDown", 15000, 15000, Ogre::Vector3(0,15000,0), Ogre::Vector3(180,0,0), mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("leftwall", GameObject::objectType::SIDE_WALL_OBJECT, "sides", 15000, 15000, Ogre::Vector3(-7500,7500,0), Ogre::Vector3(0,90,0), mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("frontwall", GameObject::objectType::SIDE_WALL_OBJECT, "sides", 15000, 15000, Ogre::Vector3(0,7500,7500), Ogre::Vector3(0,180,0), mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("backwall", GameObject::objectType::SIDE_WALL_OBJECT, "sides", 15000, 15000, Ogre::Vector3(0,7500,-7500), Ogre::Vector3(0,0,0), mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);
	createWall("rightwall", GameObject::objectType::SIDE_WALL_OBJECT, "sides", 15000, 15000, Ogre::Vector3(7500,7500,0), Ogre::Vector3(0,270,0), mSceneManager, _gameManager, 0.0f, 1.0f, 0.8f, false, _simulator);

	generateAsteroids(MIN_NUM_ASTEROIDS);
}

/* 
*CEGUI Button Callbacks 
*/

bool Application::StartSinglePlayer(const CEGUI::EventArgs &e) {

	begin = true;
	setState(SINGLE);
	_gameManager->playSound(SoundManager::MENU);
	// CEGUI::MouseCursor::getSingleton().hide();

	return true;
}

bool Application::Quit(const CEGUI::EventArgs& e) {

	mRunning = false;
	_gameManager->playSound(SoundManager::MENU);
    return true;
}

bool Application::Home(const CEGUI::EventArgs &e) {

	setState(HOME);
	_gameManager->playSound(SoundManager::MENU);
	// CEGUI::MouseCursor::getSingleton().show();
	return true;
}

bool Application::HowTo(const CEGUI::EventArgs &e) {
	setState(HOWTO);
	_gameManager->playSound(SoundManager::MENU);
	return true;
}


void Application::hideGui() {

	singlePlayerButton->hide();
	// hostServerButton->hide();
	// joinServerButton->hide();
	// ipBox->hide();
	// ipText->hide();
	// ipWindow->hide();
	_gameManager->hideGameOver();
	// replayButton->hide();
	howToButton->hide();
	howToText->hide();
}

void Application::showGui() {

	singlePlayerButton->show();
	// hostServerButton->show();
	// joinServerButton->show();
	// ipBox->show();
	// ipText->show();
	// ipWindow->show();
	howToButton->show();
}

void Application::showEndGui() {
	_gameManager->showGameOver();
	// replayButton->show();
}

void Application::setState(State state) {


	switch(state) {
		case HOME:
			// resetNetManager();
			// states.clear();
			hideGui();
			showGui();
			clearAsteroids();
			clearLasers();
			_gameManager->resetScore();
			mRenderWindow->removeAllViewports();
			mRenderWindow->addViewport(cameras[0]);
			_theSpaceship->reset();
			_theSpaceship->setStatus(true);
			_theSpaceship->addToSimulator();
			dynamic_cast<Spaceship*>(_theSpaceship)->setVelocity(1.0);
			generateAsteroids(MIN_NUM_ASTEROIDS);
			gameState = HOME;
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
		// case REPLAY:
		// 	_gameManager->resetScore();
		// 	hideGui();
		// 	gameState = REPLAY;
		// 	break;
		case HOWTO:
			hideGui();
			gameState = HOWTO;
			howToText->show();
			break;
	}

}

