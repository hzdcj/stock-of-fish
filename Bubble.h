#ifndef _Bubble_H_
#define _Bubble_H_
#include"cocos2d.h"
USING_NS_CC;

using namespace std;
class Bubble :public Node
{
public:
	static Bubble* create(const string&, float second);
	void down();  //气泡下落
	Vec2 randomGenerate();  //随机生成气泡
	CREATE_FUNC(Bubble);
	void bubbleInit(const string&, float second);
	Sprite* bubbleImage;
	float second;
};
#endif