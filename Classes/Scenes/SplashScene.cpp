#include "SplashScene.h"
#include "Common\MessagesQueue.h"

const std::string SplashScene::pathToBack = "background.png";

Scene* SplashScene::createScene() {
	auto scene = Scene::create();
	auto layer = SplashScene::create();
	scene->addChild(layer);
	return scene;
}

void SplashScene::onEnter() {
	Node::onEnter();
	MessagesQueue::getInstance()->addMessageToQueue({ MessagesQueue::MessageType::INIT_SCENES, nullptr });
}

bool SplashScene::init() {
	if (!Layer::init())
		return false;

	Size size = getContentSize();
	auto background = Sprite::create(pathToBack);
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	background->setScaleX(size.width / background->getContentSize().width);
	background->setScaleY(size.height / background->getContentSize().height);
	addChild(background, -1);

	auto label = Label::createWithSystemFont("LOADING. PLEASE WAIT.", "Fixedsys", 45);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	label->setPosition({ size.width / 2, size.height / 2 });
	label->enableShadow();
	addChild(label, 0);

	return true;
}