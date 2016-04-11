#pragma once

#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
#include <iostream>

using std::cout;
using std::endl;


#include "GameObject.h"

//class GameObject;

class Simulator {
    public:
    	Simulator();
    	~Simulator();
        void stepSimulation(const Ogre::Real, int, const Ogre::Real);
        bool checkHit(int);
        btDiscreteDynamicsWorld * getWorld();
        void addObject(GameObject* o);
 	bool removeObject(GameObject* o); 

    private:
    	//bullet
	btBroadphaseInterface* broadphase;
	btCollisionDispatcher* dispatcher;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;


 	btConstraintSolver* mConstraintsolver;
 	Ogre::SceneManager* sceneMgr;
 	std::deque<GameObject*> objList; 
 	std::deque<int> idList; 
};
