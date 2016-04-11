#ifndef PADDLE_H
#define PADDLE_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
#include "Room.h"

class Paddle : public GameObject{
private:

	const static Ogre::Real PADDLE_WIDTH = 364.0f;
	const static Ogre::Real PADDLE_LENGTH = 5.0f;
	const static Ogre::Real PADDLE_HEIGHT = 300.0f;
	const static btScalar 	PADDLE_RESTITUTION = 1.04f;
	const static btScalar 	PADDLE_FRICTION = 0.01f;
	float lastTime;
public:	
	//constructor
	Paddle(
		Ogre::String name, 
		Ogre::SceneManager::PrefabType meshName, 
		Ogre::SceneManager* mSceneMgr, 
		Ogre::Vector3 position, 
		Simulator* pSim);
	//update move
	void move(/*need parameter*/);
	//update paddle
	void update(float);

	void moveBy(Ogre::Vector3 distance);
	void moveTo(Ogre::Vector3 newPos);
};

#endif