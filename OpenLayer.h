#ifndef _Open_Layer_H_
#define _Open_Layer_H_
#include"cocos2d.h"
#include"network/SocketIO.h"
USING_NS_CC;
using namespace cocos2d::network;
using namespace CocosDenshion;
class SceneManger;
using namespace std;
class OpenLayer :public Layer,public SocketIO::SIODelegate
{
public:
	CREATE_FUNC(OpenLayer);
	virtual bool init();
	void menuCallBack(Ref *psender);
	void updateTimesPerSecond(float);
	void httpRequest(const char*param, int action);

	string getLocalAddress();
public:
	SceneManger *tsm;
	static string farIpAddress;
	static string localIpAdress;
private:
	Menu *menu;
	int action;
	Label *label4;
	Label *label2;

	virtual void onConnect(cocos2d::network::SIOClient* client);
	virtual void onMessage(cocos2d::network::SIOClient* client, const std::string& data);
	virtual void onClose(cocos2d::network::SIOClient* client);
	virtual void onError(cocos2d::network::SIOClient* client, const std::string& data);

	void createCallBack(cocos2d::network::SIOClient *client, const std::string& data);
	void callConnect(cocos2d::network::SIOClient *client, const std::string& data);
	SIOClient *_sioClient;

};
#endif