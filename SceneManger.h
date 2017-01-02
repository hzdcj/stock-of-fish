#ifndef _Scene_Manger_H_
#define _Scene_Manger_H_
#include"cocos2d.h"
USING_NS_CC;
using namespace std;
class SceneManger :public Scene
{
public:
	void createLoadScene();
	void goOpenScene();
	void goGameScene();
	
	static string WStrToUTF8(const wstring& src);
public:
	Scene *loadScene;
	Scene *openScene;
	Scene *gameScene;
	static SceneManger* p;
};
#endif