#ifndef __Engine_h_
#define __Engine_h_
 
#include "BaseApplication.h"


struct Ball
{
	static int instances;
	int id;


	Ogre::Vector3 vel;
	Ogre::Entity* entity;
	Ogre::SceneNode* node;
	Ball(int _id);
	~Ball();

	void move();

};

class Engine : public BaseApplication
{
public:
  Engine();
  virtual ~Engine();
  std::vector<Ball*> balls;

 
protected:
  virtual void createScene();
  virtual void createCamera();
  virtual void createViewports();
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  bool isCollision(Ball* b);
  void handleCollision(Ball* b, Ogre::Vector3& normal);

};
 
#endif // #ifndef _Engine_h_
