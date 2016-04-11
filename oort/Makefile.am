ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h GameManager.h Ball.h Simulator.h GameObject.h OgreMotionState.h SoundManager.h Paddle.h Room.h Surface.h Goal.h NetManager.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BaseApplication.cpp GameManager.cpp Ball.cpp Simulator.cpp GameObject.cpp OgreMotionState.cpp SoundManager.cpp Paddle.cpp Room.cpp Surface.cpp Goal.cpp NetManager.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) -I/usr/include/SDL
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system -lSDL -lSDL_mixer -lSDL_net

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign

AM_LDFLAGS = -d -rdynamic

clean-submit: clean
	-rm -f install-sh
	-rm -f depcomp
	-rm -f configure
	-rm -f ltmain.sh
	-rm -f stamp-h1
	-rm -f Makefile
	-rm -f Makefile.in
	-rm -f Ogre.*
	-rm -f missing
	-rm -f ogre.*
	-rm -f libtool
	-rm -f config.*
	-rm -f compile
	-rm -f *.m4
	-rm -rf m4
	-rm -rf *.cache
