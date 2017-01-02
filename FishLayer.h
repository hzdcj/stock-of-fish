#ifndef _Fish_Layer_H_
#define _Fish_Layer_H_
#include"cocos2d.h"
#include"Bubble.h"
#include"Fish.h"
#include"GameLayer.h"
#include"ui/CocosGUI.h"
#include"SimpleAudioEngine.h"
#include"socket/SocketServer.h"
#include"network/SocketIO.h"
using namespace cocos2d::network;
using namespace CocosDenshion;
USING_NS_CC;
using namespace ui;
class Skill;
class SceneManger;
class HRocker;
struct SharkData
{
	Vec2 position;
	float rotation;
	float scale;
};
struct BubbleData
{
	Vec2 position;
	float scale;
	float second;
};
struct LightFishData
{
	Vec2 position;
	float rotation;
	float scale;
	float sunScale;
	bool lightFishCall;
	float bloodPercent;
};
struct FishData
{
	Vec2 position;
    bool Flipx;
};
struct QipaoData
{
	Vec2 position;

};
struct GameData_send
{
	SharkData sharkData;
	BubbleData bubbleData;
	FishData fishData[30];
	QipaoData qipaoData;
	float bloodPercent;
	int dataSize;
	int erase;
	int _timeLost;
};
struct GameData_recv
{
	LightFishData lightFishData;
};

class FishLayer :public GameLayer
{
  public:
	CREATE_FUNC(FishLayer);
	virtual bool init();
	void update(float delata);
    HRocker *rocker;
	Shark *shark;
	Label *label1;
	Label *label2;
	Label *label3;
	Label *label4;
	Label *label5;
	Label *label6;
	Map<int,Fish*>fish;
	void setViewPointCenter(float, Vec2,Vec2);
	Sprite *_background1;
	Sprite *_background2;
	void sharkMove();
	Sprite *sharkHead;
	Sprite* sharkBody;
	Sprite* lightFishBody;
    Rect getBoundingBox(Sprite *sprite)const;
	Sprite *cutShark1(Sprite*);
	Sprite* cutShark2(Sprite*);
	Sprite* cutLightFish(Sprite*);
	vector<int>lock;
	void updateTimesPerSecond(float delta);
	void onNewConnect(HSocket socket);
	Menu *menu;
	Vector<Bubble*>bubble;
	ProgressTimer* progTime;
	int _timeLost;
	static int level;
	void timeLost(float);
	LoadingBar *blood;
	LoadingBar *blood_2;
	Size visibleSize;
	int testDirection(Fish*);
	void win();
	void lose();
	void restart(Ref*);
	void nextLevel(Ref*);
	void moveNode(float);
	Sprite *qipao;
	Skill *skill;
	ClippingNode *clipNode;
	LightFish *lightFish;
	static int T;
	void waterWeedsInit();
	static float speed;
	static float rotationSpeed;
    int eraseFish;
	bool lightFishCall;
	int _countdown = 3;
	bool _connected;


	SocketServer* _server;
	void initServer();
	void sendData(float);
	void onRecv(HSocket socket, const char* data, int count);
	void onDisconnect(HSocket socket);
	void sendBubbleData(float second, Vec2 position, float scale);

	SIOClient *_sioClient;
	virtual void onConnect(cocos2d::network::SIOClient* client);
	virtual void onMessage(cocos2d::network::SIOClient* client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient* client);
	virtual void onError(cocos2d::network::SIOClient* client, const std::string& data);
	void farDisconnect(cocos2d::network::SIOClient *client, const std::string& data);
	void callConnect(cocos2d::network::SIOClient *client, const std::string& data);
};
#endif


