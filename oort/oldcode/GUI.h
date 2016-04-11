#include "OgreTextAreaOverlayElement.h"
#include "OgreStringConverter.h"

#ifndef GUI_h_
#define GUI_h_

class TextBox
{
private:
	OverlayManager* overlayMgr;
	TextAreaOverlayElement* textArea;
	std::string handle;
	std::string current;

public:
    TextBox()
    {
        overlayMgr=OverlayManager::getSingletonPtr();

        handle="element_"+StringConverter::toString(init);
        overlay=overlayMgr->getByName("GUI_OVERLAY");
        panel=static_cast<OverlayContainer*>(overlayMgr->getOverlayElement("GUI"));
        textArea=static_cast<TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea",handle));
    }

    TextBox(std::string s, float x, float y, float r, float g, float b, float i)
    {
        overlayMgr=OverlayManager::getSingletonPtr();

        handle="element_"+StringConverter::toString(init);
        overlay=overlayMgr->getByName("GUI_OVERLAY");
        panel=static_cast<OverlayContainer*>(overlayMgr->getOverlayElement("GUI"));
        textArea=static_cast<TextAreaOverlayElement*>(overlayMgr->createOverlayElement("TextArea",handle));

        setText(s);
        setPosition(x,y);
        setColor(r,g,b);
    }

    ~TextBox()
    {
        handle="element_"+StringConverter::toString(init);
        overlayMgr->destroyOverlayElement(handle);
    }

    TextAreaOverlayElement* getTextArea()
    {
    	return textArea;
    }

    std::string getText()
    {
    	return current;
    }

    void setText(char *str)
    {
    	current = str;
        textArea->setCaption(str);
        textArea->setDimensions(1.0f,1.0f);
        textArea->setMetricsMode(Ogre::GMM_RELATIVE);
        textArea->setFontName("BlueHighway");
        textArea->setCharHeight(0.03f);
    }
    void setText(String str) // now You can use Ogre::String as text
    {
    	current = str;
        textArea->setCaption(str);
        textArea->setDimensions(1.0f,1.0f);
        textArea->setMetricsMode(Ogre::GMM_RELATIVE);
        textArea->setFontName("BlueHighway");
        textArea->setCharHeight(0.03f);
    }
    void setPosition(float x,float y)
    {
        textArea->setPosition(x,y);
    }
    void setColor(float R,float G,float B,float I)
    {
        textArea->setColour(Ogre::ColourValue(R,G,B,I));
    }
};


class GUI
{
	OverlayManager* overlayMgr;
	OverlayContainer* panel;
	Overlay* overlay;
	std::vector<TextBox*> boxes;
public:
	GUI()
    {
        overlayMgr=OverlayManager::getSingletonPtr();
    	panel = static_cast<OverlayContainer*>(overlayMgr->createOverlayElement("Panel","GUI"));
        panel->setMetricsMode(Ogre::GMM_PIXELS);
        panel->setPosition(0,0);
        panel->setDimensions(1.0f,1.0f);
        overlay=overlayMgr->create("GUI_OVERLAY");
        overlay->add2D(panel);
        

        overlay=overlayMgr->getByName("GUI_OVERLAY");
        panel=static_cast<OverlayContainer*>(overlayMgr->getOverlayElement("GUI"));
        overlay->show();
    }
    ~GUI()
    {
    	for (std::vector<TextBox*>::iterator i = boxes.begin(); i != boxes.end(); ++i)
    	{
    		delete *i;
    	}
       
        overlayMgr->destroyOverlayElement("GUI");
        overlayMgr->destroy("GUI_OVERLAY");
    }

	TextBox* addTextBox(std::string s, float x, float y, float r, float g, float b, float i)
	{
		boxes.push_back(new TextBox(s,x,y,r,g,b,i));
		panel.addChild(boxes.back()->getTextArea());
	}

	TextBox* getTextBox(int i)
	{
		return boxes[i];
	}

	TextBox* getTextBox(std::string msg)
	{
		for (std::vector<TextBox*>::iterator i = boxes.begin(); i != boxes.end(); ++i)
		{
			if((*i)->getText() == msg)
			{
				return *i;
			}
		}
		return NULL:
	}
};


#endif