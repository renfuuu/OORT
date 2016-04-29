#include "Laser.h"
#include "MultiPlatformHelper.h"

#define SPEED 30.0f

Laser::Laser(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	// shape = new btSphereShape((ent->getBoundingBox().getHalfSize().x)*scale);

	auto var = ent->getBoundingBox();

	// Bullet uses half margins for collider
	auto size = var.getSize();

	shape = new btBoxShape(btVector3(size.x * scale, size.y * scale, size.z * scale));
	// Below is to turn on particles. Need to change the default particle type in GameObject.cpp
	Ogre::SceneNode* particleNode = rootNode->createChildSceneNode("Particle_"+nme);
	particleNode->attachObject(particle);

	alive = true;

}

Laser::~Laser() {
}

void Laser::updateTransform() {
	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));

	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));

	motionState->updateTransform(tr);
}

void Laser::moveLaser()
{
	Ogre::SceneNode* mNode = rootNode;
	mNode->translate(SPEED * velocity);

	updateTransform();
}

void Laser::update() {

	static int MAX_DT = 5;

	if (context->hit && context->getTheObject()->getType()) {
		Ogre::Real dt = gameManager->getTime() - lastHitTime;
		if ( dt > MAX_DT )
			lastHitTime = gameManager->getTime();


		// startScore();
		if(previousHit != nullptr) {
			// Check for wall collision but not twice in a row
			if( context->getTheObject()->getType() == GameObject::UP_DOWN_WALL_OBJECT || context->getTheObject()->getType() == GameObject::SIDE_WALL_OBJECT && context->getTheObject() != previousHit ) {
				// gameManager->playSound(GameManager::PADDLE_BOUNCE);
				std::cout << "Wall hit" << std::endl;
				alive = false;
			}
			if( context->getTheObject()->getType() == GameObject::ASTEROID_OBJECT && context->getTheObject() != previousHit ) {
				std::cout << context->getTheObject()->getName() <<" Hit!" << std::endl;
				alive = false;
			}

		}
		
		previousHit = context->getTheObject();
	}
}