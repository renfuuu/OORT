#include "Laser.h"
#include "MultiPlatformHelper.h"

Laser::Laser(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	shape = new btSphereShape((ent->getBoundingBox().getHalfSize().x)*scale);
}

Laser::~Laser() {
}

void Laser::updateTransform() {
	// Ogre::Vector3 pos = rootNode->getPosition();
	// tr.setOrigin(btVector3(pos.x, pos.y, pos.z));

	// Ogre::Quaternion qt = rootNode->getOrientation();
	// tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));

	motionState->updateTransform(tr);
}

void Laser::update() {

	if (context->hit) {

		// startScore();
		// if(previousHit != nullptr) {
		// 	// Check for paddle collision but not twice in a row
		// 	if( context->getTheObject()->getType() == GameObject::PADDLE_OBJECT && context->getTheObject() != previousHit ) {
		// 		gameManager->playSound(GameManager::PADDLE_BOUNCE);
		// 	}		

		// 	if (gameManager->isServer() ) {
		// 		//Score only when you hit behind your opponent.
		// 		if ( context->getTheObject()->getType() == GameObject::BACK_WALL_OBJECT && previousHit->getType() != GameObject::BACK_WALL_OBJECT) {
		// 			gameManager->scorePoints(1);
		// 			gameManager->playSound(GameManager::SCORE);
		// 			reset(startPos);
		// 		}
		// 		else if ( context->getTheObject()->getType() == GameObject::FRONT_WALL_OBJECT && previousHit->getType() != GameObject::FRONT_WALL_OBJECT) {
		// 			Ogre::Vector3 vec(-startPos.x, startPos.y, -startPos.z);
		// 			gameManager->scoreOpponentPoints(1);
		// 			reset(vec);
		// 		}
		// 	}
		// }
		
		// previousHit = context->getTheObject();
	}
}