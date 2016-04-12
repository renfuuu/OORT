#pragma once

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderTargetListener.h> 

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <SdkCameraMan.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <unordered_map>
#include <list>
#include <string>
#include <cstring>
#include <cstdlib>

class StadiumFlashes {
	static const int num_flashes = 16;
public:
	StadiumFlashes(Ogre::SceneManager*);
	void startStadiumFlashes();
	void stopStadiumFlashes();
protected:
	Ogre::SceneManager* sManager;
	Ogre::ParticleSystem* flashParticles[num_flashes];
	Ogre::SceneNode* particles[num_flashes];
};