#include "Player.h"
#include "Scenes\MainGameScene.h"
#include "Common\ConstantsRegistry.h"

const std::string Player::playerC3B = "knight.c3b";
const std::string Player::playerTexture = "knight.png";

Player::Player() {
	_isJumped = false;

	_sprite = Sprite3D::create(playerC3B, playerTexture);
	_sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_sprite->setScale(ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SPRITE_SCALE));
	_sprite->setRotation3D({ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_X_3D)
		, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_Y_3D)
		, ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::ROTATION_Z_3D) });
#ifdef _DEBUG
	_sprite->setOpacity(50);
#endif

	b2BodyDef playerDef;
	playerDef.type = b2_dynamicBody;
	playerDef.position = { _sprite->getPositionX()
			/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
		, _sprite->getPositionY()
			/ ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D) };
	playerDef.userData = _sprite;
	_physBody = MainGameScene::getInstance()->getWorld()->CreateBody(&playerDef);

	Size size = _sprite->getContentSize();
	b2PolygonShape shapePlayer;
	shapePlayer.SetAsBox(0.8f, 1.1f);

	b2FixtureDef fixtureDefPlayer;
	fixtureDefPlayer.density = 1;
	fixtureDefPlayer.friction = 0;
	fixtureDefPlayer.restitution = 0;
	fixtureDefPlayer.filter.categoryBits = PlayerCategoryBits::PLAYER_CATEGORY_BITS;
	fixtureDefPlayer.shape = &shapePlayer;
	_physBody->CreateFixture(&fixtureDefPlayer);
	b2MassData mass;
	_physBody->GetMassData(&mass);
	mass.mass = 0.001;
	_physBody->SetMassData(&mass);
}

void Player::setPositionPxl(Vec3 pos) {
	_sprite->setPosition3D(pos);
	_physBody->SetTransform({ pos.x / ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D)
		, pos.y / ConstantsRegistry::getValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D) }, 0);
}