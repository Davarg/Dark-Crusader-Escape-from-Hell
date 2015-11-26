#include "Box2DContactFilter.h"
#include "Common\Player\Player.h"
#include "Common\PlatformBuilder.h"
#include "Common\ConstantsRegistry.h"
#include "UI\BackgroundElementMainGame.h"

bool Box2DContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) {
	bool resA = false;
	bool resB = false;
	std::set<uint16> set;

	const uint16 fixtureACategoryBits = fixtureA->GetFilterData().categoryBits;
	const uint16 fixtureBCategoryBits = fixtureB->GetFilterData().categoryBits;

	set.insert(PlatformBuilder::PlatformCategoryBits::DESTROYABLE_CATEGORY_BITS);
	set.insert(PlatformBuilder::PlatformCategoryBits::NOT_DESTROYABLE_CATEGORY_BITS);
	if (set.find(fixtureACategoryBits) != set.end()) {
		if (set.find(fixtureBCategoryBits) != set.end())
			return false;
	}
	else if (set.find(fixtureBCategoryBits) != set.end()) {
		if (set.find(fixtureACategoryBits) != set.end())
			return false;
	}
	return true;
}