#include "Spaceship.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

#define MIN_VELOCITY 0.1
#define MAX_VELOCITY 25.0

//TODO add max acceleration
//TODO add min acceleration

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
	// acceleration = 0.0f;

	pitchAngle = Ogre::Degree(0);
	yawAngle = Ogre::Degree(0);
	rollAngle = Ogre::Degree(0);

	hitWall = false;
	alive = true;

}

Spaceship::~Spaceship() {
}

void Spaceship::update() {
	static int MAX_DT = 5;

	if (context->hit) {
		Ogre::Real dt = gameManager->getTime() - lastHitTime;
		if ( dt > MAX_DT )
			lastHitTime = gameManager->getTime();


		// startScore();
		// if(previousHit != nullptr) {
			// Check for wall collision but not twice in a row
			if( context->getTheObject()->getType() == GameObject::UP_DOWN_WALL_OBJECT || context->getTheObject()->getType() == GameObject::SIDE_WALL_OBJECT /*&& context->getTheObject() != previousHit */) {
				// gameManager->playSound(GameManager::PADDLE_BOUNCE);
				hitWall = true;
			}

		// }
		
		previousHit = context->getTheObject();
	}
}

void Spaceship::moveSpaceship(OISManager* _oisManager, int height, int width, Ogre::SceneNode* camNode) {

	Ogre::SceneNode* mNode = rootNode;

	if(hitWall && context->getTheObject()->getType() != GameObject::LASER_OBJECT){

		mNode->yaw(Ogre::Degree(0.5));
		yawAngle += Ogre::Degree(0.5);

		Ogre::Vector3 look = mNode->getOrientation().zAxis();
		Ogre::Vector3 wallNormal = dynamic_cast<Wall*>(context->getTheObject())->normal;
		// std::cout << "Wall Normal: " << wallNormal << "\nLook: " << look << std::endl;

		Ogre::Real dotProduct = look.dotProduct(wallNormal);
		
		//if the look dotted with the wall's normal is negative then keep yawing
		if(dotProduct > 0){
			hitWall = false;
			mNode->translate(velocity*look);
		}
	}else{
		OIS::Keyboard* kb = _oisManager->getKeyboard();
		OIS::Mouse* mouse = _oisManager->getMouse();

		float zVal = mouse->getMouseState().Z.rel;
		float boost;

		// std::cout << zVal << std::endl;
		if(zVal!=0)
		{
			if(zVal>0)
			{
				velocity += 1.0;
			}
			else
			{
				velocity -= 1.0;
			}
		}


		if(velocity >= MIN_VELOCITY && velocity <= MAX_VELOCITY)
		{
			velocity = velocity + boost;
		} 
		else if(velocity > MAX_VELOCITY) 
			velocity = MAX_VELOCITY;
		else 
			velocity = MIN_VELOCITY;
			

		if(kb && kb->isKeyDown(OIS::KC_W))
		{
			pitchAngle += Ogre::Degree(0.5);
			mNode->pitch(Ogre::Degree(0.5));
			// camNode->pitch(Ogre::Degree(0.5));
		}
		if (kb && kb->isKeyDown(OIS::KC_S))
		{
			pitchAngle += Ogre::Degree(-0.5);
			mNode->pitch(Ogre::Degree(-0.5));
			// camNode->pitch(Ogre::Degree(-0.5));
		}
		if (kb && kb->isKeyDown(OIS::KC_A))
		{
			yawAngle += Ogre::Degree(0.5);
			mNode->yaw(Ogre::Degree(0.5));
			// camNode->yaw(Ogre::Degree(-0.5));
		}
		if (kb && kb->isKeyDown(OIS::KC_D))
		{
			yawAngle += Ogre::Degree(-0.5);
			mNode->yaw(Ogre::Degree(-0.5));
			// camNode->yaw(Ogre::Degree(0.5));
		}
		if (kb && kb->isKeyDown(OIS::KC_Q))
		{
			rollAngle += Ogre::Degree(-0.5);
			mNode->roll(Ogre::Degree(-0.5));
			// camNode->roll(Ogre::Degree(-0.5));
		}
		if (kb && kb->isKeyDown(OIS::KC_E))
		{
			rollAngle += Ogre::Degree(0.5);
			mNode->roll(Ogre::Degree(0.5));
			// camNode->roll(Ogre::Degree(0.5));
		}

		Ogre::Vector3 look = mNode->getOrientation().zAxis();
		mNode->translate(velocity*look);
	}


	updateTransform();
}

void Spaceship::setVelocity(int i){
	velocity = i;
}