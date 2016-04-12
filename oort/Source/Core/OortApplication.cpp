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

		// netManager = new NetManager();
		// setupCEGUI();

		setupCameras();

		setupGM();

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		// setupLighting();

		// createObjects();

	}
	catch (Exception e) {
		std::cout << "Exception Caught: " << e.what() << std::endl;
	}
}

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
	// static int maxSpeed = 4000;
	// btVector3 velocity = _theBall->getBody()->getLinearVelocity();
 //    btScalar speed = velocity.length();
 //    if(speed > maxSpeed) {
 //        velocity *= maxSpeed/speed;
 //        _theBall->getBody()->setLinearVelocity(velocity);
 //    }

	// ballCam->lookAt(_theBall->getNode()->getPosition());

	// cameraMan->frameRenderingQueued(evt);

	return true;
}