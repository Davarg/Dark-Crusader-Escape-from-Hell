#include "Box2DContactListener.h"
#include "Common\Player\Player.h"
#include "Scenes\MainGameScene.h"
#include "Common\PlatformBuilder.h"
#include "Common\ConstantsRegistry.h"
#include "UI\ScoresElementMainGame.h"
#include "UI\BackgroundElementMainGame.h"

void Box2DContactListener::BeginContact(b2Contact *contact) {
	std::set<uint16> set;
	set.insert(PlatformBuilder::PlatformCategoryBits::DESTROYABLE_CATEGORY_BITS);
	set.insert(PlatformBuilder::PlatformCategoryBits::NOT_DESTROYABLE_CATEGORY_BITS);
	const uint16 playerCategoryBits = Player::PlayerCategoryBits::PLAYER_CATEGORY_BITS;
	const uint16 fixtureACategoryBits = contact->GetFixtureA()->GetFilterData().categoryBits;
	const uint16 fixtureBCategoryBits = contact->GetFixtureB()->GetFilterData().categoryBits;

	//Platform & Player
	if (set.find(fixtureACategoryBits) != set.end() && fixtureBCategoryBits == playerCategoryBits) {
		contactPlayerPlatform(contact, contact->GetFixtureA(), contact->GetFixtureB());
		return;
	}
	else if (set.find(fixtureBCategoryBits) != set.end() && fixtureACategoryBits == playerCategoryBits) {
		contactPlayerPlatform(contact, contact->GetFixtureB(), contact->GetFixtureA());
		return;
	}

	ScoresElementMainGame *scoresElement = (ScoresElementMainGame*)MainGameScene::getInstance()->getUI()
		->getChildrenByName(ScoresElementMainGame::getElementName());
	//Player & Bottom
	if (fixtureACategoryBits == playerCategoryBits
			&& BackgroundElementMainGame::getBottomCategoryBits() == fixtureBCategoryBits) {
		int *scores = new int(scoresElement->getScores());
		MessagesQueue::Message msg = { MessagesQueue::MessageType::PLAYER_DIE, scores };
		MessagesQueue::getInstance()->addMessageToQueue(msg);
		return;
	}
	else if (fixtureBCategoryBits == playerCategoryBits
			&& BackgroundElementMainGame::getBottomCategoryBits() == fixtureACategoryBits) {
		int *scores = new int(scoresElement->getScores());
		MessagesQueue::Message msg = { MessagesQueue::MessageType::PLAYER_DIE, scores };
		MessagesQueue::getInstance()->addMessageToQueue(msg);
		return;
	}

	//Platform & Bottom
	if (set.find(fixtureACategoryBits) != set.end() 
			&& BackgroundElementMainGame::getBottomCategoryBits() == fixtureBCategoryBits) {
		int *scores = new int(0);

		fixtureACategoryBits == PlatformBuilder::PlatformCategoryBits::DESTROYABLE_CATEGORY_BITS
			? *scores = PlatformBuilder::PlatformDestroyable::getScores()
				: *scores = PlatformBuilder::PlatformNotDestroyable::getScores();

		MessagesQueue::Message msg = { MessagesQueue::MessageType::UPDATE_SCORES, scores };
		MessagesQueue::getInstance()->addMessageToQueue(msg);
		MessagesQueue::Message msg2 = { MessagesQueue::MessageType::DELETE_PLATFORM, contact->GetFixtureA()->GetBody() };
		MessagesQueue::getInstance()->addMessageToQueue(msg2);
		return;
	}
	else if	(set.find(fixtureBCategoryBits) != set.end()
			&& BackgroundElementMainGame::getBottomCategoryBits() == fixtureACategoryBits) {
		int *scores = new int(0);

		fixtureBCategoryBits == PlatformBuilder::PlatformCategoryBits::DESTROYABLE_CATEGORY_BITS
			? *scores = PlatformBuilder::PlatformDestroyable::getScores()
				: *scores = PlatformBuilder::PlatformNotDestroyable::getScores();

		MessagesQueue::Message msg = { MessagesQueue::MessageType::UPDATE_SCORES, scores };
		MessagesQueue::getInstance()->addMessageToQueue(msg);
		MessagesQueue::Message msg2 = { MessagesQueue::MessageType::DELETE_PLATFORM, contact->GetFixtureB()->GetBody() };
		MessagesQueue::getInstance()->addMessageToQueue(msg2);
		return;
	}
}

void Box2DContactListener::contactPlayerPlatform(b2Contact *contact, b2Fixture *fixturePlatform, b2Fixture *fixturePlayer) {
	int numPoints = contact->GetManifold()->pointCount;
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	b2Body *platformBody = fixturePlatform->GetBody();
	const float platformY = 0.35f;
	const float maxPlatformX = 0.95f;
	b2Vec2 relativePointPlatform;

	for (int i = 0; i < numPoints; i++) {
		relativePointPlatform = platformBody->GetLocalPoint(worldManifold.points[i]);
		if (relativePointPlatform.y > platformY
			&& relativePointPlatform.x > -maxPlatformX && relativePointPlatform.x < maxPlatformX) {
			MainGameScene::getInstance()->getPlayer()->isJumped(false);
			return;
		}
	}

	_contacts.push_back(contact);
	contact->SetEnabled(false);
}

//Needed to change density of body
void Box2DContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	for (auto iterator : _contacts)
		iterator->SetEnabled(false);
}

void Box2DContactListener::EndContact(b2Contact* contact) {
	auto iterator = std::find(_contacts.begin(), _contacts.end(), contact);
	if (iterator != _contacts.end()) {
		contact->SetEnabled(true);
		_contacts.erase(iterator);
	}
}