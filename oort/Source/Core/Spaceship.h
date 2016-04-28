#pragma once

#include "GameObject.h"
#include "Laser.h"
#include <SdkCameraMan.h>
class Spaceship : public GameObject {

	Ogre::Real velocity;
	// Ogre::Real acceleration;

	std::vector<Laser*> lasers;

public:
	Spaceship(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin);
	~Spaceship();

	Ogre::Degree pitchAngle;
	Ogre::Degree yawAngle;
	Ogre::Degree rollAngle;

	bool hitWall;

	OgreBites::SdkCameraMan* getCamera();

	virtual void update();
	virtual void moveSpaceship(OISManager* _oisManager, int height, int width, Ogre::SceneNode* cam);
	void updateChaseCam(Ogre::SceneNode * cam);
};