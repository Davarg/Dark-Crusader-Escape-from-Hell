#include "SplashScene.h"
#include "ScoresScene.h"
#include "OptionsScene.h"
#include "ScenesManager.h"
#include "MainMenuScene.h"
#include "MainGameScene.h"


ScenesManager *ScenesManager::scenesManager = new ScenesManager();
const std::string ScenesManager::pathToBackgroundMscMenu = "main_menu_background.mp3";
const std::string ScenesManager::pathToBackgroundMscMainGame = "main_game_background.mp3";

ScenesManager::~ScenesManager() {
	CC_SAFE_DELETE(scenesManager);
	CC_SAFE_DELETE(_callBackPlayerDie);
}

Scene* ScenesManager::getStartScene() {
	MessagesQueue::WrapperMessageQueueCallback_1 callback(CC_CALLBACK_1(ScenesManager::releaseSceneCallback, this)
		, "ScenesManagerReleaseScene");
	MessagesQueue::getInstance()->addListener(MessagesQueue::MessageType::RELEASE_SCENE, callback);
	return SplashScene::createScene();
}

void ScenesManager::playerDiedCallback(void *args) {
	_sharedCallbackArgs = args;
	Size size = MainGameScene::getInstance()->getContentSize();
	auto label = Label::createWithSystemFont("GAME OVER :(", "Fixedsys", 45);
	auto layer = Layer::create();
	auto sprite = Sprite::create("gray_back.png");

	sprite->setScaleX(size.width / sprite->getContentSize().width);
	sprite->setScaleY(size.height / sprite->getContentSize().height);
	sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	label->setColor(Color3B::RED);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	label->setPosition({ size.width / 2, size.height / 2 });
	label->enableShadow();

	MainGameScene::getInstance()->addChild(layer);
	MessagesQueue::getInstance()->removeListener(MessagesQueue::MessageType::PLAYER_DIE, *_callBackPlayerDie);
	CC_SAFE_DELETE(_callBackPlayerDie);

	auto labelReturn = Label::createWithSystemFont("Return", "Fixedsys", 45);
	labelReturn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto menuItemReturn = MenuItemLabel::create(labelReturn
		, CC_CALLBACK_1(ScenesManager::buttonReturnGameOverCallback, this));

	auto menu = Menu::createWithItem(menuItemReturn);
	menu->setPosition(label->getPositionX(), label->getPositionY() - 60);

	layer->addChild(sprite);
	layer->addChild(label);
	layer->addChild(menu);
	//Bug, maybe in engine. When transition is not finished try to close app
	//Director::getInstance()->replaceScene(TransitionFade::create(3.5, createStartMenu(), Color3B(201, 183, 72))); 
}

void ScenesManager::buttonScoresCallback(Ref*) {
	auto scene = ScoresScene::createScene(0, false);
	Director::getInstance()->replaceScene(TransitionFade::create(1, scene, Color3B(160, 82, 45)));
	scene->retain();

	MessagesQueue::Message msg = { MessagesQueue::MessageType::RELEASE_SCENE, Director::getInstance()->getRunningScene() };
	MessagesQueue::getInstance()->addMessageToQueue(msg);
}

void ScenesManager::buttonReturnGameOverCallback(Ref*) {
	int *scores = static_cast<int*>(_sharedCallbackArgs);
	auto scene = ScoresScene::createScene(*scores);
	
	Director::getInstance()->replaceScene(TransitionFade::create(1, scene, Color3B(160, 82, 45)));
	CC_SAFE_DELETE(scores);
	scene->retain();

	MessagesQueue::Message msg = { MessagesQueue::MessageType::RELEASE_SCENE, Director::getInstance()->getRunningScene() };
	MessagesQueue::getInstance()->addMessageToQueue(msg);
}

Scene* ScenesManager::createStartMenu() {
	if (UserDefault::getInstance()->getBoolForKey("music", true)) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pathToBackgroundMscMenu.c_str(), true);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1f); //Not implemented for Win32 in engine
	}
	return MainMenuScene::createScene();
}

void ScenesManager::startMenuCallback(void*) {
	auto scene = createStartMenu();
	Director::getInstance()->replaceScene(TransitionFade::create(1.5, scene, Color3B(160, 82, 45)));
	scene->retain();

	/*MessagesQueue::Message msg = { MessagesQueue::MessageType::RELEASE_SCENE, Director::getInstance()->getRunningScene() };
	MessagesQueue::getInstance()->addMessageToQueue(msg);*/
}

void ScenesManager::buttonPlayCallback(Ref *sender) {
	_callBackPlayerDie = new MessagesQueue::WrapperMessageQueueCallback_1(CC_CALLBACK_1(ScenesManager::playerDiedCallback, this)
		, "ScenesManagerPlayerDied");
	MessagesQueue::getInstance()->addListener(MessagesQueue::MessageType::PLAYER_DIE, *_callBackPlayerDie);

	if (UserDefault::getInstance()->getBoolForKey("music", true))
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pathToBackgroundMscMainGame.c_str(), true);

	auto scene = MainGameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1, scene, Color3B(160, 82, 45)));
	scene->retain();

	MessagesQueue::Message msg = { MessagesQueue::MessageType::RELEASE_SCENE, Director::getInstance()->getRunningScene() };
	MessagesQueue::getInstance()->addMessageToQueue(msg);
}

void ScenesManager::buttonExitCallback(Ref *sender) {
	Director::getInstance()->end();
}

void ScenesManager::buttonOptionsCallback(Ref*) {
	auto scene = OptionsScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1, scene, Color3B(160, 82, 45)));
	scene->retain();

	MessagesQueue::Message msg = { MessagesQueue::MessageType::RELEASE_SCENE, Director::getInstance()->getRunningScene() };
	MessagesQueue::getInstance()->addMessageToQueue(msg);
}

void ScenesManager::buttonMusicOnCallback(Ref *ref) {
	MenuItemLabel *menuItem = (MenuItemLabel*)ref;

	Label *labelMusic = nullptr;
	bool isOn = UserDefault::getInstance()->getBoolForKey("music", true);
	if (isOn) {
		labelMusic = Label::createWithSystemFont("Music: Off", "Fixedsys", 45);
		labelMusic->setColor(Color3B::GRAY);
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("music", false);
	}
	else {
		labelMusic = Label::createWithSystemFont("Music: On", "Fixedsys", 45);
		labelMusic->setColor(Color3B::WHITE);
		if (CocosDenshion::SimpleAudioEngine::getInstance()->willPlayBackgroundMusic())
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		else
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pathToBackgroundMscMenu.c_str(), true);
		UserDefault::getInstance()->setBoolForKey("music", true);
	}
	labelMusic->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	labelMusic->enableShadow();

	menuItem->setLabel(labelMusic);
}

void ScenesManager::buttonSoundOnCallback(Ref *ref) {
}

void ScenesManager::releaseSceneCallback(void *args) {
	Scene *scene = static_cast<Scene*>(args);
	scene->release();
}

void ScenesManager::buttonReturnOptionsCallback(Ref*) {
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1, scene, Color3B(160, 82, 45)));
	scene->retain();
	
	MessagesQueue::Message msg = { MessagesQueue::MessageType::RELEASE_SCENE, Director::getInstance()->getRunningScene() };
	MessagesQueue::getInstance()->addMessageToQueue(msg);
}