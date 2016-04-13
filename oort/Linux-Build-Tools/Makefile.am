noinst_HEADERS = Application.h MultiPlatformHelper.h OISManager.h SceneHelper.h CoreConfig.h SoundManager.h ScoreManager.h SoundScoreManager.h TextOverlay.h GameObject.h Simulator.h BulletContactCallback.h CollisionContext.h OgreMotionState.h Spaceship.h Wall.h

bin_PROGRAMS = oort
oort_CPPFLAGS = -I$(top_srcdir) -std=c++11 -Wunused-variable
oort_SOURCES = Application.cpp main.cpp OISManager.cpp SoundManager.cpp ScoreManager.cpp SoundScoreManager.cpp Simulator.cpp TextOverlay.cpp GameObject.cpp OgreMotionState.cpp CollisionContext.cpp BulletContactCallback.cpp Spaceship.cpp Wall.cpp
oort_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
oort_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
oort_LDFLAGS = -lOgreOverlay -lboost_system -lSDL -lSDL_mixer

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
