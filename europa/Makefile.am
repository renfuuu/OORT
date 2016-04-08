ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h Engine.h

bin_PROGRAMS= Engine
Engine_CPPFLAGS= -I$(top_srcdir)
Engine_SOURCES= BaseApplication.cpp Engine.cpp
Engine_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS)
Engine_LDADD= $(OGRE_LIBS) $(OIS_LIBS)
Engine_LDFLAGS= -lOgreOverlay -lboost_system

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
