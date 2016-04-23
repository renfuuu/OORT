#include "Spaceship.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

#define MIN_VELOCITY 0.1
#define MAX_VELOCITY 35.0

Spaceship::Spaceship(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	kinematic = true;
	auto var = ent->getBoundingBox();

	// Bullet uses half margins for collider
	auto size = var.getSize()/2;

	shape = new btBoxShape(btVector3(size.x*scale, size.y*scale, size.z*scale));

	// Below is to turn on particles. Need to change the default particle type in GameObject.cpp
	// Ogre::SceneNode* particleNode = rootNode->createChildSceneNode("Particle");
	// particleNode->attachObject(particle);

	velocity = 1.0f;
	acceleration = 0.0f;

	pitchAngle = Ogre::Degree(0);
	yawAngle = Ogre::Degree(0);
	rollAngle = Ogre::Degree(0);

}

Spaceship::~Spaceship() {
}

void Spaceship::update() {

	static int MAX_DT = 5;

	if (context->hit) {
		Ogre::Real dt = gameManager->getTime() - lastHitTime;
		if ( dt > MAX_DT )
			lastHitTime = gameManager->getTime();
	}
}

void Spaceship::moveSpaceship(OISManager* _oisManager, int height, int width, Ogre::SceneNode* camNode) {

	Ogre::SceneNode* mNode = rootNode;

	OIS::Keyboard* kb = _oisManager->getKeyboard();


	if(kb && kb->isKeyDown(OIS::KC_SPACE))
	{	
		if(velocity >= MIN_VELOCITY && velocity <= MAX_VELOCITY){
			Ogre::Vector3 look = mNode->getOrientation().zAxis();
			velocity = velocity + velocity*acceleration;
			mNode->translate(velocity*look);
		} 
		else if(velocity > MAX_VELOCITY) velocity = MAX_VELOCITY;
		else velocity = MIN_VELOCITY;
	}
	
	if(kb && kb->isKeyDown(OIS::KC_W))
	{
		pitchAngle += Ogre::Degree(0.5);
		mNode->pitch(Ogre::Degree(0.5));
		camNode->pitch(Ogre::Degree(0.5));
	}
	if (kb && kb->isKeyDown(OIS::KC_S))
	{
		pitchAngle += Ogre::Degree(-0.5);
		mNode->pitch(Ogre::Degree(-0.5));
		camNode->pitch(Ogre::Degree(-0.5));
	}
	if (kb && kb->isKeyDown(OIS::KC_A))
	{
		yawAngle += Ogre::Degree(-0.5);
		mNode->yaw(Ogre::Degree(-0.5));
		camNode->yaw(Ogre::Degree(-0.5));
	}
	if (kb && kb->isKeyDown(OIS::KC_D))
	{
		yawAngle += Ogre::Degree(0.5);
		mNode->yaw(Ogre::Degree(0.5));
		camNode->yaw(Ogre::Degree(0.5));
	}
	if (kb && kb->isKeyDown(OIS::KC_Q))
	{
		rollAngle += Ogre::Degree(-0.5);
		mNode->roll(Ogre::Degree(-0.5));
		camNode->roll(Ogre::Degree(-0.5));
	}
	if (kb && kb->isKeyDown(OIS::KC_E))
	{
		rollAngle += Ogre::Degree(0.5);
		mNode->roll(Ogre::Degree(0.5));
		camNode->roll(Ogre::Degree(0.5));
	}
	else if (kb && kb->isKeyDown(OIS::KC_UP))
		acceleration += .0001;
	else if (kb && kb->isKeyDown(OIS::KC_DOWN))
		acceleration -= .0001; 
	
	updateTransform();
}