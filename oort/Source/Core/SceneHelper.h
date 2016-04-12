#pragma once
#include <OgreSceneManager.h>
#include <string>
#include "Ball.h"
#include "Paddle.h"
#include "Application.h"

namespace SceneHelper {
	inline Ogre::Entity* getEntity(Ogre::SceneManager* mSceneManager, std::string name, int pos) {
		return ((Ogre::Entity*)(mSceneManager->getSceneNode(name)->getAttachedObject(pos)));
	}
};