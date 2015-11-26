#include "OptionsScene.h"
#include "ScenesManager.h"

const std::string OptionsScene::pathToBackgroundImg = "background.png";

Scene* OptionsScene::createScene() {
	auto scene = Scene::create();
	auto layer = OptionsScene::create();
	scene->addChild(layer);
	return scene;
}

bool OptionsScene::init() {
	if (!Layer::init())
		return false;

	const float paddingMenuItem = 20;
	Size size = getContentSize();

	auto labelSound = Label::createWithSystemFont("Sound: On", "Fixedsys", 45);
	labelSound->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelSound->enableShadow();
	Label *labelMusic = nullptr;
	if (UserDefault::getInstance()->getBoolForKey("music", true))
		labelMusic = Label::createWithSystemFont("Music: On", "Fixedsys", 45);
	else {
		labelMusic = Label::createWithSystemFont("Music: Off", "Fixedsys", 45);
		labelMusic->setColor(Color3B::GRAY);
	}
	labelMusic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelMusic->enableShadow();
	auto labelReturn = Label::createWithSystemFont("Return", "Fixedsys", 45);
	labelReturn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	auto menuItemSound = MenuItemLabel::create(labelSound
		, CC_CALLBACK_1(ScenesManager::buttonSoundOnCallback, ScenesManager::getInstance()));
	auto menuItemMusic = MenuItemLabel::create(labelMusic
		, CC_CALLBACK_1(ScenesManager::buttonMusicOnCallback, ScenesManager::getInstance()));
	auto menuItemReturn = MenuItemLabel::create(labelReturn
		, CC_CALLBACK_1(ScenesManager::buttonReturnOptionsCallback, ScenesManager::getInstance()));

	_optionsMenu = Menu::createWithItem(menuItemSound);
	_optionsMenu->addChild(menuItemMusic);
	_optionsMenu->addChild(menuItemReturn);
	_optionsMenu->setPosition({ size.width / 2, size.height / 2 });
	_optionsMenu->alignItemsVerticallyWithPadding(paddingMenuItem);

	auto background = Sprite::create(pathToBackgroundImg);
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setScaleX(size.width / background->getContentSize().width);
	background->setScaleY(size.height / background->getContentSize().height);

	addChild(_optionsMenu, 0);
	addChild(background, -1);

	return true;
}