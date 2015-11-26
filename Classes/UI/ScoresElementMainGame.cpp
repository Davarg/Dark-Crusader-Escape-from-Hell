#include "ScoresElementMainGame.h"

const std::string ScoresElementMainGame::backgroundImgPath = "gray_back.png";
const std::string ScoresElementMainGame::elementName = "ScoresElementMainGame";

ScoresElementMainGame::~ScoresElementMainGame() {
	MessagesQueue::getInstance()->removeListener(MessagesQueue::MessageType::UPDATE_SCORES, *_updateScoresCallback);
	CC_SAFE_DELETE(_updateScoresCallback);
}

ScoresElementMainGame::ScoresElementMainGame(Layer *layer, Size size) {
	const int fraction = 16;
	_scores = 0;
	_layerParent = layer;
	_layerBack = Layer::create();
	_layerBack->setPosition({ 0, (size.height / fraction) * (fraction - 1) });
	_layerBack->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

	auto spriteBackground = Sprite::create(backgroundImgPath);
	spriteBackground->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	spriteBackground->setScaleX(size.width / spriteBackground->getContentSize().width);
	spriteBackground->setScaleY((size.height / spriteBackground->getContentSize().height) / fraction);
	spriteBackground->setOpacity(50);
	_layerBack->addChild(spriteBackground, -1);

	_labelScores = Label::createWithSystemFont("Scores: " + std::to_string(_scores), "Fixedsys", 25);
	_labelScores->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_labelScores->setPosition({ size.width / 2, 25 });
	_layerBack->addChild(_labelScores, 0);

	_updateScoresCallback = new MessagesQueue::WrapperMessageQueueCallback_1(CC_CALLBACK_1(ScoresElementMainGame::updateScoresCallback, this)
		, "ScoresElementUpdateScores");
	MessagesQueue::getInstance()->addListener(MessagesQueue::MessageType::UPDATE_SCORES, *_updateScoresCallback);
}

void ScoresElementMainGame::updateScoresCallback(void *args) {
	_scores += *(static_cast<int*>(args));
	_labelScores->setString("Scores: " + std::to_string(_scores));

	CC_SAFE_DELETE(args);
}

void ScoresElementMainGame::show() {
	_layerParent->addChild(_layerBack);
}

void ScoresElementMainGame::disable(bool flag) {
	_isDisabled = flag;
	if (!flag)
		_layerParent->removeChild(_layerBack);
	else
		_layerParent->addChild(_layerBack);
}