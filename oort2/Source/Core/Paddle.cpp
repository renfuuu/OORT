#include "Paddle.h"
#include "MultiPlatformHelper.h"
#include "SceneHelper.h"

Paddle::Paddle(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
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

	if (context->hit) {

	}
}

void Paddle::movePaddle(OISManager* _oisManager, int height, int width, float realX, float realY) {

	// Standardize the positions into -1.0 to 1.0 for X and Y then scale them to 800x600. This works for all resolutions now.
	float t1 = ((float)width / 2.0f);
	float t2 = ((float)height / 2.0f);
	realX /= t1;
	realY /= t2;

	int mouseX = Ogre::Math::Clamp((int)(realX*600), -300, 300);
	int mouseY = Ogre::Math::Clamp((int)(realY*300), -100, 200);
	int paddleZ = getZ(mouseX, mouseY);

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

float Paddle::getZ(float x, float y) {
	return (600 / 3) - (Ogre::Math::Sqr(x) / (800 * .75) + Ogre::Math::Sqr((800 / 600)*1.5*y) / (600 * .75));
}

std::string Paddle::getCoordinates() {
	Ogre::Quaternion orient(rootNode->getOrientation());
	std::string ow = std::to_string(orient.w);
	std::string ox = std::to_string(orient.x);
	std::string oy = std::to_string(orient.y);
	std::string oz = std::to_string(orient.z);
	std::string px = std::to_string(rootNode->getPosition().x);
	std::string py = std::to_string(rootNode->getPosition().y);
	std::string pz = std::to_string(rootNode->getPosition().z);

	std::string str = "PDW " + ow + "\nPDX " + ox + "\nPDY " + oy + "\nPDZ " + oz + "\nPPX " + px + "\nPPY " + py + "\nPPZ " + pz;
	
	return str;
}