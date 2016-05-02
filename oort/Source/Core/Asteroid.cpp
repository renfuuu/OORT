#include "Asteroid.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

#define MIN_VELOCITY 0.1
#define MAX_VELOCITY 35.0

//TODO add max acceleration
//TODO add min acceleration

Asteroid::Asteroid(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	kinematic = true;

	shape = new btSphereShape((ent->getBoundingRadius()*1.2)*scale);
	// shape = new btSphereShape((ent->getBoundingBox().getSize().x)*scale);

	// auto var = ent->getBoundingBox();

	// // Bullet uses half margins for collider
	// auto size = var.getSize()*.9;

	// shape = new btBoxShape(btVector3(size.x*scale, size.y*scale, size.z*scale));

	// Below is to turn on particles. Need to change the default particle type in GameObject.cpp
	// Ogre::SceneNode* particleNode = rootNode->createChildSceneNode("Particle");
	// particleNode->attachObject(particle);

	velocity = 1.0f;

	hitWall = false;
	alive = true;

}

Asteroid::~Asteroid() {
}




void Asteroid::update() {
	static int MAX_DT = 5;

	if (context->hit) {
		Ogre::Real dt = gameManager->getTime() - lastHitTime;
		if ( dt > MAX_DT )
			lastHitTime = gameManager->getTime();


		// startScore();
		if(previousHit != nullptr) {
			// Check for wall collision but not twice in a row
			if( context->getTheObject()->getType() == GameObject::UP_DOWN_WALL_OBJECT || context->getTheObject()->getType() == GameObject::SIDE_WALL_OBJECT /*&& context->getTheObject() != previousHit */) {
				// gameManager->playSound(GameManager::PADDLE_BOUNCE);
				// hitWall = true;
			}
			if( context->getTheObject()->getType() == GameObject::LASER_OBJECT && context->getTheObject() != previousHit ) {
				alive = false;
				std::cout << context->getTheObject()->getName() << " hit " << name << std::endl;
				context->getTheObject()->setStatus(false);
				simulator->removeObject(context->getTheObject());
				simulator->removeObject(this);
			}
			if( context->getTheObject()->getType() == GameObject::SPACESHIP_OBJECT && context->getTheObject() != previousHit ) {
				alive = false;
				context->getTheObject()->setStatus(false);
				simulator->removeObject(this);
				simulator->removeObject(context->getTheObject());
				std::cout << "Spaceship hit: " << name << std::endl;
			}

		}
		
		previousHit = context->getTheObject();
	}
}

void Asteroid::moveAsteroid(Ogre::SceneNode* ssNode) {

	Ogre::SceneNode* mNode = rootNode;

	if(hitWall){

		// mNode->yaw(Ogre::Degree(0.5));
		// yawAngle += Ogre::Degree(0.5);

		// Ogre::Vector3 look = mNode->getOrientation().zAxis();
		// Ogre::Vector3 wallNormal = dynamic_cast<Wall*>(context->getTheObject())->normal;
		// Ogre::Real dotProduct = look.dotProduct(wallNormal);
		
		// //if the look dotted with the wall's normal is negative then keep yawing
		// if(dotProduct > 0){
		// 	hitWall = false;
		// 	mNode->translate(velocity*look);
		// }
	}else{
		

		// Ogre::Vector3 look = mNode->getOrientation().zAxis();
		Ogre::Vector3 look = ssNode->getPosition() - mNode->getPosition();
		look.normalise();
		mNode->translate(velocity*look);
	}


	updateTransform();
}

