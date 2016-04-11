#include "Room.h"

Room::Room(Ogre::SceneManager* mSceneMgr, Simulator* pSim)
{	
	//surfaces = new Surface*;
	surfaces[0] = new Surface("floor",     mSceneMgr, pSim,  Ogre::Vector3::UNIT_Y, -ROOM_HEIGHT/2.0f, ROOM_WIDTH,  ROOM_LENGTH, ROOM_RESTITUTION, ROOM_FRICTION, "Examples/Rockwall");

	surfaces[1] = new Surface("leftwall",  mSceneMgr, pSim,  Ogre::Vector3::UNIT_X, -ROOM_WIDTH/2.0f,  ROOM_LENGTH, ROOM_HEIGHT, ROOM_RESTITUTION, ROOM_FRICTION, "Examples/Rockwall");
	surfaces[2] = new Surface("rightwall", mSceneMgr, pSim, -Ogre::Vector3::UNIT_X, -ROOM_WIDTH/2.0f,  ROOM_LENGTH, ROOM_HEIGHT, ROOM_RESTITUTION, ROOM_FRICTION, "Examples/Rockwall");

	surfaces[3] = new Surface("backwall",  mSceneMgr, pSim,  Ogre::Vector3::UNIT_Z, -ROOM_LENGTH/2.0f, ROOM_WIDTH,  ROOM_HEIGHT, ROOM_RESTITUTION, ROOM_FRICTION, "Examples/Rockwall");
	surfaces[4] = new Surface("frontwall", mSceneMgr, pSim, -Ogre::Vector3::UNIT_Z, -ROOM_LENGTH/2.0f, ROOM_WIDTH,  ROOM_HEIGHT, ROOM_RESTITUTION, ROOM_FRICTION, "Examples/Rockwall");
	
	surfaces[5] = new Surface("roof",      mSceneMgr, pSim, -Ogre::Vector3::UNIT_Y, -ROOM_HEIGHT/2.0f, ROOM_WIDTH,  ROOM_LENGTH, ROOM_RESTITUTION, ROOM_FRICTION, "Examples/Rockwall");
	
	for (int i = 0; i < 6; ++i) {
		surfaces[i]->addToSimulator();
		surfaces[i]->checkCollisions(true);
	}
}