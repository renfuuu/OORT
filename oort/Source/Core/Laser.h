#pragma once

#include "GameObject.h"

class Laser : public GameObject {

public:
	
	Ogre::Vector3 velocity;
	Ogre::Real acceleration;

	// bool alive;

	Laser(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Vector3 scal, bool kin);
	~Laser();
	virtual void updateTransform();
	void moveLaser();
	virtual void update();

};
