#include "Surface.h"

Surface::Surface(Ogre::String name, 
    Ogre::SceneManager* mgr, 
    Simulator* sim, 
    Ogre::Vector3 normal,
    Ogre::Real offset, 
    Ogre::Real width, 
    Ogre::Real height,
    btScalar restitution,
    btScalar friction,
    Ogre::String texture = "") :
     GameObject(name, mgr, restitution, friction, sim) 
{
    Ogre::Vector3 ortho = Ogre::Vector3::UNIT_Y;
    if (normal == Ogre::Vector3::UNIT_Y || normal == -Ogre::Vector3::UNIT_Y)
        ortho = Ogre::Vector3::UNIT_Z;

    Ogre::Plane plane(normal, offset);
    Ogre::MeshManager::getSingleton().createPlane(
      name,
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
      plane, 
      width, height, 20, 20, 
      true, 
      1, 5, 5, 
      ortho);
    geom = mgr->createEntity(name);
    rootNode = mgr->getRootSceneNode()->createChildSceneNode(name);
    rootNode->attachObject(geom);
    geom->setCastShadows(false);
    geom->setMaterialName(texture);

    mass = 0;
    Ogre::Vector3 position = normal * offset;
    std::cout << name << ": " << position << "; " << offset << endl;
    shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), 0);
    tr.setOrigin(btVector3(position.x, position.y, position.z));
}

void Surface::update(float elapsedTime){
    lastTime += elapsedTime;
    simulator->getWorld()->contactTest(body, *cCallBack);
    // cout << "v = " << context->velocity.getY() << endl;
    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0) 
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        /*
        if (context->theObject->getName() == "Player") {
            //soundMgr->playClip(punchClip);
        } else {
            //soundMgr->playClip(bounceClip);
        }*/
        lastTime = 0.0f;
    }
    
    context->hit = false;
}