#pragma once

#include "GameObject.h" 

class Wall : public GameObject {

public:
	Ogre::Vector3 normal;

	Wall(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, Ogre::Vector3 n, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin);
	~Wall();
	virtual void update();
};