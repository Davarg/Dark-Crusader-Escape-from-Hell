#include "PlatformManager.h"
#include "Scenes\MainGameScene.h"
#include "Common\ConstantsRegistry.h"

PlatformManager::~PlatformManager() {
	MessagesQueue::getInstance()->removeListener(MessagesQueue::MessageType::DELETE_PLATFORM, *_deletePlatformCallback);
	CC_SAFE_DELETE(_deletePlatformCallback);
}

PlatformManager::PlatformManager() {
	_minSizeBetweenPlatforms = Size(70.4f, 32); //Pxl
	_maxSizeBetweenPlatforms = Size(140.8f, 64); //Pxl
	_platformRequestCount = 0;
	_minGenPlatformCount = 100;
	_lastPlatform = nullptr;
	_platformCount = 0;

	_deletePlatformCallback = new MessagesQueue::WrapperMessageQueueCallback_1(CC_CALLBACK_1(PlatformManager::platformDestroyCallback
			, this)
		, "Box2DContactListenerDeleteBody");
	MessagesQueue::getInstance()->addListener(MessagesQueue::MessageType::DELETE_PLATFORM, *_deletePlatformCallback);
}

void PlatformManager::addPlatforms(Layer *layer, int count) {
	if (!count)
		count = _minGenPlatformCount;
	_lastLayer = layer;
	const float distanceFactorX = (_minSizeBetweenPlatforms.width * _platformRequestCount) / 10;
	const float distanceFactorY = (_minSizeBetweenPlatforms.height * _platformRequestCount) / 10;
	const int substractFactor = (count * (_platformRequestCount / 2)) / 100;

	float minBorderY = 0;
	float maxBorderY = _maxSizeBetweenPlatforms.height + distanceFactorY;
	std::uniform_int_distribution<int> posCoordX(23, 953);
	std::uniform_int_distribution<int> posCoordY(minBorderY, maxBorderY);
	std::random_device randomDevice;
	std::default_random_engine randomEngine(randomDevice());

	std::vector<PlatformBuilder::Platform*> platforms = PlatformBuilder::generateRandomPlatforms(count - substractFactor);
	_platformCount += platforms.size();

	int counter = 0;
	int x = 0;
 	int y = _lastPlatform ? _lastPlatform->getSprite()->getPositionY() : 0;
	if (!_lastPlatform)
		_lastPlatform = platforms.front();
	for (auto iterator : platforms) {
		x = posCoordX(randomEngine);
		if (counter % 4 == 0 && counter != 0)
			y += _maxSizeBetweenPlatforms.height;
		//iterator->setPosition(Vec3(455, 100, 0));
		Vec3 tmpVec3 = Vec3(x, y + posCoordY(randomEngine), 0);
		if (_lastPlatform->getSprite()->getPositionY() < iterator->getSprite()->getPositionY() + tmpVec3.y)
			_lastPlatform = iterator;

		iterator->setPosition(tmpVec3);
		layer->addChild(iterator->getBackLayer());
		counter++;
	}
	_platformRequestCount++;
}

void PlatformManager::platformDestroyCallback(void *args) {
	b2Body *body = static_cast<b2Body*>(args);
	body->SetUserData(nullptr); //cocos2dx will remove sprite automaticaly
	MainGameScene::getInstance()->getWorld()->DestroyBody(body);
	--_platformCount;

	if (_platformCount == 50)
		addPlatforms(_lastLayer, 50);
}

void PlatformManager::playerJumpCallback(void *args) {
	b2World *world = MainGameScene::getInstance()->getWorld();
	for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData()) {
			Sprite *spr = static_cast<Sprite*>(body->GetUserData());
			const uint16 fixtureCategoryBits = body->GetFixtureList()->GetFilterData().categoryBits;
			
			switch (fixtureCategoryBits) {
			case PlatformBuilder::PlatformCategoryBits::DESTROYABLE_CATEGORY_BITS:
			case PlatformBuilder::PlatformCategoryBits::NOT_DESTROYABLE_CATEGORY_BITS:
				body->ApplyLinearImpulse({ 0, -9.3f }, body->GetWorldCenter(), true);
				break;

			case Player::PlayerCategoryBits::PLAYER_CATEGORY_BITS:
				body->ApplyLinearImpulse({ 0, 0.007f }, body->GetWorldCenter(), true);
				break;
			}
		}
	}
}