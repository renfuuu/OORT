#include "Ball.h"

Ball::Ball(Ogre::String name, Ogre::SceneManager::PrefabType meshName, Ogre::SceneManager* mSceneMgr, Ogre::Vector3 position, Simulator* pSim) : 
		GameObject(name, mSceneMgr, BALL_RESTITUTION, BALL_FRICTION, pSim)
{
    score = 0;
    lives = BEGIN_LIVES;

	geom = mSceneMgr->createEntity(name, meshName);
    geom->setMaterialName("Examples/RustySteel");
    geom->setCastShadows(true);
	rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	rootNode->attachObject(geom);
	rootNode->setPosition(position);
	this->radius = BALL_RADIUS; 
    rootNode->setScale(Ogre::Vector3(BALL_RADIUS/50.0f, BALL_RADIUS/50.0f, BALL_RADIUS/50.0f));
	shape = new btSphereShape(this->radius);
	this->mass = 100; // Maybe make this another const var

    this->tr.setOrigin(btVector3(position.x, position.y, position.z));
    shape->setLocalScaling(btVector3(BALL_RADIUS/50.0f, BALL_RADIUS/50.0f, BALL_RADIUS/50.0f));

    SoundManager* mixer = SoundManager::getInstance();
    char* sound = "media/audio/bounce.ogg";
    bounceSound = mixer->onLoad(sound);
    sound = "media/audio/paddle.ogg";
    paddleSound = mixer->onLoad(sound);
    sound = "media/audio/loseLife.ogg";
    lossSound = mixer->onLoad(sound);
    sound = "media/audio/hitGoal.ogg";
    scoreSound = mixer->onLoad(sound);
}

// Note: sound stuff here
void Ball::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getWorld()->contactTest(body, *cCallBack);

    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0) 
        && (lastTime > 0.015 || (context->lastBody != context->body && lastTime > 0.003))) {
        SoundManager* mixer = SoundManager::getInstance();
    	cout << "hit" << endl;
        
        if (context->theObject->getName() == "frontwall") {
            mixer->play(lossSound);
            --lives;
        } else if (context->theObject->getName() == "playerPaddle") {
            mixer->play(paddleSound);
        } else if (context->theObject->getName() == "Goal") {
            mixer->play(scoreSound);
            ++score;
        } else {
            mixer->play(bounceSound);
        }

        lastTime = 0.0f;
    }
    
    context->hit = false;
}