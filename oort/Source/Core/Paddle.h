#pragma once

#include "GameObject.h"

class Paddle : public GameObject {
public:
	Paddle(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Paddle();
	virtual void update();
	virtual void movePaddle(OISManager* _oisManager, int height, int width, float realX, float realY);
	float getZ(float x, float y);
	virtual std::string getCoordinates();

};