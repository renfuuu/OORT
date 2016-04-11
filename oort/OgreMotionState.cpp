#ifndef OGREMOTIONSTATE_H
#define OGREMOTIONSTATE_H

#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
#include <iostream>
#include "OgreMotionState.h"



OgreMotionState::OgreMotionState(const btTransform& initialpos, Ogre::SceneNode* node) {
	mVisibleobj = node;
	mPosl = initialpos;
}


OgreMotionState::~OgreMotionState() {}

void OgreMotionState::setNode(Ogre::SceneNode* node) {
	mVisibleobj = node;
}
void OgreMotionState::updateTransform(btTransform& newpos) {
	mPosl = newpos;
	setWorldTransform(mPosl);
}

void OgreMotionState::getWorldTransform(btTransform& worldTrans) const {
	worldTrans = mPosl;
}
void OgreMotionState::setWorldTransform(const btTransform& worldTrans) {
	//std::cout << "set world" << std::endl;
	if (NULL == mVisibleobj)
		return; // silently return before we set a node
	btQuaternion rot = worldTrans.getRotation();
	mVisibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	btVector3 pos = worldTrans.getOrigin();
	mVisibleobj->setPosition(pos.x(), pos.y(), pos.z());
}


#endif