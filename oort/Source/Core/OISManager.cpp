#include <OISInputManager.h>

#include "MultiPlatformHelper.h"
#include "OISManager.h"
 
OISManager *OISManager::mOISManager;
 
OISManager::OISManager( void ) :
    mMouse( 0 ),
    mKeyboard( 0 ),
    mInputSystem( 0 ) {
}
 
OISManager::~OISManager( void ) {
    if( mInputSystem ) {
        if( mMouse ) {
            mInputSystem->destroyInputObject( mMouse );
            mMouse = 0;
        }
 
        if( mKeyboard ) {
            mInputSystem->destroyInputObject( mKeyboard );
            mKeyboard = 0;
        }

        // If you use OIS1.0RC1 or above, uncomment this line
        // and comment the line below it
        mInputSystem->destroyInputSystem( mInputSystem );
        //mInputSystem->destroyInputSystem();
        mInputSystem = 0;
 
        // Clear Listeners
        mKeyListeners.clear();
        mMouseListeners.clear();
    }
}
 
void OISManager::initialise( Ogre::RenderWindow *renderWindow ) {
    if( !mInputSystem ) {

        /*mInputSystem->destroyInputObject(mMouse);
        mInputSystem->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputSystem);
*/
        // Setup basic variables
        OIS::ParamList paramList;    
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
 
        // Get window handle
        renderWindow->getCustomAttribute( "WINDOW", &windowHnd );
 
        // Fill parameter list
        windowHndStr << (unsigned int) windowHnd;
        paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );
 
    #if defined OIS_WIN32_PLATFORM
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    #elif defined OIS_LINUX_PLATFORM
        paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
        paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
        paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif

        // Create inputsystem
        mInputSystem = OIS::InputManager::createInputSystem( paramList );
    


        //mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
        
        // If possible create a buffered keyboard
        // (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
        //if( mInputSystem->numKeyboards() > 0 ) {
        if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
            mKeyboard = static_cast<OIS::Keyboard*>( mInputSystem->createInputObject( OIS::OISKeyboard, true ) );
            mKeyboard->setEventCallback( this );
        }
 
        // If possible create a buffered mouse
        // (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
        //if( mInputSystem->numMice() > 0 ) {
        if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
            mMouse = static_cast<OIS::Mouse*>( mInputSystem->createInputObject( OIS::OISMouse, true ) );
            mMouse->setEventCallback( this );
 
            // Get window size
            unsigned int width, height, depth;
            int left, top;
            renderWindow->getMetrics( width, height, depth, left, top );
 
            // Set mouse region
            this->setWindowExtents( width, height );
        }
    }
}
 
void OISManager::capture( void ) {
    // Need to capture / update each device every frame
    if( mMouse ) {
        mMouse->capture();
    }
 
    if( mKeyboard ) {
        mKeyboard->capture();
    }
}
 
void OISManager::addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName ) {
    if( mKeyboard ) {
        // Check for duplicate items
        itKeyListener = mKeyListeners.find( instanceName );
        if( itKeyListener == mKeyListeners.end() ) {
            mKeyListeners[ instanceName ] = keyListener;
        }
        else {
            // Duplicate Item
        }
    }
}
 
void OISManager::addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName ) {
    if( mMouse ) {
        // Check for duplicate items
        itMouseListener = mMouseListeners.find( instanceName );
        if( itMouseListener == mMouseListeners.end() ) {
            mMouseListeners[ instanceName ] = mouseListener;
        }
        else {
            // Duplicate Item
        }
    }
}
 
 
void OISManager::removeKeyListener( const std::string& instanceName ) {
    // Check if item exists
    itKeyListener = mKeyListeners.find( instanceName );
    if( itKeyListener != mKeyListeners.end() ) {
        mKeyListeners.erase( itKeyListener );
    }
    else {
        // Doesn't Exist
    }
}
 
void OISManager::removeMouseListener( const std::string& instanceName ) {
    // Check if item exists
    itMouseListener = mMouseListeners.find( instanceName );
    if( itMouseListener != mMouseListeners.end() ) {
        mMouseListeners.erase( itMouseListener );
    }
    else {
        // Doesn't Exist
    }
}

 
void OISManager::removeKeyListener( OIS::KeyListener *keyListener ) {
    itKeyListener    = mKeyListeners.begin();
    itKeyListenerEnd = mKeyListeners.end();
    for(; itKeyListener != itKeyListenerEnd; ++itKeyListener ) {
        if( itKeyListener->second == keyListener ) {
            mKeyListeners.erase( itKeyListener );
            break;
        }
    }
}
 
void OISManager::removeMouseListener( OIS::MouseListener *mouseListener ) {
    itMouseListener    = mMouseListeners.begin();
    itMouseListenerEnd = mMouseListeners.end();
    for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) {
        if( itMouseListener->second == mouseListener ) {
            mMouseListeners.erase( itMouseListener );
            break;
        }
    }
}

void OISManager::removeAllListeners( void ) {
    mKeyListeners.clear();
    mMouseListeners.clear();
}
 
void OISManager::removeAllKeyListeners( void ) {
    mKeyListeners.clear();
}
 
void OISManager::removeAllMouseListeners( void ) {
    mMouseListeners.clear();
}

void OISManager::setWindowExtents( int width, int height ) {
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = mMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
}
 
OIS::Mouse* OISManager::getMouse( void ) {
    return mMouse;
}
 
OIS::Keyboard* OISManager::getKeyboard( void ) {
    return mKeyboard;
}

 
bool OISManager::keyPressed( const OIS::KeyEvent &e ) {
    mKeyPressed = e.key;

    return true;
}

OIS::KeyCode OISManager::lastKeyPressed() {
    OIS::KeyCode ret = mKeyPressed;
    mKeyPressed = OIS::KC_UNASSIGNED;
    return ret;
}

bool OISManager::keyReleased( const OIS::KeyEvent &e ) {
    return true;
}
 
bool OISManager::mouseMoved( const OIS::MouseEvent &e ) {

    // From -width/2 to +width/2
    mouseXAxis = (e.state.X.abs) - e.state.width/2;
    mouseYAxis = (e.state.Y.abs) - e.state.height/2;

    return true;
}
 
bool OISManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    return true;
}
 
bool OISManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    return true;
}
 
OISManager* OISManager::getSingletonPtr( void ) {
    if( !mOISManager ) {
        mOISManager = new OISManager();
    }
 
    return mOISManager;
}

int OISManager::getMouseXAxis() {
    return mouseXAxis;
}

int OISManager::getMouseYAxis() {
    return mouseYAxis;
}

OIS::KeyCode OISManager::getKeyPressed(){
    return mKeyPressed;
}