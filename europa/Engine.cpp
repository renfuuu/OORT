#include "Engine.h"
#include <cstdlib>
#include <ctime>

int Ball::instances = 0;

Ball::Ball(int _id = Ball::instances++): id(_id) {}
Ball::~Ball(){}

void Ball::move()
{
  node->translate(vel);
}

Engine::Engine(void)
{
}
 
Engine::~Engine(void)
{
  for (std::vector<Ball*>::iterator i = balls.begin(); i != balls.end(); ++i)
  {
    delete *i;
  }
}
 
void Engine::createCamera()
{
  mCamera = mSceneMgr->createCamera("PlayerCam");
 
  mCamera->setPosition(Ogre::Vector3(0, -10, -500));
  mCamera->lookAt(Ogre::Vector3(0, 0, 0));
  mCamera->setNearClipDistance(5);
 
  mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
 
void Engine::createViewports()
{
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
 
  vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
 
  mCamera->setAspectRatio(
    Ogre::Real(vp->getActualWidth()) /
    Ogre::Real(vp->getActualHeight()));
 
}
 
void Engine::createScene()
{
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
  mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
 
  //create ball
  Ball* ball0 = new Ball;
	ball0->entity = mSceneMgr->createEntity("sphere.mesh");
	ball0->entity->setCastShadows(true);
  ball0->entity->setMaterialName("Examples/BeachStones");


	ball0->node = mSceneMgr->getRootSceneNode()->createChildSceneNode("ball0");
	ball0->node->setScale(.5,.5,.5);
  ball0->node->attachObject(ball0->entity);
  
  float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  

  Ogre::Vector3 vectemp0(1-r1,1-r2,1-r3);
  ball0->vel = 2*vectemp0;

  Ball* ball1 = new Ball;
  ball1->entity = mSceneMgr->createEntity("sphere.mesh");
  ball1->entity->setCastShadows(true);
  ball1->entity->setMaterialName("Examples/BeachStones");


  ball1->node = mSceneMgr->getRootSceneNode()->createChildSceneNode("ball1");
  ball1->node->setScale(.5,.5,.5);
  ball1->node->attachObject(ball1->entity);
  
  r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  

  Ogre::Vector3 vectemp1(1-r1,1-r2,1-r3);
  ball1->vel = 2*vectemp1;

  balls.push_back(ball0);
  balls.push_back(ball1);

//------------------------------------------------------------------------------
	
	int size = 700; // room octant disance
                  // room is a cube around the origin

  Ogre::Vector3 vec0(-size,0,0);
  Ogre::Vector3 vec1(0,-size,0);
  Ogre::Vector3 vec2(0,0,-size);
  Ogre::Vector3 vec3(size,0,0);
  Ogre::Vector3 vec4(0,size,0);
  Ogre::Vector3 vec5(0,0,size);

	int boxSize = size*2;

//--------------------------------------------------------------------------------
  // Create wall0
  Ogre::Plane plane0(Ogre::Vector3::UNIT_X, vec0);
  Ogre::MeshManager::getSingleton().createPlane(
    "wallnegx",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane0, boxSize, boxSize, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
  Ogre::Entity* groundEntity0 = mSceneMgr->createEntity("wallnegx");
  mSceneMgr->getRootSceneNode()->createChildSceneNode("0")->attachObject(groundEntity0);
  groundEntity0->setMaterialName("Examples/Rockwall");
  groundEntity0->setCastShadows(false);
  

  // Create ground1
  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, vec1);
  Ogre::MeshManager::getSingleton().createPlane(
    "ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane1, boxSize, boxSize, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
  Ogre::Entity* groundEntity1 = mSceneMgr->createEntity("ground");
  mSceneMgr->getRootSceneNode()->createChildSceneNode("1")->attachObject(groundEntity1);
  groundEntity1->setMaterialName("Examples/Rockwall");
  groundEntity1->setCastShadows(false);


  // Create wall2
  Ogre::Plane plane2(Ogre::Vector3::UNIT_Z, vec2);
  Ogre::MeshManager::getSingleton().createPlane(
    "wallnegz",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane2, boxSize, boxSize, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
  Ogre::Entity* groundEntity2 = mSceneMgr->createEntity("wallnegz");
  mSceneMgr->getRootSceneNode()->createChildSceneNode("2")->attachObject(groundEntity2);
  groundEntity2->setMaterialName("Examples/Rockwall");
  groundEntity2->setCastShadows(false);

  // Create wall3
  Ogre::Plane plane3(0-Ogre::Vector3::UNIT_X, vec3);
  Ogre::MeshManager::getSingleton().createPlane(
    "wallposx",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane3, boxSize, boxSize, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
  Ogre::Entity* groundEntity3 = mSceneMgr->createEntity("wallposx");
  mSceneMgr->getRootSceneNode()->createChildSceneNode("3")->attachObject(groundEntity3);
  groundEntity3->setMaterialName("Examples/Rockwall");
  groundEntity3->setCastShadows(false);

  // Create wall4
  Ogre::Plane plane4(0-Ogre::Vector3::UNIT_Y, vec4);
  Ogre::MeshManager::getSingleton().createPlane(
    "wallnegy",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane4, boxSize, boxSize, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
  Ogre::Entity* groundEntity4 = mSceneMgr->createEntity("wallnegy");
  mSceneMgr->getRootSceneNode()->createChildSceneNode("4")->attachObject(groundEntity4);
  groundEntity4->setMaterialName("Examples/Rockwall");
  groundEntity4->setCastShadows(false);

  // Create wall4
  Ogre::Plane plane5(0-Ogre::Vector3::UNIT_Z, vec5);
  Ogre::MeshManager::getSingleton().createPlane(
    "wallposz",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane5, boxSize, boxSize, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
  Ogre::Entity* groundEntity5 = mSceneMgr->createEntity("wallposz");
  mSceneMgr->getRootSceneNode()->createChildSceneNode("5")->attachObject(groundEntity5);
  groundEntity5->setMaterialName("Examples/Rockwall");
  groundEntity5->setCastShadows(false);

//-----------------------------------------------------------------------------
 
  // Pointlight
  Ogre::Light* spotLight = mSceneMgr->createLight("Spotlight");
  spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
 
  spotLight->setDiffuseColour(Ogre::ColourValue(0, 0, 1));
  spotLight->setSpecularColour(Ogre::ColourValue(0, 0, 1));
 
  spotLight->setDirection(-1, -1, 0);
  spotLight->setPosition(Ogre::Vector3(200, 200, 0));
 
  spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

  // Directional light
  Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
  directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
 
  directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0, 0));
  directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0, 0));
 
  directionalLight->setDirection(Ogre::Vector3(0, -1, 1));
 
  // Point light
  Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
  pointLight->setType(Ogre::Light::LT_POINT);
 
  pointLight->setDiffuseColour(.3, .3, .3);
  pointLight->setSpecularColour(.3, .3, .3);
 
  pointLight->setPosition(Ogre::Vector3(0, 150, 250));
 
}

bool Engine::frameRenderingQueued(const Ogre::FrameEvent& evt)
{ 
  bool retValue = BaseApplication::frameRenderingQueued(evt);
  
  for (std::vector<Ball*>::iterator i = balls.begin(); i != balls.end(); ++i)
  {
    isCollision(*i);
    (*i)->move();
  }
  return retValue;
}


bool Engine::isCollision(Ball* b)
{
  float radius = b->entity->getWorldBoundingBox().getSize().x/2;
  Ogre::Ray ray(b->node->getPosition()+((radius+5)*b->vel.normalisedCopy()), b->vel);
  mRayQuery->setRay(ray);


  Ogre::RaySceneQueryResult &result = mRayQuery->execute();
  Ogre::RaySceneQueryResult::iterator itr;
  for (itr = result.begin(); itr != result.end(); itr++) 
  {

      Ogre::String name = itr->movable->getParentNode()->getName();

      Ogre::Vector3 collisionNormal(0,0,0);

      if(name == "0")
      {
        collisionNormal.x = 1;
      }
      else if(name == "1")
      {
        collisionNormal.y = 1;
      }
      else if(name == "2")
      {
        collisionNormal.z = 1;
      }
      else if(name == "3")
      {
        collisionNormal.x = -1;
      }
      else if(name == "4")
      {
        collisionNormal.y = -1;
      }
      else if(name == "5")
      {
        collisionNormal.z = -1;
      }

      if(itr->distance < 1 || (itr->movable->getParentNode()->getPosition() - b->node->getPosition()).length() < radius)
      {
        handleCollision(b, collisionNormal);
        return true;
      }
  }
  return false;
}

void Engine::handleCollision(Ball* b, Ogre::Vector3& normal) 
{
  float k = 1.0f;
  Ogre::Vector3 velNormComp = (b->vel.dotProduct(normal))*normal;
  Ogre::Vector3 velTangComp = b->vel - velNormComp;
  b->vel = velTangComp - k*velNormComp;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        srand(time(0));
        Engine app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
