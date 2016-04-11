#ifndef __Surface_h_
#define __Surface_h_

#include "GameObject.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include "Simulator.h"
#include <Ogre.h>

class Surface : public GameObject {
    private:
	    float lastTime;
	public:
		Surface(Ogre::String name, 
		    Ogre::SceneManager* mgr, 
		    Simulator* sim, 
		    Ogre::Vector3 normal,
		    Ogre::Real offset, 
		    Ogre::Real width, 
		    Ogre::Real height,
		    btScalar restitution,
		    btScalar friction,
		    Ogre::String texture);

        void update(float);
};

#endif