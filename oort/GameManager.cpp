#include "GameManager.h"
#define PRINT(x) std::cout<< x <<std::endl



GameManager::GameManager(void) {
	//creates GameManager.
	cout << "Creating GameManager" << endl;
	playEnabled = true;
	gameOver = false;
	isGuest = false;
	mixer = SoundManager::getInstance();
	mixer->initAudio();
	char* sound = "media/audio/background.ogg";
	backgroundSound = mixer->onLoad(sound);
	mixer->playBackground();
	NetMan = new NetManager();
	currentState = SINGLE;

}

GameManager::~GameManager(void) {
	cout << "Deleting GameManager. Releasing resources...";
	//release resource here
	if (marbleBall) {
		delete marbleBall;
		marbleBall = 0;
	}

	if (simulator) {
		delete simulator;
		simulator = 0;
	}

	if (room) {
		delete room;
		room = 0;
	}
	cout << "Done." << endl;
}

void GameManager::createCameras()
{
	Ogre::Vector3 temp = Ogre::Vector3(1850, 1780, 3050);
	hostPos = temp;
	temp = Ogre::Vector3(1350, 1700, -3200);
	guestPos = temp;
}

void GameManager::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
	// Set initial mouse clipping size
	windowResized(mWindow);
	// Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	mRoot->addFrameListener(this);
	mWindow->setDeactivateOnFocusChange(false);
}

std::string GameManager::createClientPacket()
{
	std::ostringstream oss;

	oss << theGoal->rootNode->getPosition().x;
	oss << " ";
	oss << theGoal->rootNode->getPosition().y;
	oss << " ";

	return oss.str();
}


std::string GameManager::createServerPacket()
{
	std::ostringstream oss;
	oss << playerPaddle->rootNode->getPosition().x;
	oss << " ";
	oss << playerPaddle->rootNode->getPosition().y;
	oss << " ";
	oss << marbleBall->rootNode->getPosition().x;
	oss << " ";
	oss << marbleBall->rootNode->getPosition().y;
	oss << " ";
	oss << marbleBall->rootNode->getPosition().z;
	oss << " ";
	oss << marbleBall->score;
	oss << " ";
	oss << marbleBall->lives;
	oss << " ";
	oss << gameOver ? 1 : 0;
	oss << " ";
	return oss.str();
}

bool GameManager::setupMultiplayer()
{
	//setup


	PRINT("IN MULTIPLAYER");

	if (!NetMan->initNetManager())
	{
		PRINT("FAILED TO INIT NetManager");
		return false;
	}
	else
	{

		marbleBall->score = 0;
		marbleBall->lives = Ball::BEGIN_LIVES;
		timer.start();

		std::string temp = IPBox->getText().c_str();

		NetMan->addNetworkInfo(PROTOCOL_UDP, isGuest ? temp.c_str() : NULL, 51215);
		if (!isGuest)
		{
			NetMan->stopServer();
			if (!NetMan->startServer())
			{

				PRINT("startServer failed");
				return false;
			}
			PRINT("startServer worked");
			NetMan->acceptConnections();
			return true;
		}
		else
		{
			NetMan->stopClient();
			if (!NetMan->startClient())
			{
				PRINT("startClient failed");
				return false;
			}
			PRINT("startClient worked");
			return true;
		}
	}
}

bool GameManager::handleClient(const Ogre::FrameEvent& evt)
{
	// PRINT("handle Client");

	std::string temp  = createClientPacket();

	NetMan->messageServer(PROTOCOL_UDP, temp.c_str(), temp.size());

	if (NetMan->pollForActivity(1)) {
		std::istringstream iss(NetMan->udpServerData[0].output);
		float x, y;
		iss >> x >> y;
		float z = playerPaddle->rootNode->getPosition().z;
		playerPaddle->moveTo(Ogre::Vector3(x, y, z));

		float bx, by, bz;
		iss >> bx >> by >> bz;
		marbleBall->rootNode->setPosition(Ogre::Vector3(bx, by, bz));
		int s, l;
		iss >> s >> l;
		marbleBall->score = s;
		marbleBall->lives = l;
		int flag;
		iss >> flag;
		if (flag == 1)
		{
			gameOver = true;
		}
		else if (flag == 0)
		{
			gameOver = false;
		}
	} 
	// else {
	// 	// PRINT("no data");
	// }

	bool var = handleMultiPlayer(evt);
	return var;
}

bool GameManager::handleServer(const Ogre::FrameEvent& evt)
{
	// PRINT("handle Server");


	std::string temp = createServerPacket();

	NetMan->messageClients(PROTOCOL_UDP, temp.c_str(), temp.size());

	if (NetMan->pollForActivity(1)) {
		std::istringstream iss(NetMan->udpClientData[0]->output);
		float x, y;
		iss >> x >> y;
		float z = theGoal->rootNode->getPosition().z;
		theGoal->moveTo(Ogre::Vector3(x, y, z));
	} 
	// else {
	// 	PRINT("no data");
	// }
	bool var = handleMultiPlayer(evt);
	return var;
}

bool GameManager::handleSinglePlayer(const Ogre::FrameEvent& evt)
{
	if (cameraFollow)
	{
		if (!isGuest)
		{
			mCameraMan->setYawPitchDist((Ogre::Radian) 0.0f, (Ogre::Radian) 0.6f, (Ogre::Real) 1500.0f);
		}
		else
		{
			mCameraMan->setYawPitchDist((Ogre::Radian) 3.14f, (Ogre::Radian) 0.6f, (Ogre::Real) 1500.0f);
		}
	}


	scoreBox->setText(SSTR(marbleBall->score));
	liveBox->setText( SSTR(marbleBall->lives));
	timeBox->setText( SSTR(timer.getIntTime()));
	playerBox->setText("SINGLE");


	if (marbleBall->lives <= 0)
	{
		gameOver = true;
		marbleBall->lives = 0;
		playEnabled = false;
		// livesText->setText(SSTR("GAME OVER :("));
	}

	theGoal->move(evt);
	//TODO make this into a function call for unbuffered input
	//TODO need to make a parent node for the paddle otherwise camera spins around paddle
	//TODO get scene node up here
	// processUnbufferedInput(evt);

	if (gameOver)
	{
		// infoBox->setText("Game Over. Press esc to quit");
		gameOverBox->show();

		// mWindow->setActive(false);
	}

	//step simulation
	simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1 / 60.0f);
	return true;
}

bool GameManager::handleMultiPlayer(const Ogre::FrameEvent& evt)
{
	if (cameraFollow)
	{
		if (!isGuest)
		{
			mCameraMan->setYawPitchDist((Ogre::Radian) 0.0f, (Ogre::Radian) 0.6f, (Ogre::Real) 1500.0f);
		}
		else
		{
			mCameraMan->setYawPitchDist((Ogre::Radian) 3.14f, (Ogre::Radian) 0.6f, (Ogre::Real) 1500.0f);
		}
	}


	scoreBox->setText(SSTR(marbleBall->score));
	liveBox->setText( SSTR(marbleBall->lives));
	timeBox->setText( SSTR(timer.getIntTime()));



	if (isGuest)
	{
		playerBox->setText("GUEST");
	}
	else
	{
		playerBox->setText("HOST");
	}
	if (!isGuest)
	{
		if (marbleBall->lives <= 0)
		{
			gameOver = true;
			// infoBox->setText("Game Over. Press esc to quit");
			marbleBall->lives = 0;
			playEnabled = false;
			gameOverBox->show();
			// livesText->setText(SSTR("GAME OVER :("));
		}
	}

	if (gameOver)
	{
		// infoBox->setText("Game Over. Press esc to quit");
		gameOverBox->show();
		// mWindow->setActive(false);
	}

	//step simulation
	if (!isGuest)
		simulator->stepSimulation(evt.timeSinceLastFrame, 10, 1 / 60.0f);
	return true;
}

bool GameManager::handlePause(const Ogre::FrameEvent& evt)
{

}

bool GameManager::hostGame(const CEGUI::EventArgs &e)
{
	// NetMan->stopServer();
	// NetMan->initNetManager();
	// std::string ip = NetMan->getIPstring();
	// PRINT(ip);
	// NetMan->addNetworkInfo(PROTOCOL_ALL, ip.c_str());
	// NetMan->startServer();
	// NetMan->multiPlayerInit();

	host->setText("Hosting");
	host->disable();
	join->disable();
	PRINT("IN HOST GAME");
	currentState = MULTI;
	this->isGuest = false;
	return setupMultiplayer();
}

bool GameManager::joinGame(const CEGUI::EventArgs &e)
{

	// 	std::string ip = "128.83.130.38";
	// 	PRINT(ip);
	// 	PRINT(ip.substr(13));

	// 	//we need +
	// 	NetMan->joinMultiPlayer(ip);
	// 	PRINT("joinMultiPlayer method finished");
	// 	NetMan->acceptConnections();
	// 	PRINT("acceptConnections method finished");
	// 	std::cout << "An exception occured :: " << e << std::endl;

	join->setText("Joining");
	join->disable();
	host->disable();
	currentState = MULTI;
	this->isGuest = true;
	mCameraMan->getCamera()->setPosition(guestPos);
	mCameraMan->getCamera()->lookAt(Ogre::Vector3(0, 0, 0));
	return setupMultiplayer();
}

void GameManager::createScene(void) {
	//Initialize everything related to Ogre and else that we need on start of application
	cout << "Creating Scene..." << endl;


	//create bullet simulator
	simulator = new Simulator();

	//Reposition Camera
	mCamera->setPosition(Ogre::Vector3(CAMERA_DEFAULT_X, CAMERA_DEFAULT_Y, CAMERA_DEFAULT_Z));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));


	// Set the scene's ambient light
	marbleBall = new Ball("Ball", Ogre::SceneManager::PT_SPHERE, mSceneMgr, Ogre::Vector3(0, 300, 0), simulator);
	marbleBall->addToSimulator();
	marbleBall->checkCollisions(true);

	//init player paddle
	playerPaddle = new Paddle("playerPaddle", Ogre::SceneManager::PT_SPHERE, mSceneMgr, Ogre::Vector3(0, 0, ROOM_LENGTH / 2.0f - 45.0f), simulator);
	playerPaddle->addToSimulator();
	playerPaddle->checkCollisions(true);


	//Create a parent node for paddle for camera purposes
	// Ogre::SceneNode* parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("paddleParent");

	//init goal
	theGoal = new Goal("Goal", Ogre::SceneManager::PT_CUBE, mSceneMgr, Ogre::Vector3(0, 0, -ROOM_LENGTH / 2.0f + 45.0f), simulator, Ogre::Vector3(1, 5, 0));
	theGoal->addToSimulator();
	theGoal->checkCollisions(true);

	//init room
	room = new Room(mSceneMgr, simulator);

	// Create a Light and set its position
	Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setDiffuseColour(1.2f, 1.26f, 1.29f);
	pointLight->setSpecularColour(1.2f, 1.26f, 1.29f);
	pointLight->setPosition(Ogre::Vector3(0, 500, 0));

	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);


	//---------------------------------------------------------------
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(quit);
	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameManager::quit, this));

	host = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/HostButton");
	host->setText("Host Multiplayer");
	host->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.06f, 0)));
	host->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(host);
	host->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameManager::hostGame, this));

	join = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/ClientButton");
	join->setText("Join Multiplayer");
	join->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.12f, 0)));
	join->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(join);
	join->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameManager::joinGame, this));

	IPBox = wmgr.createWindow("TaharezLook/Editbox", "CEGUIDemo/IPBox");
	IPBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.18f, 0)));
	IPBox->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(IPBox);

	scoreBox = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/ScoreBox");
	scoreBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.0f, 0)));
	scoreBox->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(scoreBox);


	liveBox = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/LiveBox");
	liveBox->setPosition(CEGUI::UVector2(CEGUI::UDim(.8f, 0), CEGUI::UDim(0.0f, 0)));
	liveBox->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(liveBox);

	timeBox = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/TimeBox");
	timeBox->setPosition(CEGUI::UVector2(CEGUI::UDim(.8f, 0), CEGUI::UDim(0.9f, 0)));
	timeBox->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(timeBox);

	infoBox = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/InfoBox");
	infoBox->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f, 0), CEGUI::UDim(0.8f, 0)));
	infoBox->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.1, 0)));
	infoBox->setText(NetMan->getIPstring());
	sheet->addChild(infoBox);

	playerBox = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/PlayerBox");
	playerBox->setPosition(CEGUI::UVector2(CEGUI::UDim(.8f, 0), CEGUI::UDim(0.78f, 0)));
	playerBox->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.05, 0)));
	playerBox->setText("Player");
	sheet->addChild(playerBox);

	playerBox = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/PlayerBox");
	playerBox->setPosition(CEGUI::UVector2(CEGUI::UDim(.8f, 0), CEGUI::UDim(0.78f, 0)));
	playerBox->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.05, 0)));
	playerBox->setText("Player");
	sheet->addChild(playerBox);

	gameOverBox = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/gameOverBox");
	gameOverBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
	gameOverBox->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0), CEGUI::UDim(1.0, 0)));

	gameOverBox->setText("GAME OVER! Press 'esc' to quit.");
	sheet->addChild(gameOverBox);
	gameOverBox->hide();


	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

//=------------------------------------------------------


	createCameras();
	timer.start();

	cout << "Done." << endl;
}

void GameManager::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
	//TODO make this code more modular by making the degree values constant.
	Ogre::SceneNode* paddle = mSceneMgr->getSceneNode("playerPaddle");
	static Ogre::Real rotate = .13;

	Ogre::Radian pitch = paddle->getOrientation().getPitch();

	if (mKeyboard->isKeyDown(OIS::KC_A))
	{
		paddle->yaw(Ogre::Degree(5 * rotate));
	}
	if (mKeyboard->isKeyDown(OIS::KC_D))
	{
		paddle->yaw(Ogre::Degree(-5 * rotate));
	}
	if (mKeyboard->isKeyDown(OIS::KC_W))
	{
		// Ogre::Degree MAX_UP_PITCH = Ogre::Degree(90);


		cout << pitch << endl;
		cout << "The W value: " << Ogre::Radian(Ogre::Degree(45)) << endl;

		if (Ogre::Math::Abs(pitch) > Ogre::Radian(Ogre::Degree(45))) {
			return;
		}
		paddle->pitch(Ogre::Degree(-5 * rotate));
	}
	if (mKeyboard->isKeyDown(OIS::KC_S))
	{

		cout << pitch << endl;
		cout << "The S value: " << Ogre::Radian(Ogre::Degree(45)) << endl;

		if (Ogre::Math::Abs(pitch) > Ogre::Radian(Ogre::Degree(45))) {
			return;
		}

		paddle->pitch(Ogre::Degree(5 * rotate));
	}
}

//-----------gui changes---------------------------------------------------

bool GameManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

	if (mWindow->isClosed())
		return false;

	if (mShutDown)
		return false;


	mKeyboard->capture();
	mMouse->capture();
	// mInputContext.capture();


	mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	switch (currentState)
	{
	case SINGLE:
	{
		// PRINT("IN SINGLE");
		return handleSinglePlayer(evt);
	}
	break;
	case MULTI:
	{
		// PRINT("IN MULTI");
		if (!isGuest)
			return handleServer(evt);
		else
			return handleClient(evt);
	}
	break;
	case PAUSE:
	{
		// PRINT("IN PAUSE");
		return handlePause(evt);
	}
	break;
	default:
		return false;
	}

}
bool GameManager::keyPressed( const OIS::KeyEvent &arg )
{
	if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
		return true;
	}
	else if (arg.key == OIS::KC_Q)  // kick ball
	{
		marbleBall->getBody()->activate(true);
		marbleBall->getBody()->applyCentralImpulse(btVector3(0, 15000, 150000));
	}
	else if (arg.key == OIS::KC_G)
	{
		if (guiMode) {
			guiMode = false;
		}
		else if (!gameOver)
		{
			playEnabled = false;
			guiMode = true;
		}
	}
	else if (arg.key == OIS::KC_E)  // enable player controls
	{
		if (!gameOver)
			playEnabled = !playEnabled;
	}
	else if (arg.key == OIS::KC_C) // toggle camera modes
	{
		if (guiMode)
			guiMode = false;
		cameraFollow = !cameraFollow;
		if (cameraFollow)
		{
			if (!gameOver) playEnabled = true;
			mCameraMan->setStyle(OgreBites::CS_ORBIT);

			if (isGuest) { mCameraMan->setTarget(mSceneMgr->getSceneNode("Goal"));}
			else { mCameraMan->setTarget(mSceneMgr->getSceneNode("playerPaddle"));}
		}
		else
		{
			mCameraMan->setStyle(OgreBites::CS_FREELOOK);
			if (isGuest) { mCameraMan->getCamera()->setPosition(guestPos);}
			else {mCameraMan->getCamera()->setPosition(hostPos);}

			mCameraMan->getCamera()->lookAt(Ogre::Vector3(0, 0, 0));

		}
	}
	else if (arg.key == OIS::KC_I)
	{
		//TODO Maybe lock it to this position
		mCamera->setPosition(Ogre::Vector3(CAMERA_DEFAULT_X, CAMERA_DEFAULT_Y, CAMERA_DEFAULT_Z));
		mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}
	else if (arg.key == OIS::KC_M)
	{
		mixer->toggleMute();
	}
	else if (arg.key == OIS::KC_B)
	{
		mixer->toggleBackground();
	}
	else if (arg.key == OIS::KC_PERIOD)
	{
		mixer->backgroundUp();
	}
	else if (arg.key == OIS::KC_COMMA)
	{
		mixer->backgroundDown();
	}
	else if (arg.key == OIS::KC_V)
	{
		mWindow->setVisible(!mWindow->isVisible());
	}
	else if (arg.key == OIS::KC_H)
	{
		mWindow->setActive(!mWindow->isActive());
	}

	mCameraMan->injectKeyDown(arg);
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)arg.key);
	context.injectChar((CEGUI::Key::Scan)arg.text);
	return true;
}


bool GameManager::keyReleased( const OIS::KeyEvent &arg )
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
	mCameraMan->injectKeyUp(arg);

	return true;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}

bool GameManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	mCameraMan->injectMouseDown(arg, id);
	return true;
}

bool GameManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	mCameraMan->injectMouseUp(arg, id);
	return true;
}

bool GameManager::mouseMoved( const OIS::MouseEvent &arg )
{

	if (playEnabled)
	{
		if (!isGuest)
		{
			if (arg.state.X.rel > MOUSE_DZ || arg.state.X.rel < -MOUSE_DZ)
			{
				playerPaddle->moveBy(Ogre::Vector3(MOUSE_MOVE_SPEED * arg.state.X.rel, 0, 0));
			}
			if (arg.state.Y.rel > MOUSE_DZ || arg.state.Y.rel < -MOUSE_DZ)
			{
				playerPaddle->moveBy(Ogre::Vector3(0, MOUSE_MOVE_SPEED * -arg.state.Y.rel, 0));
			}
			return true;
		}
		else
		{
			if (arg.state.X.rel > MOUSE_DZ || arg.state.X.rel < -MOUSE_DZ)
			{
				theGoal->moveBy(Ogre::Vector3(MOUSE_MOVE_SPEED * arg.state.X.rel, 0, 0));
			}
			if (arg.state.Y.rel > MOUSE_DZ || arg.state.Y.rel < -MOUSE_DZ)
			{
				theGoal->moveBy(Ogre::Vector3(0, MOUSE_MOVE_SPEED * -arg.state.Y.rel, 0));
			}
			return true;
		}
	}

	if (!guiMode)
		mCameraMan->injectMouseMove(arg);

	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);

	return true;
}

bool GameManager::quit(const CEGUI::EventArgs &e)
{
	mShutDown = true;
	return true;
}



//---------------------------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
	// Create application object
	GameManager app;

	try {
		app.go();
	} catch ( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " <<
		          e.getFullDescription().c_str() << std::endl;
#endif
	}

	return 0;
}

#ifdef __cplusplus
}
#endif

