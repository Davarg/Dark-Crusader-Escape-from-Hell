#ifndef __ABSTRACT_ELEMENT_UI_H__
#define __ABSTRACT_ELEMENT_UI_H__

#include <string>

class AbstractElementUI {
public:
	virtual void show() = 0;
	virtual void disable(bool) = 0;
	virtual bool isDisabled() const = 0;
	virtual const std::string getName() const = 0;
};

#endif