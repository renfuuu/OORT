class oortApplication : public Application
{
	Application();
	virtual ~Application();
	virtual void init();

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
	Ogre::Camera * ballCam;
	Ogre::Timer* t1;

	GameManager* gameManager;
	OISManager* _oisManager;
	Simulator* _simulator;
	GameObject* _spaceship;

    CEGUI::OgreRenderer* mRenderer;
	
	std::vector<Ogre::Camera*> cameras;

	int points;
	int width;
	int height;

	double fps = 300.0;

	bool begin = false;
	bool mRunning = true;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
	// void createRootEntity(std::string name, std::string mesh, int x, int y, int z);
	// void createChildEntity(std::string name, std::string mesh, Ogre::SceneNode* sceneNode, int x, int y, int z);
	// bool update(const Ogre::FrameEvent &evt);
	// bool handleGUI(const Ogre::FrameEvent &evt);
	// void handleAi();
	// bool updateServer(const Ogre::FrameEvent &evt);
	// bool updateClient(const Ogre::FrameEvent &evt);

	// Ball* createBall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	// Paddle* createPaddle(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Real scale, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	// Wall* createWall(Ogre::String nme, GameObject::objectType tp, Ogre::String meshName, int x, int y, int z, Ogre::Vector3 scale, Ogre::Degree pitch, Ogre::Degree yaw, Ogre::Degree roll, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, bool kinematic, Simulator* mySim);
	// void startStadiumFlashes(void);
	// void setupWindowRendererSystem(void);
	// void setupResources(void);
	// void setupOIS(void);
	// void setupCEGUI(void);
	// void setupCameras(void);
	// void setupGM(void);
	// void setupLighting(void);
	// void createObjects(void);
	// bool StartServer(const CEGUI::EventArgs &e);
	// bool JoinServer(const CEGUI::EventArgs &e);
	// bool Quit(const CEGUI::EventArgs &e);
	// bool StartSinglePlayer(const CEGUI::EventArgs &e);
	// bool Replay(const CEGUI::EventArgs &e);
	// bool Home(const CEGUI::EventArgs &e);
	// bool HowTo(const CEGUI::EventArgs &e);
};