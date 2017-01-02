#include"LoadLayer.h"
#include"OpenLayer.h"
#include"GameLayer.h"
#include"FishLayer.h"
#include"SceneManger.h"
SceneManger* SceneManger::p = NULL;
void SceneManger::createLoadScene()
{
	loadScene = Scene::create();
	LoadLayer *layer = LoadLayer::create();
	layer->tsm = this;
	loadScene->addChild(layer);
}
void SceneManger::goOpenScene()
{
	openScene = Scene::create();
	OpenLayer *layer = OpenLayer::create();
	layer->tsm = this;
	openScene->addChild(layer);
	Director::getInstance()->replaceScene(openScene);
	
}
void SceneManger::goGameScene()
{
	gameScene = Scene::createWithPhysics();
	gameScene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	GameLayer *layer = GameLayer::create();
	layer->tsm = this;
	gameScene->addChild(layer);
	Director::getInstance()->replaceScene(gameScene);
}



string SceneManger::WStrToUTF8(const wstring& src)
{
	std::string dest;
	dest.clear();
	for (size_t i = 0; i < src.size(); i++)
	{
		wchar_t w = src[i];
		if (w <= 0x7f)
		{
			dest.push_back((char)w);
		}
		else if(w <= 0x7ff)
		{
			dest.push_back(0xc0 | ((w >> 6) & 0x1f));
			dest.push_back(0x80 | (w & 0x3f));
		}
		else if(w <= 0xffff)
		{
			dest.push_back(0xe0 | ((w >> 12) & 0x0f));
			dest.push_back(0x80 | ((w >> 6) & 0x3f));
			dest.push_back(0x80 | (w & 0x3f));
		}
		else if(sizeof(wchar_t) > 2 && w <= 0x10ffff)
		{
			dest.push_back(0xf0 | ((w >> 18) & 0x07));
			dest.push_back(0x80 | ((w >> 12) & 0x3f));
			dest.push_back(0x80 | ((w >> 6) & 0x3f));
			dest.push_back(0x80 | (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
	return dest;
}