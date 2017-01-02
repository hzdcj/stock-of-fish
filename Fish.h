#ifndef _Fish_H_
#define _Fish_H_
#include"cocos2d.h"
USING_NS_CC;
#include"HRocker.h"

typedef enum
{
	stay = 0,
	left=1,
	right=2
}Direction;
class Fish :public Sprite
{
public: 
	virtual bool init();
	CREATE_FUNC(Fish);
	Sprite *fishSprite;
	virtual void move();
	int ff=0;
	void reLocation(Vec2);
private:
	int speed;	
	Direction direction;
	int f[3];
};
class Shark :public Fish
{
public:
	void switchMovedState(int dir);
	virtual bool init();
	CREATE_FUNC(Shark);
public:
	Sprite *zSprite;
	
};
class LightFish:public Sprite
{
public:
	virtual bool init();
	CREATE_FUNC(LightFish);
	Sprite* lightFishImage;
	void randMove();
	int f[3];
	int ff = 0;
	ParticleSun *sun;
};
#endif
