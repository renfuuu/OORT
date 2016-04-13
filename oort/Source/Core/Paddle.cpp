#include "Paddle.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Paddle::Paddle(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, SoundScoreManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	kinematic = true;
	auto var = ent->getBoundingBox();

	// Bullet uses half margins for collider
	auto size = var.getSize()/2;

	shape = new btBoxShape(btVector3(size.x*scale, size.y*scale, size.z*scale));
}

Paddle::~Paddle() {
}

void Paddle::update() {

	static int MAX_DT = 5;

	if (context->hit) {
		Ogre::Real dt = soundScoreManager->getTime() - lastHitTime;
		if ( dt > MAX_DT )
			lastHitTime = soundScoreManager->getTime();
	}
}

void Paddle::movePaddle(OISManager* _oisManager, int height, int width) {

	// Standardize the positions into -1.0 to 1.0 for X and Y then scale them to 800x600. This works for all resolutions now.
	float t1 = ((float)width / 2.0f);
	float t2 = ((float)height / 2.0f);
	float realX = (float)_oisManager->getMouseXAxis() / t1;
	float realY = (float)_oisManager->getMouseYAxis() / t2;

	int mouseX = Ogre::Math::Clamp((int)(realX*600), -300, 300);
	int mouseY = Ogre::Math::Clamp((int)(realY*300), -100, 200);
	int paddleZ = (600/3)-(Ogre::Math::Sqr(mouseX)/(800*.75) + Ogre::Math::Sqr((800/600)*1.5*mouseY)/(600*.75));

	Ogre::SceneNode* mNode = rootNode;

	Ogre::Vector3 surfacePoint = Ogre::Vector3(mouseX, paddleZ, mouseY);
	Ogre::Quaternion orient = mNode->getOrientation();
	Ogre::Vector3 normal = surfacePoint.normalisedCopy();
	Ogre::Vector3 normalCopy = -(surfacePoint + Ogre::Vector3(0,0,0)).normalisedCopy();
	Ogre::Vector3 ortho1 = (Ogre::Vector3(0, 1, 0).crossProduct(normalCopy)).normalisedCopy();
	Ogre::Vector3 ortho2 = (normalCopy.crossProduct(ortho1)).normalisedCopy();

	Ogre::Quaternion newOrientation(ortho1, ortho2, normalCopy);
	mNode->setOrientation(newOrientation);

	this->setPosition(surfacePoint + normal*50 - startPos);
	
	Ogre::Vector3 u;
	if (mouseX < 0) 
		u = newOrientation.yAxis();
	else 
		u = -newOrientation.yAxis();
	
	Ogre::Vector3 v = Ogre::Vector3(0, 0, 1);
	Ogre::Real cosine = u.dotProduct(v);

	Ogre::Real sin = u.crossProduct(v).length();
	mNode->roll(Ogre::Math::ATan2(sin, cosine));

	updateTransform();
}