#include "MainMenuScene.h"
#include "MainGameScene.h"
#include "ScenesManager.h"

const std::string MainMenuScene::pathToBackgroundImg = "background.png";

Scene* MainMenuScene::createScene() {
	auto scene = Scene::create();
	auto layer = MainMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenuScene::init() {
	if (!Layer::init())
		return false;

	const float paddingMenuItem = 20;
	Size size = getContentSize();
	
	auto labelPlay = Label::createWithSystemFont("PLAY", "Fixedsys", 45);
	labelPlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelPlay->enableShadow();
	auto labelExit = Label::createWithSystemFont("EXIT", "Fixedsys", 45);
	labelExit->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelExit->enableShadow();
	auto labelOptions = Label::createWithSystemFont("OPTIONS", "Fixedsys", 45);
	labelOptions->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelOptions->enableShadow();
	auto labelScores = Label::createWithSystemFont("SCORES", "Fixedsys", 45);
	labelScores->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelScores->enableShadow();
	
	auto menuItemPlay = MenuItemLabel::create(labelPlay
		, CC_CALLBACK_1(ScenesManager::buttonPlayCallback, ScenesManager::getInstance()));
	auto menuItemExit = MenuItemLabel::create(labelExit
		, CC_CALLBACK_1(ScenesManager::buttonExitCallback, ScenesManager::getInstance()));
	auto menuItemOptions = MenuItemLabel::create(labelOptions
		, CC_CALLBACK_1(ScenesManager::buttonOptionsCallback, ScenesManager::getInstance()));
	auto menuItemScores = MenuItemLabel::create(labelScores
		, CC_CALLBACK_1(ScenesManager::buttonScoresCallback, ScenesManager::getInstance()));

	_mainMenu = Menu::createWithItem(menuItemPlay);
	_mainMenu->addChild(menuItemScores);
	_mainMenu->addChild(menuItemOptions);
	_mainMenu->addChild(menuItemExit);
	_mainMenu->setPosition({ size.width / 2, size.height / 2 });
	_mainMenu->alignItemsVerticallyWithPadding(paddingMenuItem);

	auto background = Sprite::create(pathToBackgroundImg);
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setScaleX(size.width / background->getContentSize().width);
	background->setScaleY(size.height / background->getContentSize().height);
	
	addChild(_mainMenu, 0);
	addChild(background, -1);

	return true;
}