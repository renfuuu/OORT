#include "Wall.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Wall::Wall(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::Vector3 n, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	auto var = ent->getBoundingBox();
	auto size = var.getSize();

	normal = n;

	shape = new btBoxShape(btVector3((var.getSize().x*vscale.x)/2, (var.getSize().y*vscale.y)/2, (var.getSize().z*vscale.z)/2));
}

Wall::~Wall(){
}

void Wall::update() {

	static int MAX_DT = 4;

	// if (context->hit) {
	// 	Ogre::Real dt = gameManager->getTime() - lastHitTime;
	// 	if ( dt > MAX_DT && context->getTheObject()->getType() == GameObject::BALL_OBJECT ) {
	// 		lastHitTime = gameManager->getTime();
	// 		gameManager->playSound(GameManager::WALL_BOUNCE);
	// 	}

	// 	if ( type != GameObject::FLOOR_OBJECT ) {
	// 		gameManager->nonFloorHit();
	// 	}
	// }

}