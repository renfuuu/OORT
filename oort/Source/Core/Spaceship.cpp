#include "Spaceship.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

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
		Ogre::Vector3 look = mNode->getOrientation().zAxis();
		velocity = velocity + velocity*acceleration;
		mNode->translate(velocity*look);
	}
	if(kb && kb->isKeyDown(OIS::KC_W))
	{
		mNode->pitch(Ogre::Degree(0.10));
		camNode->pitch(Ogre::Degree(0.10));
	}
	if (kb && kb->isKeyDown(OIS::KC_S))
	{
		mNode->pitch(Ogre::Degree(-0.10));
		camNode->pitch(Ogre::Degree(-0.10));
	}
	if (kb && kb->isKeyDown(OIS::KC_A))
	{
		mNode->yaw(Ogre::Degree(-0.10));
		camNode->yaw(Ogre::Degree(-0.10));
	}
	if (kb && kb->isKeyDown(OIS::KC_D))
	{
		mNode->yaw(Ogre::Degree(0.10));
		camNode->yaw(Ogre::Degree(0.10));
	}
	if (kb && kb->isKeyDown(OIS::KC_Q))
	{
		mNode->roll(Ogre::Degree(-0.10));
		camNode->roll(Ogre::Degree(-0.10));
	}
	if (kb && kb->isKeyDown(OIS::KC_E))
	{
		mNode->roll(Ogre::Degree(0.10));
		camNode->roll(Ogre::Degree(0.10));
	}
	else if (kb && kb->isKeyDown(OIS::KC_UP))
		acceleration += .1;
	else if (kb && kb->isKeyDown(OIS::KC_DOWN))
		acceleration -= .1; 
	
	updateTransform();
}