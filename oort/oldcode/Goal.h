#ifndef GOAL_H
#define GOAL_H


#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
#include "Room.h"

class Goal : public GameObject{
private:


	//Width is x-axis
	//Height is y-axis
	//Length is z-axis
	const static Ogre::Real GOAL_WIDTH = 445.0f;
	const static Ogre::Real GOAL_HEIGHT = 400.0f;
	const static Ogre::Real GOAL_LENGTH = 5.0f;
	const static btScalar 	GOAL_RESTITUTION = 1.0f;
	const static btScalar 	GOAL_FRICTION = 0.5f;
	const static Ogre::Real SPEED = 100.0f;
	float lastTime;
	Ogre::Vector3 gDirection;
public:	
	//constructor
	Goal(
		Ogre::String name, 
		Ogre::SceneManager::PrefabType meshName, 
		Ogre::SceneManager* mSceneMgr, 
		Ogre::Vector3 position, 
		Simulator* pSim,
		Ogre::Vector3 direction);
	//update move
	//update paddle
	void update(float);

	// void moveBy(Ogre::Vector3 distance);

	void move(const Ogre::FrameEvent& evt);
	void moveBy(Ogre::Vector3 distance);
	void moveTo(Ogre::Vector3 newPos);
};

#endif
