#include "MainGameScene.h"
#include "Common\PlatformBuilder.h"
#include "Common\ConstantsRegistry.h"
#include "UI\BackgroundElementMainGame.h"

MainGameScene* MainGameScene::gameScene = nullptr;

MainGameScene::~MainGameScene() {
	CC_SAFE_DELETE(_ui);
	CC_SAFE_DELETE(_world);
	CC_SAFE_DELETE(_player);
#ifdef _DEBUG
	CC_SAFE_DELETE(_debugDraw);
#endif
	CC_SAFE_DELETE(_playerInput);
	CC_SAFE_DELETE(_platformManager);
	MessagesQueue::getInstance()->removeListener(MessagesQueue::MessageType::PLAYER_JUMP, *_playerJumpCallback);
	CC_SAFE_DELETE(_playerJumpCallback);
}

#ifdef _DEBUG
	void MainGameScene::draw(Renderer*, const Mat4&, uint32_t) {
		glEnableVertexAttribArray(0);
		_world->DrawDebugData();
	}
#endif

Scene* MainGameScene::createScene() {
    auto scene = Scene::create();
	auto layer = MainGameScene::create();
	gameScene = layer;

    scene->addChild(layer);
	
	b2Vec2 gravityVec{ 0, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y) };
	layer->_world = new b2World(gravityVec);

	layer->_ui = new UserInterfaceMainGame(layer, layer->getContentSize());
	layer->_ui->show();

	layer->_player = new Player();
	layer->_player->setPositionPxl({ 512, 300, 0 });
	layer->addChild(layer->_player->getSprite(), 2);
	layer->_playerJumpCallback = new MessagesQueue::WrapperMessageQueueCallback_1(
		CC_CALLBACK_1(PlatformManager::playerJumpCallback, layer->_platformManager)
		, "PlatformManagerPlayerJump");
	MessagesQueue::getInstance()->addListener(MessagesQueue::MessageType::PLAYER_JUMP, *layer->_playerJumpCallback);

	layer->_playerInput = new PlayerInput(layer->_player);

	layer->_platformManager = new PlatformManager();
	layer->_platformManager->addPlatforms(layer, 100);

#ifdef _DEBUG
	layer->_debugDraw = new GLESDebugDraw(ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D));
	uint32  flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_centerOfMassBit;
	layer->_debugDraw->SetFlags(flags);
	layer->_world->SetDebugDraw(layer->_debugDraw);
#endif
	
	layer->scheduleUpdate();
	MainGameScene::getInstance()->getWorld()->SetContactListener(&layer->_contactListener);
	MainGameScene::getInstance()->getWorld()->SetContactFilter(&layer->_contactFilter);

    return scene;
}

bool MainGameScene::init() {
    if (!Layer::init())
        return false;

    return true;
}

b2World* MainGameScene::getWorld() {
	if (!gameScene->_world) {
		b2Vec2 gravityVec{ 0, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y) };
		gameScene->_world = new b2World(gravityVec);
	}

	return gameScene->_world;
}

void MainGameScene::update(float dt) {
	const int velocityIterations = 10;
	const int positionIterations = 10;	for (b2Body *body = MainGameScene::getWorld()->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData()) {
			body->SetAngularVelocity(0);
			
			Sprite *spr = static_cast<Sprite*>(body->GetUserData());
			if (spr != nullptr) {
				const Size size = spr->getContentSize();
				const Vec2 offsetDstr = PlatformBuilder::PlatformDestroyable::getOffsetPxl();
				const Vec2 offsetNotDstr = PlatformBuilder::PlatformNotDestroyable::getOffsetPxl();
				const uint16 fixtureCategoryBits = body->GetFixtureList()->GetFilterData().categoryBits;

				switch (fixtureCategoryBits) {
				case PlatformBuilder::DESTROYABLE_CATEGORY_BITS:
					body->ApplyForceToCenter({ 0, body->GetMass()
						* -ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y) }, true);
					spr->setPosition3D({ (body->GetPosition().x
							* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)) + offsetDstr.x
						, (body->GetPosition().y
							* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)) - offsetDstr.y
						, 0 });
					break;

				case PlatformBuilder::NOT_DESTROYABLE_CATEGORY_BITS:
					body->ApplyForceToCenter({ 0, body->GetMass()
						* -ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y) }, true);
					spr->setPosition3D({ (body->GetPosition().x
							* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)) - offsetNotDstr.x
						, (body->GetPosition().y
							* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)) - offsetNotDstr.y
						, 0 });
					break;

				default:
					spr->setPosition3D({ body->GetPosition().x
							* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
						, body->GetPosition().y
							* ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
						, 0});
					break;
				}
				spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
			}
		}
	}
	MainGameScene::getWorld()->Step(dt, velocityIterations, positionIterations);
}