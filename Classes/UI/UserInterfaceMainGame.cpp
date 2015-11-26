#include "UserInterfaceMainGame.h"
#include "ScoresElementMainGame.h"
#include "BackgroundElementMainGame.h"

UserInterfaceMainGame::~UserInterfaceMainGame() {
	for (auto iterator : _childrens) {
		CC_SAFE_DELETE(iterator);
	}
	_childrens.clear();
}

UserInterfaceMainGame::UserInterfaceMainGame(Layer *layer, Size size) {
	_layerParent = layer;
	BackgroundElementMainGame *bemg = new BackgroundElementMainGame(layer, size);
	_childrens.push_back(bemg);

	ScoresElementMainGame *semg = new ScoresElementMainGame(layer, size);
	_childrens.push_back(semg);
}

void UserInterfaceMainGame::show() {
	for (auto iterator : _childrens)
		iterator->show();
}

AbstractElementUI* UserInterfaceMainGame::getChildrenByName(const std::string name) {
	for (auto iterator : _childrens) {
		if (iterator->getName() == name)
			return iterator;
	}
	return nullptr;
}