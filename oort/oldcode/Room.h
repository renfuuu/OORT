#ifndef ROOM_H
#define ROOM_H

#include "Surface.h"
#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

const Ogre::Real  ROOM_WIDTH  = 1500.0;
const Ogre::Real  ROOM_HEIGHT = 1500.0;
const Ogre::Real  ROOM_LENGTH = 2500.0;

class Room {
protected:
private:
	const static btScalar 	 ROOM_RESTITUTION = 1.0f;
	const static btScalar    ROOM_FRICTION = 0.085f;
	Surface* surfaces[];
public:
	Room(Ogre::SceneManager*, Simulator*);

};

#endif 
