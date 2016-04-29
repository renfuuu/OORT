#pragma once

#include "GameObject.h"
class Asteroid : public GameObject {

public:
	Asteroid(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Asteroid();

	bool hitWall;
	Ogre::Real velocity;
	
	bool alive;

	virtual void update();
	void move();
};