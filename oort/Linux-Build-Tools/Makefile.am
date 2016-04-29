noinst_HEADERS = Application.h MultiPlatformHelper.h OISManager.h SceneHelper.h CoreConfig.h SoundManager.h ScoreManager.h GameManager.h  GameObject.h Simulator.h BulletContactCallback.h CollisionContext.h OgreMotionState.h Spaceship.h Wall.h Laser.h Asteroid.h tinyxml2.h

bin_PROGRAMS = oort
oort_CPPFLAGS = -I$(top_srcdir) -std=c++11 -Wunused-variable
oort_SOURCES = Application.cpp main.cpp OISManager.cpp SoundManager.cpp ScoreManager.cpp GameManager.cpp Simulator.cpp GameObject.cpp OgreMotionState.cpp CollisionContext.cpp BulletContactCallback.cpp Spaceship.cpp Wall.cpp Laser.cpp Asteroid.cpp tinyxml2.cpp
oort_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS)
oort_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)
oort_LDFLAGS = -lOgreOverlay -lboost_system -lSDL -lSDL_mixer -R/lusr/lib/cegui-0.8

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
