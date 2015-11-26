#include "PlayerInput.h"
#include "Common\MessagesQueue.h"

PlayerInput::~PlayerInput() {
	Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
}

PlayerInput::PlayerInput(Player *player) {
	_player = player;
	_currentPressedKey = EventKeyboard::KeyCode::KEY_0;

	_listener = EventListenerKeyboard::create();
	_listener->onKeyPressed = CC_CALLBACK_2(PlayerInput::onKeyPressed, this);
	_listener->onKeyReleased = CC_CALLBACK_2(PlayerInput::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_listener, 1);
}

void PlayerInput::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
	CCASSERT(_player, "_player pointer is null");
	MessagesQueue::Message msg = { MessagesQueue::MessageType::PLAYER_JUMP, nullptr };
	b2Body *body = nullptr;

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_SPACE:
		if (_player->isJumped())
			return;
		_currentPressedKey = keyCode;
		MessagesQueue::getInstance()->addMessageToQueue(msg);
		_player->isJumped(true);
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (!_player->isJumped())
			return;
		body = _player->getPhysBody();
		body->SetLinearVelocity({ -10, body->GetLinearVelocity().y });
		_currentPressedKey = keyCode;
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (!_player->isJumped())
			return;
		body = _player->getPhysBody();
		body->SetLinearVelocity({ 10, body->GetLinearVelocity().y });
		_currentPressedKey = keyCode;
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		_currentPressedKey = keyCode;
		break;
	}
}

void PlayerInput::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event) {
	CCASSERT(_player, "_player pointer is null");
	
	b2Body *body = _player->getPhysBody();
	body->SetLinearVelocity({ 0, body->GetLinearVelocity().y });
	_currentPressedKey = EventKeyboard::KeyCode::KEY_0;
}