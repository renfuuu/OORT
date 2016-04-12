#include "StadiumFlashes.h"


StadiumFlashes::StadiumFlashes(Ogre::SceneManager* smgr) {
	sManager = smgr;
	//Right Side
	flashParticles[0] = sManager->createParticleSystem("StadiumFlash0", "Flash");
	particles[0] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle0");
	particles[0]->_setDerivedPosition(Ogre::Vector3(3700, 0, -1000));
	particles[0]->attachObject(flashParticles[0]);

	flashParticles[1] = sManager->createParticleSystem("StadiumFlash1", "Flash");
	particles[1] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle1");
	particles[1]->_setDerivedPosition(Ogre::Vector3(3700, -300, 0));
	particles[1]->attachObject(flashParticles[1]);

	flashParticles[2] = sManager->createParticleSystem("StadiumFlash2", "Flash");
	particles[2] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle2");
	particles[2]->_setDerivedPosition(Ogre::Vector3(3700, 0, 1000));
	particles[2]->attachObject(flashParticles[2]);

	//Left Side
	flashParticles[3] = sManager->createParticleSystem("StadiumFlash3", "Flash");
	particles[3] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle3");
	particles[3]->_setDerivedPosition(Ogre::Vector3(-3700, 0, -1000));
	particles[3]->attachObject(flashParticles[3]);

	flashParticles[4] = sManager->createParticleSystem("StadiumFlash4", "Flash");
	particles[4] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle4");
	particles[4]->_setDerivedPosition(Ogre::Vector3(-3700, -300, 0));
	particles[4]->attachObject(flashParticles[4]);

	flashParticles[5] = sManager->createParticleSystem("StadiumFlash5", "Flash");
	particles[5] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle5");
	particles[5]->_setDerivedPosition(Ogre::Vector3(-3700, 0, 1000));
	particles[5]->attachObject(flashParticles[5]);

	//Back
		//Lower
	flashParticles[8] = sManager->createParticleSystem("StadiumFlash8", "Flash");
	particles[8] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle8");
	particles[8]->_setDerivedPosition(Ogre::Vector3(0, 0, 3500));
	particles[8]->attachObject(flashParticles[8]);
		
		//Upper Left
	flashParticles[10] = sManager->createParticleSystem("StadiumFlash10", "Flash");
	particles[10] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle10");
	particles[10]->_setDerivedPosition(Ogre::Vector3(-3000, 1500, 5100));
	particles[10]->attachObject(flashParticles[10]);

		//Upper Right
	flashParticles[11] = sManager->createParticleSystem("StadiumFlash11", "Flash");
	particles[11] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle11");
	particles[11]->_setDerivedPosition(Ogre::Vector3(3000, 1500, 5100));
	particles[11]->attachObject(flashParticles[11]);

	//Front
		//Lower
	flashParticles[12] = sManager->createParticleSystem("StadiumFlash12", "Flash");
	particles[12] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle12");
	particles[12]->_setDerivedPosition(Ogre::Vector3(0, -100, -3500));
	particles[12]->attachObject(flashParticles[12]);
		
		//Upper Left
	flashParticles[14] = sManager->createParticleSystem("StadiumFlash14", "Flash");
	particles[14] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle14");
	particles[14]->_setDerivedPosition(Ogre::Vector3(-3000, 1500, -5100));
	particles[14]->attachObject(flashParticles[14]);

		//Upper Right
	flashParticles[15] = sManager->createParticleSystem("StadiumFlash15", "Flash");
	particles[15] = sManager->getSceneNode("stadium")->createChildSceneNode("FlashParticle15");
	particles[15]->_setDerivedPosition(Ogre::Vector3(3000, 1500, -5100));
	particles[15]->attachObject(flashParticles[15]);
}