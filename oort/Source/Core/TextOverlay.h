#pragma once

#include <Overlay/OgreTextAreaOverlayElement.h>
#include "OgreStringConverter.h"
#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlay.h>
#include <Overlay/OgreOverlayContainer.h>

class TextOverlay {
public:
	TextOverlay(Ogre::Overlay*, Ogre::String, float, float, float);
	~TextOverlay(void);
	virtual void setText(Ogre::String);
	virtual void setPos(float, float);
	virtual void setCol(float, float, float, float);
	virtual void showOverlay(void);
	virtual void hideOverlay(void);

protected:
	float charHeight;
	Ogre::OverlayManager* olm;
	Ogre::Overlay *overlay;
	Ogre::TextAreaOverlayElement *textArea;
	Ogre::String name;
};