#include "GameObject.h"
#include "Simulator.h"

GameObject::GameObject(Ogre::String name, Ogre::SceneManager* sceneMgr, Ogre::Real restitution, Ogre::Real friction, Simulator* pSim):
	name(name),
	sceneMgr(sceneMgr),
    cCallBack(0),
    kinematic(false),
    restitution(restitution),
    friction(friction),
    simulator(pSim)
{
	tr.setIdentity();
}

void GameObject::checkCollisions(bool check) {
    needsUpdates = check;
    if (!cCallBack && needsUpdates) {
        context = new CollisionContext();
        cCallBack = new BulletContactCallback(*body, *context);
    }
}

void GameObject::updateTransform() {
    Ogre::Vector3 curPos = rootNode->getPosition();
    tr.setOrigin(btVector3(curPos.x, curPos.y, curPos.z));

    Ogre::Quaternion curQt = rootNode->getOrientation();
    tr.setRotation(btQuaternion(curQt.x, curQt.y, curQt.z, curQt.w));
    motionState->updateTransform(tr);
}

void GameObject::addToSimulator() {
	shape->calculateLocalInertia(mass, inertia);
    motionState = new OgreMotionState(tr, rootNode);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    rbInfo.m_restitution = restitution;
    rbInfo.m_friction = friction;
    body = new btRigidBody(rbInfo);
    body->setUserPointer(this);

    if (kinematic) {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);
    }
    simulator->addObject(this);
}

btRigidBody * GameObject::getBody() {
    return body;
}

