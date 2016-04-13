#include "TextOverlay.h"

TextOverlay::TextOverlay(Ogre::Overlay* ovly, Ogre::String nme, float x, float y, float ch) : name(nme), charHeight(ch), overlay(ovly) {
	olm = Ogre::OverlayManager::getSingletonPtr();
	Ogre::OverlayElement* elem1 = olm->createOverlayElement("Panel", name + "GUI");
	elem1->setMetricsMode(Ogre::GMM_PIXELS);
	elem1->setPosition(x, y);
	elem1->setDimensions(1.0f, 1.0f);

	Ogre::OverlayContainer* overlayCon = static_cast<Ogre::OverlayContainer*>(elem1);

	// Cast the parent pointer from factory
	textArea = static_cast<Ogre::TextAreaOverlayElement*>(olm->createOverlayElementFromFactory("TextArea", name));
	overlayCon->addChild(textArea);

	overlay->add2D(overlayCon);
	setPos(x,y);
}

TextOverlay::~TextOverlay(void) {
	olm->destroyOverlayElement(name);
	olm->destroyOverlayElement(name + "GUI");
	olm->destroy(name);
}

void TextOverlay::setText(Ogre::String str) {
	textArea->setCaption(str);
	textArea->setDimensions(1.0f, 1.0f);
	textArea->setMetricsMode(Ogre::GMM_RELATIVE);
	textArea->setFontName("dameron");
	textArea->setCharHeight(charHeight);
}

void TextOverlay::setPos(float x, float y) {
	textArea->setPosition(x, y);
}

void TextOverlay::setCol(float r, float g, float b, float a) {
	textArea->setColour(Ogre::ColourValue(r, g, b, a));
}

void TextOverlay::showOverlay(void) {
	textArea->show();
}

void TextOverlay::hideOverlay(void) {
	textArea->hide();
}