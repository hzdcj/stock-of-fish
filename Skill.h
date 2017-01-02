#ifndef _Skill_H_
#define _Skill_H_
#include"cocos2d.h"
USING_NS_CC;
using namespace std;
class FishLayer;
class Skill
{
public:
	MenuItemImage* create(const string&, int, Sprite*,FishLayer*);
	MenuItemImage* button_1;
	MenuItemImage* button_2;
	MenuItemImage* button_3;
	ProgressTimer* progress_1;
	ProgressTimer* progress_2;
	ProgressTimer* progress_3;
private:
	LoadingBar *blood;
	void accelerate(Sprite*, FishLayer*);
	void lash(Sprite*, FishLayer*);
	void addBlood(Sprite*, FishLayer*);
};
#endif