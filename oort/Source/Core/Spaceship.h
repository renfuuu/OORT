#pragma once

#include "GameObject.h"

class Spaceship : public GameObject {
public:
	Spaceship(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Spaceship();
	virtual void update();
	virtual void moveSpaceship(OISManager* _oisManager, int height, int width);

};