#ifndef _Game_Layer_H_
#define _Game_Layer_H_
#include"cocos2d.h"
#include"network/SocketIO.h"
using namespace cocos2d::network;
USING_NS_CC;
using namespace std;
class SceneManger;
class GameLayer :public Layer,public SocketIO::SIODelegate
{
public:
	CREATE_FUNC(GameLayer);
	virtual bool init();
	void menuCallBack_Start(Ref *pSender);
	void startGame();
	Sprite *setBackground1(const string& fileName, int n);
	Sprite *setBackground2(const string& fileName, int n);
	virtual void onConnect(cocos2d::network::SIOClient* client);
	virtual void onMessage(cocos2d::network::SIOClient* client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient* client);
	virtual void onError(cocos2d::network::SIOClient* client, const std::string& data);
	void findAllCallBack(cocos2d::network::SIOClient *client, const std::string& data);
	void farDisconnect(cocos2d::network::SIOClient *client, const std::string& data);
	void callConnect(cocos2d::network::SIOClient *client, const std::string& data);
public:
	SceneManger *tsm;
	Menu *menu;
	SIOClient *_sioClient;
	bool _connected;
	Label *label4;
	static string farIpAddress;
};
#endif