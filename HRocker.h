#ifndef _HRocker_H_
#define _HRocker_H_
#include<iostream>
#include"cocos2d.h"

USING_NS_CC;
typedef enum
{
	tag_rocker,
	tag_rockerBG
}tagForHRocker;
typedef enum
{	rocker_stay=0,
	rocker_right=1,
	rocker_up=2,
	rocker_left=3,
	rocker_down=4,
	rocker_leftUp=5,
	rocker_rightUp=6,
	rocker_leftDown=7,
	rocker_rightDown=8
}tagDirection;
class HRocker :public Layer
{
public:
	static HRocker *createHRocker(const char *rockerImageName, const char *rockerBGImageName, Vec2 position);
	void startRocker(bool _isStopOther);
	void stopRocker();
	int rockerDirection;
	bool rockerRun;
	CREATE_FUNC(HRocker);
	float getRad(Vec2 pos1, Vec2 pos2);
	float angle;
	bool isCanMove;
	Vec2 point;
	Vec2 rockerBGPosition;
private:
	void rockerInit(const char *rockerImageName, const char *rockerBGImageName, Vec2 position);
	float rockerBGR;
	bool onTouchBegan(Touch *touch, Event *unused_enent);
	void onTouchMoved(Touch *touch, Event *unused_enent);
	void onTouchEnded(Touch *touch, Event *unused_enent);
};
#endif