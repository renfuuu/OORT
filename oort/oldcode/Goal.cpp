#include "Goal.h"

Goal::Goal(Ogre::String name, 
    Ogre::SceneManager::PrefabType meshName, 
    Ogre::SceneManager* mSceneMgr, 
    Ogre::Vector3 position, 
    Simulator* pSim,
    Ogre::Vector3 direction) :
	GameObject(name, mSceneMgr, GOAL_RESTITUTION, GOAL_FRICTION, pSim)
{
	geom = sceneMgr->createEntity(name, meshName);
	geom->setMaterialName("Examples/RustySteel");
	rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	rootNode->attachObject(geom);
	rootNode->setPosition(position);
    rootNode->setScale(Ogre::Vector3(GOAL_WIDTH/100.0f, GOAL_HEIGHT/100.0f, GOAL_LENGTH/100.0f));

    shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)); //TODO I don't quite understand, things seem to work
	// shape = new btSphereShape(0.5f); //TODO this is currently hardcoded, how to fix?
    shape->setLocalScaling(btVector3(GOAL_WIDTH, GOAL_HEIGHT, GOAL_LENGTH));
	this->mass = 0;
    tr.setOrigin(btVector3(position.x, position.y, position.z));
    kinematic = true;
    gDirection = direction;
}

void Goal::update(float elapsedTime){
	//Sounds stuff do not worry yet
}

void Goal::move(const Ogre::FrameEvent& evt) {
    Ogre::Vector3 curPos = rootNode->getPosition();

    float negBoundx = -ROOM_WIDTH/2.0f + GOAL_WIDTH/2.0f;
    float posBoundx = ROOM_WIDTH/2.0f - GOAL_WIDTH/2.0f;
    float negBoundy = -ROOM_HEIGHT/2.0f + GOAL_HEIGHT/2.0f;
    float posBoundy = ROOM_HEIGHT/2.0f - GOAL_HEIGHT/2.0f;
    // Ogre::Vector3 newPos = curPos + distance;


    //Handling x bound
    if (curPos.x < negBoundx && gDirection.x < 0.0f)  {
        //TODO Make this a random value in the opposite 180 degrees
        gDirection.x = -gDirection.x;
    }
    if (curPos.x > posBoundx && gDirection.x > 0.0f) {
        gDirection.x = -gDirection.x;
    }

    //Handling y bound
    if (curPos.y < negBoundy && gDirection.y < 0.0f) {
        gDirection.y = -gDirection.y;
    }
    if (curPos.y > posBoundy && gDirection.y > 0.0f) {
        gDirection.y = -gDirection.y;
    }


    rootNode->translate(SPEED * evt.timeSinceLastFrame * gDirection, Ogre::Node::TS_LOCAL );
    updateTransform();
}

void Goal::moveBy(Ogre::Vector3 distance) {
    distance = Ogre::Vector3(-1*distance.x, distance.y, distance.z);

    Ogre::Vector3 curPos = rootNode->getPosition();
    float negBound = -ROOM_WIDTH/2.0f + GOAL_WIDTH/2.0f;
    float posBound = ROOM_WIDTH/2.0f - GOAL_WIDTH/2.0f;
    Ogre::Vector3 newPos = curPos + distance;

    if (newPos.x >= posBound || newPos.x <= negBound) {
        return;
    }
    if (newPos.y >= posBound || newPos.y <= negBound) {
        return;
    }

    rootNode->translate(distance);
    updateTransform();
}

void Goal::moveTo(Ogre::Vector3 newPos) {
    Ogre::Vector3 curPos = rootNode->getPosition();
    float negBound = -ROOM_WIDTH/2.0f + GOAL_WIDTH/2.0f;
    float posBound = ROOM_WIDTH/2.0f - GOAL_WIDTH/2.0f;

    if (newPos.x >= posBound || newPos.x <= negBound) {
        return;
    }
    if (newPos.y >= posBound || newPos.y <= negBound) {
        return;
    }

    rootNode->setPosition(newPos);
    updateTransform();
}