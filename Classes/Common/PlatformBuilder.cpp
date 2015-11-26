#include "PlatformBuilder.h"
#include "Scenes\MainGameScene.h"
#include "Common\ConstantsRegistry.h"

const int PlatformBuilder::PlatformDestroyable::scores = 6;
const int PlatformBuilder::PlatformNotDestroyable::scores = 3;
const Vec2 PlatformBuilder::PlatformDestroyable::offset = { 18, 6 };
const Vec2 PlatformBuilder::PlatformNotDestroyable::offset = { 2, 8 };
const std::string PlatformBuilder::destroyableC3B = "destroyable.c3b";
const std::string PlatformBuilder::destroyableTexture = "destroyable.png";
const std::string PlatformBuilder::notDestroyableC3B = "notDestroyable.c3b";
const std::string PlatformBuilder::notDestroyableTexture = "notDestroyable.png";

std::vector<PlatformBuilder::Platform*> PlatformBuilder::generateRandomPlatforms(int count) {
	std::vector<PlatformBuilder::Platform*> result;
	while (count) {
		Platform *platform = nullptr;
		switch ((int)rand_0_1()) {
		case 0:
			platform = new PlatformNotDestroyable();
			break;

		case 1:
			platform = new PlatformDestroyable();
			break;
		}

		CCASSERT(platform, "platform can't be null");
		result.push_back(platform);
		--count;
	}

	return result;
}

PlatformBuilder::PlatformDestroyable::PlatformDestroyable() {
	_layerBack = Layer::create();
	_sprite = Sprite3D::create(destroyableC3B, destroyableTexture);
	_sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_sprite->setScale(ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SPRITE_SCALE));
	_sprite->setRotation3D({ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_X_3D)
		, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_Y_3D)
		, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_Z_3D) });
#ifdef _DEBUG
	_sprite->setOpacity(50);
#endif
	_layerBack->addChild(_sprite);

	b2BodyDef platformDef;
	platformDef.type = b2_dynamicBody;
	platformDef.position = { _sprite->getPositionX()
		/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
		, _sprite->getPositionY()
		/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D) };
	platformDef.userData = _sprite;
	platformDef.linearDamping = 1;
	_physBody = MainGameScene::getInstance()->getWorld()->CreateBody(&platformDef);

	b2PolygonShape shapePlatform;
	shapePlatform.SetAsBox(1.1f, 0.4f);

	b2FixtureDef fixtureDefPlatform;
	fixtureDefPlatform.density = 1;
	fixtureDefPlatform.friction = 0;
	fixtureDefPlatform.restitution = 0.1f;
	fixtureDefPlatform.filter.categoryBits = PlatformCategoryBits::DESTROYABLE_CATEGORY_BITS;
	fixtureDefPlatform.shape = &shapePlatform;
	_physBody->CreateFixture(&fixtureDefPlatform);
}

PlatformBuilder::PlatformNotDestroyable::PlatformNotDestroyable() {
	_layerBack = Layer::create();
	_sprite = Sprite3D::create(notDestroyableC3B, notDestroyableTexture);
	_sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_sprite->setScale(ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SPRITE_SCALE));
	_sprite->setRotation3D({ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_X_3D)
		, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_Y_3D)
		, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_Z_3D) });
#ifdef _DEBUG
	_sprite->setOpacity(50);
#endif
	_layerBack->addChild(_sprite);

	b2BodyDef platformDef;
	platformDef.type = b2_dynamicBody;
	platformDef.position = { _sprite->getPositionX()
			/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
		, _sprite->getPositionY()
			/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D) };
	platformDef.userData = _sprite;
	platformDef.linearDamping = 1;
	_physBody = MainGameScene::getInstance()->getWorld()->CreateBody(&platformDef);

	b2PolygonShape shapePlatform;
	shapePlatform.SetAsBox(1, 0.4f);

	b2FixtureDef fixtureDefPlatform;
	fixtureDefPlatform.density = 1;
	fixtureDefPlatform.friction = 0;
	fixtureDefPlatform.restitution = 0.1f;
	fixtureDefPlatform.filter.categoryBits = PlatformCategoryBits::NOT_DESTROYABLE_CATEGORY_BITS;
	fixtureDefPlatform.shape = &shapePlatform;
	_physBody->CreateFixture(&fixtureDefPlatform);
}

void PlatformBuilder::PlatformNotDestroyable::setPosition(Vec3 pos) {
	_sprite->setPosition3D(pos);
	_physBody->SetTransform({ (pos.x + offset.x) / ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
		, (pos.y + offset.y) / ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D) }
	, 0);
}

void PlatformBuilder::PlatformDestroyable::setPosition(Vec3 pos) {
	_sprite->setPosition3D(pos);
	_physBody->SetTransform({ (pos.x - offset.x) / ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
		, (pos.y + offset.y) / ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D) }
	, 0);
}