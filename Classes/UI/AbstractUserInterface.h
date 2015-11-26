#ifndef __ABSTRACT_USER_INTERFACE_H__
#define __ABSTRACT_USER_INTERFACE_H__

#include <string>
#include "UI\AbstractElementUI.h"

class AbstractUserInterface {
public:
	virtual void show() = 0;
	virtual AbstractElementUI* getChildrenByName(const std::string) = 0;
};
#endif