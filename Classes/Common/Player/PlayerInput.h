#ifndef __PLAYER_INPUT_H__
#define __PLAYER_INPUT_H__

#include "Player.h"
#include <cocos2d.h>
#include <Box2D\Box2D.h>

class PlayerInput {
private:
	Player *_player;
	EventListenerKeyboard *_listener;
	EventKeyboard::KeyCode _currentPressedKey;

public:
	PlayerInput(Player*);
	~PlayerInput();

	void onKeyPressed(EventKeyboard::KeyCode, Event*);
	void onKeyReleased(EventKeyboard::KeyCode, Event*);
};

#endif