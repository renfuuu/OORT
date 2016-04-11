#include "Simulator.h"


Simulator::Simulator() {
	//broadpahse
    broadphase = new btDbvtBroadphase();

	//setting default configuration
    collisionConfiguration = new btDefaultCollisionConfiguration();

	//setting collision dispatcher
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//solver. 
    solver = new btSequentialImpulseConstraintSolver;

	//init dynamics world. Add up all bullet stuffs
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	//set earth gravity. 
    dynamicsWorld->setGravity(btVector3(0, -100, 0));
}

Simulator::~Simulator() {
	//releasing...
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

btDiscreteDynamicsWorld * Simulator::getWorld() {
    return dynamicsWorld;
}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
	//this is the basic method we will use to simulate the bullet. 
	dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);

	for (int i = 0; i != objList.size(); i++) 
		idList[i] = 0;
    for (unsigned int i = 0; i < objList.size(); i++){
        //if (objList[i]->doUpdates())
        if(checkHit(i))
            // cout << "check hit" << endl;
		objList[i]->update(elapsedTime);
    }
}

bool Simulator::checkHit(int o) {
    for (int i = idList[o]; i < objList.size(); i++) {
        if (i != o) {
            objList[o]->context->hit = false;
			//fixed third parameter
			dynamicsWorld->contactPairTest(objList[o]->getBody(), objList[i]->getBody(), *(objList[o]->cCallBack));
            if (objList[o]->context->hit) {
                //cout << "o = " << objList[o]->getName() << ", posY = " << objList[o]->getY() << endl;
                //cout << "i = " << objList[i]->getName() << ", posY = " << objList[i]->getY() <<  endl;
                idList[o] = ++i;
                return true;
            }
        }
    }
    return false;
}

void Simulator::addObject (GameObject* o) {
    idList.push_back(0);
    objList.push_back(o);
    dynamicsWorld->addRigidBody(o->getBody());
}

bool Simulator::removeObject (GameObject* o) {
    // TODO: Don't be evil.
    //objList.pop_back();
    //idList.pop_back();
    dynamicsWorld->removeRigidBody(o->getBody());
    return true;
}
