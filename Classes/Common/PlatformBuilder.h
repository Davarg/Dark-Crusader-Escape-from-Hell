#ifndef __PLATFORM_BUILDER_H__
#define __PLATFORM_BUILDER_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class PlatformBuilder {
private:
	static const std::string notDestroyableTexture;
	static const std::string destroyableTexture;
	static const std::string notDestroyableC3B;
	static const std::string destroyableC3B;

public:
	enum PlatformCategoryBits {
		DESTROYABLE_CATEGORY_BITS = 0x0016,
		NOT_DESTROYABLE_CATEGORY_BITS
	};

	class Platform {
	protected:
		Layer *_layerBack = nullptr;

	public:
		virtual Layer* getBackLayer() = 0;
		virtual Sprite3D* getSprite() = 0;
		virtual void setPosition(Vec3) = 0;
		virtual bool isPlatformDestroyable() = 0;
		virtual void show(bool flag) { _layerBack->setVisible(flag); };
	};

	class PlatformDestroyable : public Platform {
	private:
		Sprite3D *_sprite = nullptr;
		b2Body *_physBody = nullptr;
		bool _isDestroyable = true;
		static const Vec2 offset;
		static const int scores;

	public:
		PlatformDestroyable();
		virtual void setPosition(Vec3) override;
		static const int getScores() { return scores; }
		static const Vec2 getOffsetPxl() { return offset; }
		virtual Sprite3D* getSprite() override { return _sprite; }
		virtual Layer* getBackLayer() override { return _layerBack; }
		virtual bool isPlatformDestroyable() override { return _isDestroyable; }
	};

	class PlatformNotDestroyable : public Platform {
	private:
		Sprite3D *_sprite = nullptr;
		b2Body *_physBody = nullptr;
		bool _isDestroyable = false;
		static const Vec2 offset;
		static const int scores;

	public:
		PlatformNotDestroyable();
		virtual void setPosition(Vec3) override;
		static const int getScores() { return scores; }
		static const Vec2 getOffsetPxl() { return offset; }
		virtual Sprite3D* getSprite() override { return _sprite; }
		virtual Layer* getBackLayer() override { return _layerBack; }
		virtual bool isPlatformDestroyable() override { return _isDestroyable; }
	};

	static std::vector<Platform*> generateRandomPlatforms(int);
};

#endif