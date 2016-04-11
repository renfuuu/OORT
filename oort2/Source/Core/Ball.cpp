#include "Ball.h"
#include "MultiPlatformHelper.h"

Ball::Ball(Ogre::String nme, GameObject::objectType tp, Ogre::SceneManager* scnMgr, GameManager* ssm, Ogre::SceneNode* node, Ogre::Entity* ent, OgreMotionState* ms, Simulator* sim, Ogre::Real mss, Ogre::Real rest, Ogre::Real frict, Ogre::Real scal, bool kin) : 
GameObject(nme, tp, scnMgr, ssm, node, ent, ms, sim, mss, rest, frict, scal, kin) {
	// Gets the radius of the Ogre::Entity sphere
	shape = new btSphereShape((ent->getBoundingBox().getHalfSize().x)*scale);

	Ogre::SceneNode* particleNode = rootNode->createChildSceneNode("Particle");
	particleNode->attachObject(particle);
}

Ball::~Ball() {
}

void Ball::updateTransform() {
	Ogre::Vector3 pos = rootNode->getPosition();
	tr.setOrigin(btVector3(pos.x, pos.y, pos.z));

	Ogre::Quaternion qt = rootNode->getOrientation();
	tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));

	motionState->updateTransform(tr);
}

void Ball::update() {

	if (context->hit) {

		startScore();
		if(previousHit != nullptr) {
			// Check for paddle collision but not twice in a row
			if( context->getTheObject()->getType() == GameObject::PADDLE_OBJECT && context->getTheObject() != previousHit ) {
				gameManager->playSound(GameManager::PADDLE_BOUNCE);
			}		

			if (gameManager->isServer() ) {
				//Score only when you hit behind your opponent.
				if ( context->getTheObject()->getType() == GameObject::BACK_WALL_OBJECT && previousHit->getType() != GameObject::BACK_WALL_OBJECT) {
					gameManager->scorePoints(1);
					gameManager->playSound(GameManager::SCORE);
					reset(startPos);
				}
				else if ( context->getTheObject()->getType() == GameObject::FRONT_WALL_OBJECT && previousHit->getType() != GameObject::FRONT_WALL_OBJECT) {
					Ogre::Vector3 vec(-startPos.x, startPos.y, -startPos.z);
					gameManager->scoreOpponentPoints(1);
					reset(vec);
				}
			}
		}
		
		previousHit = context->getTheObject();
	}
}

std::string Ball::getCoordinates() {
	btVector3 velocity = getBody()->getLinearVelocity();
	std::string px = std::to_string(rootNode->getPosition().x);
	std::string py = std::to_string(rootNode->getPosition().y);
	std::string pz = std::to_string(rootNode->getPosition().z);
	std::string vx = std::to_string(velocity.x());
	std::string vy = std::to_string(velocity.y());
	std::string vz = std::to_string(velocity.z());

	std::string str = "BPX " + px + "\nBPY " + py + "\nBPZ " + pz + "\nBVX " + vx + "\nBVY " + vy + "\nBVZ " + vz;
	return str;
}

std::string Ball::getPoints() {
	std::string str = "SCS " + std::to_string(gameManager->getGameScore()) + "\nSCC " + std::to_string(gameManager->getEnemyScore());
	return str;
}