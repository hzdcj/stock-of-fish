#include"Fish.h"
#include"FishLayer.h"
#pragma once
#define PI 3.1415926
bool Shark::init()
{
	zSprite = Sprite::createWithSpriteFrameName("fish010_01.png");
	zSprite->setFlipX(true);
	this->addChild(zSprite);	
	auto animation = Animation::create();
	for (int i = 1; i <= 10; i++)
	{
		if (i == 10)
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish010_10.png")));
		}
		else
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish010_0%d.png", i)));
		}
	}
	animation->setDelayPerUnit(3.0f / 30.0f);
	AnimationCache::getInstance()->addAnimation(animation, "move010");
	auto move = AnimationCache::getInstance()->getAnimation("move010");
	auto animate = Animate::create(move);
	auto repeatAnimate = RepeatForever::create(animate);
	zSprite->runAction(repeatAnimate);

	return true;
}
bool Fish::init()
{
	int a[3] = { 1, 2, 5 };
	int num = FishLayer::level;
	fishSprite = Sprite::createWithSpriteFrameName(StringUtils::format("fish0%d_01.png", a[FishLayer::level]));
	if (CCRANDOM_0_1() < 0.5)
		fishSprite->setFlipX(true);
	this->addChild(fishSprite);

	auto animation = Animation::create();
	for (int i = 1; i <= 10; i++)
	{
		if (i == 10)
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish0%d_10.png", a[FishLayer::level])));
		}
		else
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish0%d_0%d.png", a[FishLayer::level], i)));
		}
	}
	animation->setDelayPerUnit(3.0f / 30.0f);
	AnimationCache::getInstance()->addAnimation(animation, StringUtils::format("move0%d", a[FishLayer::level]));
	auto move = AnimationCache::getInstance()->getAnimation(StringUtils::format("move0%d", a[FishLayer::level]));
	auto animate = Animate::create(move);
	auto repeatAnimate = RepeatForever::create(animate);
	fishSprite->runAction(repeatAnimate);
	
	this->scheduleUpdate();
	return true;
}
void Fish::move()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (CCRANDOM_0_1() < 0.002)
	{
		this->fishSprite->setFlipX(!fishSprite->isFlippedX());
	}
	Vec2 point = this->getPosition();
	f[0] = -1;
	f[1] = 0;
	f[2] = 1;
	if (CCRANDOM_0_1()>0.99)
	{	
		int a = round(CCRANDOM_0_1() * 2);
		if (f[a] != ff)
			ff = f[a];
	}
	if (!fishSprite->isFlippedX())
	{
		this->setPosition(this->getPosition().x - 1, this->getPosition().y + ff);
	}
	else
	{		
		this->setPosition(this->getPosition().x + 1, this->getPosition().y + ff);
	}
}
void Fish::reLocation(Vec2 point)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (point.y<10 || point.y>visibleSize.height - 10)
	{
		if (this->fishSprite->isFlippedX())
		{
			this->setPosition(point.x + CCRANDOM_0_1() * 2, point.y);
		}
		else
		{
			this->setPosition(point.x - CCRANDOM_0_1() * 2, point.y);
		}	
	}
	else if (point.x < 0 && !fishSprite->isFlippedX())
	{
		this->fishSprite->setFlipX(false);
		this->setPosition(2*visibleSize.width+10, point.y + CCRANDOM_0_1() * 2);
	}
	else if (point.x>visibleSize.width * 2 && fishSprite->isFlippedX())
	{
		this->fishSprite->setFlipX(true);
		this->setPosition(-10, point.y + CCRANDOM_0_1() * 2);
	}
}
bool LightFish::init()
{
	lightFishImage = Sprite::createWithSpriteFrameName("fish09_01.png");
    lightFishImage->setFlipX(true);
	this->addChild(lightFishImage);
	auto animation = Animation::create();
	for (int i = 1; i <= 10; i++)
	{
		if (i == 10)
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish09_10.png")));
		}
		else
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish09_0%d.png", i)));
		}
	}
	animation->setDelayPerUnit(3.0f / 30.0f);
	AnimationCache::getInstance()->addAnimation(animation, "move09");
	auto move = AnimationCache::getInstance()->getAnimation("move09");
	auto animate = Animate::create(move);
	auto repeatAnimate = RepeatForever::create(animate);
	lightFishImage->runAction(repeatAnimate);
	
	sun = ParticleSun::create();
	lightFishImage->addChild(sun);
	sun->setDuration(ParticleSystem::DURATION_INFINITY);
	sun->setStartColor(Color4F(1, 1, 1, 0.1));
	sun->setEndColor(Color4F(1, 1, 1, 0.1));
	sun->setEmissionRate(100);
	if (lightFishImage->isFlippedX())
	{
		sun->setPosition(Vec2(160, 75));
	}
	else
	{
		sun->setPosition(Vec2(20, 75));
	}

	return true;
}
void LightFish::randMove()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 point = this->getPosition();
	if (point.y<10 || point.y>visibleSize.height - 10)
	{
		if (this->lightFishImage->isFlippedX())
		{
			this->setPosition(point.x + CCRANDOM_0_1() * 3, point.y);
		}
		else
		{
			this->setPosition(point.x - CCRANDOM_0_1() * 3, point.y);
		}
		return;
	}
	if (point.x < 0 && !lightFishImage->isFlippedX())
	{
		this->lightFishImage->setFlipX(false);
		this->setPosition(2 * visibleSize.width + 10, point.y + CCRANDOM_0_1() * 3);
		return;
	}
	else if (point.x>visibleSize.width * 2 && lightFishImage->isFlippedX())
	{
		this->lightFishImage->setFlipX(true);
		this->setPosition(-10, point.y + CCRANDOM_0_1() * 3);
		return;
	}
		if (CCRANDOM_0_1() < 0.01)
		{
			this->lightFishImage->setFlipX(!lightFishImage->isFlippedX());
		}
		f[0] = -1;
		f[1] = 0;
		f[2] = 1;
		if (CCRANDOM_0_1()>0.99)
		{
			int a = round(CCRANDOM_0_1() * 2);
			if (f[a] != ff)
				ff = f[a];
		}
		if (!lightFishImage->isFlippedX())
		{
			this->setPosition(this->getPosition().x - 2, this->getPosition().y + ff);
		}
		else
		{
			this->setPosition(this->getPosition().x + 2, this->getPosition().y + ff);
		}

}
