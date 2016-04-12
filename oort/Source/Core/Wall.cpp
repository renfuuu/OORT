#include "Wall.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Wall::Wall(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	auto var = ent->getBoundingBox();
	auto size = var.getSize();

	shape = new btBoxShape(btVector3((var.getSize().x*vscale.x)/2, (var.getSize().y*vscale.y)/2, (var.getSize().z*vscale.z)/2));
}

Wall::~Wall(){
}

void Wall::update() {

	static int MAX_DT = 20;

	if (context->hit) {

		auto* obj = context->getTheObject();

		Ogre::Real dt = gameManager->getTime() - lastHitTime;
		if ( dt > MAX_DT && obj->getType() == GameObject::BALL_OBJECT ) {
			gameManager->playSound(GameManager::WALL_BOUNCE);
		}

		if ( type != GameObject::FLOOR_OBJECT ) {
		}
		// The paddle can drag on the ground, don't let that reset our sound timer
		if (obj->getType() != GameObject::PADDLE_OBJECT)
			lastHitTime = gameManager->getTime();
	}

}