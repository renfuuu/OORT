#pragma once

#include <Ogre.h>
#include "BulletContactCallback.h"
#include "Simulator.h"
#include "OgreMotionState.h"
#include <iostream>

using std::cout;
using std::endl;

class OgreMotionState;
class Simulator;
 
class GameObject {
public:
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;
    OgreMotionState* motionState;

    Simulator* simulator;
    btCollisionShape* shape;
    btRigidBody* body;
    btTransform tr;
    btVector3 inertia;

    btScalar mass;
    btScalar restitution;
    btScalar friction;

    bool kinematic;
    bool needsUpdates;

public:
	GameObject(Ogre::String name, Ogre::SceneManager* sceneMgr, Ogre::Real restitution, Ogre::Real friction, Simulator* pSim);
    
    CollisionContext* context;

    BulletContactCallback* cCallBack;

    void checkCollisions(bool);
    void addToSimulator();
    btRigidBody * getBody();
    virtual void update(float) = 0;	//overrides from ball

    virtual void updateTransform();

    Ogre::String getName(){
        return this->name;
    }

    float getY(){
        return rootNode->getPosition().y;
    }
};

