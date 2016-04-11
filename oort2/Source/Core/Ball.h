#pragma once

#include "GameObject.h"

class Ball : public GameObject {
public:
	
	Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Ball();
	virtual void update();
	virtual void updateTransform();
	virtual std::string getCoordinates();
	virtual std::string getPoints();
};

