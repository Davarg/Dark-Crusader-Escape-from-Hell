#include "Scenes\MainGameScene.h"
#include "Common\ConstantsRegistry.h"
#include "BackgroundElementMainGame.h"

const uint16 BackgroundElementMainGame::bottomCategoryBits = 0x0030;
const std::string BackgroundElementMainGame::backgroundImgPath = "background.png";
const std::string BackgroundElementMainGame::elementName = "BackgroundElementMainGame";

BackgroundElementMainGame::BackgroundElementMainGame(Layer *layer, Size size) {
	_layerParent = layer;
	_layerBack = Layer::create();
	_layerBack->setPosition3D({ 0, 0, 0 });
	_layerBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	auto spriteBackground = Sprite::create(backgroundImgPath);
	spriteBackground->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	spriteBackground->setScaleX(size.width / spriteBackground->getContentSize().width);
	spriteBackground->setScaleY(size.height / spriteBackground->getContentSize().height);
#ifdef _DEBUG
	spriteBackground->setOpacity(50);
#endif
	_layerBack->addChild(spriteBackground, -1);

	createGlass();
}

void BackgroundElementMainGame::createGlass() {
	b2BodyDef glassDef;
	glassDef.type = b2_staticBody;
	glassDef.position = {0, 0};
	_bodyGlass = MainGameScene::getInstance()->getWorld()->CreateBody(&glassDef);

	b2PolygonShape shapeLeft;
	shapeLeft.SetAsBox(0.1f, 12, { 0, 12 }, 0);

	b2PolygonShape shapeBottom;
	shapeBottom.SetAsBox(16, 0.1f, { 16, -2 }, 0);

	b2FixtureDef fixtureDefLeft;
	fixtureDefLeft.density = 1;
	fixtureDefLeft.friction = 0;
	fixtureDefLeft.restitution = 0;
	fixtureDefLeft.shape = &shapeLeft;
	_bodyGlass->CreateFixture(&fixtureDefLeft);

	b2FixtureDef fixtureDefBottom;
	fixtureDefBottom.density = 1;
	fixtureDefBottom.friction = 0;
	fixtureDefBottom.restitution = 0;
	fixtureDefBottom.filter.categoryBits = bottomCategoryBits;
	fixtureDefBottom.shape = &shapeBottom;
	_bodyGlass->CreateFixture(&fixtureDefBottom);

	b2PolygonShape shapeRight;
	shapeRight.SetAsBox(0.1f, 12, { 32, 12 }, 0);

	b2FixtureDef fixtureDefRight;
	fixtureDefRight.density = 1;
	fixtureDefRight.friction = 0;
	fixtureDefRight.restitution = 0;
	fixtureDefRight.shape = &shapeRight;
	_bodyGlass->CreateFixture(&fixtureDefRight);
}

void BackgroundElementMainGame::show() {
	_layerParent->addChild(_layerBack);
}

void BackgroundElementMainGame::disable(bool flag) {
	_isDisabled = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}