#ifndef __MESSAGES_QUEUE_H__
#define __MESSAGES_QUEUE_H__

#include <cocos2d.h>

USING_NS_CC;

class MessagesQueue {
public:
	typedef std::function<void(void*)> messageQueueCallback_1;

	class WrapperMessageQueueCallback_1 {
	private:
		std::string _UniqId;
		messageQueueCallback_1 _callback;
	public:
		WrapperMessageQueueCallback_1(messageQueueCallback_1 callback, std::string uniqId) {
			_callback = callback;
			_UniqId = uniqId;
		}
		messageQueueCallback_1 getCallback() { return _callback; }
		std::string getUniqId() { return _UniqId; }
	};

	enum MessageType {
		PLAYER_JUMP,
		INIT_SCENES,
		PLAYER_DIE,
		RETURN_TO_MAIN_MENU,
		UPDATE_SCORES,
		DELETE_PLATFORM,
		CREATE_SCORES_TABLE,
		RELEASE_SCENE
	};

	struct Message {
		MessageType mt;
		void* args;
	};

private:
	static MessagesQueue *_messagesQueue;
	std::vector<MessagesQueue::Message> msgQueue;
	std::map<MessagesQueue::MessageType, std::vector<MessagesQueue::WrapperMessageQueueCallback_1> > mapListeners;

public:
	~MessagesQueue();

	void update(float dt);
	void addMessageToQueue(Message);
	static MessagesQueue* getInstance() { return _messagesQueue; }
	void addListener(MessageType, WrapperMessageQueueCallback_1&);
	void removeListener(MessageType, WrapperMessageQueueCallback_1&);
};

#endif