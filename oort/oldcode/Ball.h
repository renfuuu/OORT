#ifndef BALL_H
#define BALL_H

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include <string>
#include <iostream>

#include "GameObject.h"
#include "SoundManager.h"

using std::cout;
using std::endl;

/**
*	Simple Ball class
*	
*/
class Ball : public GameObject {
	public:
	const static Ogre::Real BALL_RADIUS = 50.0f;
	const static btScalar   BALL_RESTITUTION = 0.945;
	const static btScalar   BALL_FRICTION = 0.5;
	const static int BEGIN_LIVES = 10;
	protected:
	float radius;
	float lastTime;
	int bounceSound, lossSound, paddleSound, scoreSound;

	//The speed of ball to each axis (x, y, z)
	Ogre::Vector3 speed;


	public:
	int score;
	int lives;
	Ball(Ogre::String name, Ogre::SceneManager::PrefabType meshName, Ogre::SceneManager* sceneMgr, Ogre::Vector3 position, Simulator* pSim);

	//update Ball
	void update(float);
};

#endif
