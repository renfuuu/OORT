#include "Paddle.h"

Paddle::Paddle(Ogre::String name, Ogre::SceneManager::PrefabType meshName, Ogre::SceneManager* mSceneMgr, Ogre::Vector3 position, Simulator* pSim) :
	GameObject(name, mSceneMgr, PADDLE_RESTITUTION, PADDLE_FRICTION, pSim)
{
	geom = sceneMgr->createEntity(name, meshName);
	geom->setMaterialName("Examples/RustySteel");
	rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	rootNode->attachObject(geom);
	rootNode->setPosition(position);
    rootNode->setScale(Ogre::Vector3(PADDLE_WIDTH/100.0f, PADDLE_HEIGHT/100.0f, PADDLE_LENGTH/100.0f));

	shape = new btSphereShape(0.5f);
    shape->setLocalScaling(btVector3(PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_LENGTH));
	this->mass = 0;
    tr.setOrigin(btVector3(position.x, position.y, position.z));
    kinematic = true;
}

void Paddle::update(float elapsedTime){
	//Sounds stuff do not worry yet
}

void Paddle::moveBy(Ogre::Vector3 distance) {
    Ogre::Vector3 curPos = rootNode->getPosition();
    float negBound = -ROOM_WIDTH/2.0f + PADDLE_WIDTH/2.0f;
    float posBound = ROOM_WIDTH/2.0f - PADDLE_WIDTH/2.0f;
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

void Paddle::moveTo(Ogre::Vector3 newPos) {
    Ogre::Vector3 curPos = rootNode->getPosition();
    float negBound = -ROOM_WIDTH/2.0f + PADDLE_WIDTH/2.0f;
    float posBound = ROOM_WIDTH/2.0f - PADDLE_WIDTH/2.0f;

    if (newPos.x >= posBound || newPos.x <= negBound) {
        return;
    }
    if (newPos.y >= posBound || newPos.y <= negBound) {
        return;
    }

    rootNode->setPosition(newPos);
    updateTransform();
}

//TODO rotateBy method