#include"SimpleAudioEngine.h"
#include"SceneManger.h"
#include"OpenLayer.h"
#include"CursorTextField.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <errno.h>
#include <arpa/inet.h>		// for inet_**
#include <netdb.h>			// for gethost**
#include <netinet/in.h>		// for sockaddr_in
#include <sys/types.h>		// for socket
#include <sys/socket.h>		// for socket
#include <unistd.h>
#include <stdio.h>		    // for printf
#include <stdlib.h>			// for exit
#include <string.h>			// for bzero
#include <net/if.h>
#define HSocket int
#endif 
USING_NS_CC;

string OpenLayer::farIpAddress;
string OpenLayer::localIpAdress;
string OpenLayer::socketIoIp;
bool OpenLayer::init()
{
	Size winSize = Director::getInstance()->getWinSize();
	auto label_1 = Label::createWithTTF(SceneManger::WStrToUTF8(L"新的游戏"), "fj3.ttf", 50);
	label_1->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem = MenuItemLabel::create(label_1, CC_CALLBACK_1(OpenLayer::menuCallBack, this));
	menuItem->setTag(101);
	menuItem->setPosition(Vec2(winSize.width / 2, winSize.height*0.65));

	auto label_2 = Label::createWithTTF(SceneManger::WStrToUTF8(L"退出"), "fj3.ttf", 50);
	label_2->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem_2 = MenuItemLabel::create(label_2, CC_CALLBACK_1(OpenLayer::menuCallBack, this));
	menuItem_2->setTag(102);
	menuItem_2->setPosition(Vec2(winSize.width / 2, winSize.height*0.2));
	
	auto label_3 = Label::createWithTTF(SceneManger::WStrToUTF8(L"声音"), "fj3.ttf", 50);
	label_3->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem_3 = MenuItemLabel::create(label_3, CC_CALLBACK_1(OpenLayer::menuCallBack, this));
	menuItem_3->setTag(103);
	menuItem_3->setPosition(Vec2(winSize.width / 2, winSize.height*0.5));

	auto label_4 = Label::createWithTTF(SceneManger::WStrToUTF8(L"说明"), "fj3.ttf", 50);
	label_4->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem_4 = MenuItemLabel::create(label_4, CC_CALLBACK_1(OpenLayer::menuCallBack, this));
	menuItem_4->setTag(104);
	menuItem_4->setPosition(Vec2(winSize.width / 2, winSize.height*0.35));

	auto label_5 = Label::createWithTTF(SceneManger::WStrToUTF8(L"登录"), "fj3.ttf", 50);
	label_5->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem_5 = MenuItemLabel::create(label_5, CC_CALLBACK_1(OpenLayer::menuCallBack, this));
	menuItem_5->setTag(105);
	menuItem_5->setPosition(Vec2(winSize.width -100, winSize.height-100));

	auto label_6 = Label::createWithTTF(SceneManger::WStrToUTF8(L"服务器地址"), "fj3.ttf", 30);
	label_6->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem_6 = MenuItemLabel::create(label_6, CC_CALLBACK_1(OpenLayer::menuCallBack, this));
	menuItem_6->setTag(108);
	menuItem_6->setPosition(Vec2(winSize.width - 100, 100));

	menu = Menu::create(menuItem, menuItem_2, menuItem_3, menuItem_4, menuItem_5, menuItem_6, NULL);
	menu->setPosition(Point::ZERO);
	menu->setTag(10);
	this->addChild(menu,1);

	auto _background = Sprite::create("bg01.png");
	_background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	_background->setScale(1.1);
	this->addChild(_background);
    
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm_scene1.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm_scene2.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("bubble.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("bubble2.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("shark_eat2.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("shark_rush.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm_scene1.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("shoot.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_levelup.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sfx_slot_stop.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound_button.mp3");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fish2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fish3.plist");

	auto fishSprite = Sprite::createWithSpriteFrameName("fish011_01.png");
	this->addChild(fishSprite);
	fishSprite->setPosition(winSize.width - 10, winSize.height / 6);
	auto animation = Animation::create();
	for (int i = 1; i <= 10; i++)
	{
		if (i == 10)
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish011_10.png")));
		}
		else
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish011_0%d.png", i)));
		}
	}
	animation->setDelayPerUnit(3.0f / 30.0f);
	AnimationCache::getInstance()->addAnimation(animation, "move011");
	auto move = AnimationCache::getInstance()->getAnimation("move011");
	auto animate = Animate::create(move);
	auto repeatAnimate = RepeatForever::create(animate);
	auto moveBy = MoveBy::create(10, Vec2(-winSize.width - 10,0));
	auto moveByReverse = moveBy->reverse();
	ActionInstant *flipx = FlipX::create(true);
	fishSprite->runAction(repeatAnimate);
	fishSprite->runAction(RepeatForever::create(Sequence::create(moveBy,flipx,moveByReverse,flipx->reverse(),NULL)));

	auto fishSprite_2 = Sprite::createWithSpriteFrameName("fish08_01.png");
	this->addChild(fishSprite_2);
	fishSprite_2->setFlipX(true);
	fishSprite_2->setPosition( 0, 3*winSize.height / 4);
	auto animation_2 = Animation::create();
	for (int i = 1; i <= 16; i++)
	{
		if (i >= 10)
		{
			animation_2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish08_%d.png",i)));
		}
		else
		{
			animation_2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("fish08_0%d.png", i)));
		}
	}
	animation_2->setDelayPerUnit(3.0f / 30.0f);
	AnimationCache::getInstance()->addAnimation(animation_2, "move08");
	auto move_2 = AnimationCache::getInstance()->getAnimation("move08");
	auto animate_2 = Animate::create(move_2);
	auto repeatAnimate_2 = RepeatForever::create(animate_2);
	auto moveBy_2 = MoveBy::create(15, Vec2(winSize.width + 10, 0));
	auto moveByReverse_2 = moveBy_2->reverse();
	ActionInstant *flipx_2 = FlipX::create(false);
	fishSprite_2->runAction(repeatAnimate_2);
	fishSprite_2->runAction(RepeatForever::create(Sequence::create(moveBy_2, flipx_2, moveByReverse_2, flipx_2->reverse(), NULL)));
	
	auto fishSprite_3 = Sprite::createWithSpriteFrameName("bk_01.png");
	this->addChild(fishSprite_3);
	fishSprite_3->setPosition(Vec2(200, 50));
	fishSprite_3->setScale(2);
	auto animation_3 = Animation::create();
	for (int i = 1; i <= 24; i++)
	{
		if (i >= 10)
		{
			animation_3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("bk_%d.png", i)));
		}
		else
		{
			animation_3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("bk_0%d.png", i)));
		}
	}
	animation_3->setDelayPerUnit(3.0f / 30.0f);
	AnimationCache::getInstance()->addAnimation(animation_3, "action3");
	auto action_3 = AnimationCache::getInstance()->getAnimation("action3");
	auto animate_3 = Animate::create(action_3);
	auto repeatAnimate_3 = RepeatForever::create(animate_3);
	fishSprite_3->runAction(repeatAnimate_3);

	auto fishSprite_4 = Sprite::createWithSpriteFrameName("bk_01.png");
	fishSprite_4->setFlippedX(true);
	this->addChild(fishSprite_4);
	fishSprite_4->setPosition(Vec2(600, 50));
	fishSprite_4->setScale(2);
	auto delay = DelayTime::create(3);
	fishSprite_4->runAction(RepeatForever::create((Sequence::create(delay, animate_3, NULL))));


	schedule(CC_SCHEDULE_SELECTOR(OpenLayer::updateTimesPerSecond), 2.0f);

	auto rain = ParticleRain::create();
	rain->setPosition(Vec2(500, 800));
	this->addChild(rain);
	rain->setDuration(ParticleSystem::DURATION_INFINITY);
	rain->setEmissionRate(300);
	rain->setLife(4.0f);

	m_pCursorInputLayer = CursorTextField::textFieldWithPlaceHolder(this, "click and input",
		"fj3.ttf", 40);
	m_pCursorInputLayer->setInputWidth(winSize.width);
	m_pCursorInputLayer->setPosition(Vec2(winSize.width / 2, 50));
	this->addChild(m_pCursorInputLayer);

	//_sioClient = SocketIO::connect("http://10.4.62.100:3000/", *this);
	//_sioClient->on("createCallBack", CC_CALLBACK_2(OpenLayer::createCallBack, this));
	//_sioClient->on("callConnect", CC_CALLBACK_2(OpenLayer::callConnect, this));
	
	label4 = NULL;
	label2 = Label::createWithSystemFont("", "Times New Roman", 30);
	label2->setPosition(Vec2(winSize.width *0.9, 400));
	label3 = Label::createWithSystemFont("", "Times New Roman", 30);
	label3->setPosition(Vec2(winSize.width *0.9, 50));
	this->addChild(label2, 5);
	this->addChild(label3, 5);
	return true;

}
void OpenLayer::menuCallBack(Ref *pSender)
{	
	Size winSize = Director::getInstance()->getWinSize();
	SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
	switch (((MenuItem*)pSender)->getTag())
	{
	case 101:
	{
				if (!OpenLayer::localIpAdress.empty())
				{
					tsm->goGameScene();
					break;
				}
				break;
	}
	case 102:
	{
				if (OpenLayer::localIpAdress.empty())
				{
					Director::getInstance()->end();
				}
				else
				{
					_sioClient->emit("remove", OpenLayer::localIpAdress);
				}				
				break;
				exit(0);
	}
	case 103:
	{
				if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() != true)
				{
					SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
				}
				else
				{
					SimpleAudioEngine::getInstance()->stopBackgroundMusic();
				}
				break;
	}
	case 104:
	{
				menu->setVisible(false);
				Label *label2 = Label::createWithTTF(SceneManger::WStrToUTF8(L"用摇杆来控制大鱼去吃小鱼，\n小鱼会巧妙运用鱼群的特性来躲避，\n需要在左上角规定的时间内吃满右上角要求的数目，\n满足数目后即可进入下一关，\n注意不要被落下的泡泡砸中，砸中会少血哦，\n如果去吃掉泡泡的话也可以回少量血，\n同时右下角还有技能可以使用，\n通过一关可以解锁一个技能哦！"), "fj3.ttf", 30);
				label2->setPosition(Vec2(winSize.width / 2 + 50, winSize.height*0.6));
				label2->setColor(Color3B(0, 0, 0));
				label2->setLineHeight(50);
				this->addChild(label2, 3);
				auto label_2 = Label::createWithTTF(SceneManger::WStrToUTF8(L"返回"), "fj3.ttf", 30);
				label_2->enableOutline(Color4B::BLUE, 3);
				Menu* menu_2 = NULL;
				MenuItemLabel* menuItem = MenuItemLabel::create(label_2,
					[=](Object *pSender){
					SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
					menu->setVisible(true);
					this->removeChild(label2);
					label_2->removeFromParent();
				});
				menu_2 = Menu::create(menuItem, NULL);
				menu_2->setPosition(Point::ZERO);
				this->addChild(menu_2, 2);
				menu_2->setPosition(Vec2(winSize.width - 80, 80));
				break;
	}
	case 105:
	{	
				if (!OpenLayer::localIpAdress.empty())
				{
					label2->setString(SceneManger::WStrToUTF8(L"已登录"));
				}
				else
				{

					__String *content = __String::createWithFormat("{\"content\":\"%s\",\"flag\":\"%d\"}", getLocalAddress().c_str(), 1);
					_sioClient->emit("create", content->getCString());


				}

				break;
	}
	case 108:
	{
				string aaa = m_pCursorInputLayer->getInputText();
				label3->setString(aaa);
				socketIoIp = "http://" + aaa + ":3000/";
				_sioClient = SocketIO::connect(socketIoIp, *this);
				_sioClient->on("createCallBack", CC_CALLBACK_2(OpenLayer::createCallBack, this));
				_sioClient->on("callConnect", CC_CALLBACK_2(OpenLayer::callConnect, this));
				CCLOG("%s", socketIoIp.c_str());
	}
	}
}
void OpenLayer::updateTimesPerSecond(float delta)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto sprite = Sprite::create("qipao.png");
	this->addChild(sprite);
	sprite->setPosition(CCRANDOM_0_1()*visibleSize.width, 0);
	auto moveBy = MoveBy::create(5, Vec2(0, visibleSize.height));
	auto callFunc = CallFunc::create([this,sprite]{
		this->removeChild(sprite); });
	auto sequence = Sequence::create(moveBy, callFunc, NULL);
	sprite->runAction(sequence);
}

string OpenLayer::getLocalAddress()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
		return "";
	char local[255] = { 0 };
	gethostname(local, sizeof(local));
	hostent* ph = gethostbyname(local);
	if (ph == NULL)
		return "";
	in_addr addr;
	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr));
	string localIP;
	localIP.assign(inet_ntoa(addr));
	localIpAdress = localIP;
	WSACleanup();
	return localIP;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Size winSize = Director::getInstance()->getWinSize();
	/*int sock_get_ip;
	struct   sockaddr_in *sin;
	struct   ifreq ifr_ip;
	if ((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		CCLOG("get_ip error");
		auto label4 = Label::createWithSystemFont("get_ip error", "Times New Roman", 30);
		label4->setPosition(Vec2(winSize.width *0.9, 400));
		this->addChild(label4);
		return "";
	}
	memset(&ifr_ip, 0, sizeof(ifr_ip));
	strncpy(ifr_ip.ifr_name, "wlan0", sizeof(ifr_ip.ifr_name) - 1);
	if (ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0){
		char errmsg[30];
		string mesg(strerror(errno));
		auto label4 = Label::createWithSystemFont(mesg, "Times New Roman", 30);
		label4->setPosition(Vec2(winSize.width *0.9, 400));
		this->addChild(label4);
		sprintf(errmsg, "getip1:%d", errno);
		return "";
	}
	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
	char *szLocalIP = NULL;
	strcpy(szLocalIP, inet_ntoa(sin->sin_addr));
	close(sock_get_ip);
	string localIP(szLocalIP);
	localIpAdress = localIP;
	return localIP;*/
	int sockfd;
	struct ifconf ifconf;
	struct ifreq *ifreq;
	char buf[512];
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;
	string allIp;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		auto label4 = Label::createWithSystemFont("sockfd error", "Times New Roman", 30);
		label4->setPosition(Vec2(winSize.width *0.9, 400));
		this->addChild(label4);
		return "";
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);
	ifreq = (struct ifreq*)ifconf.ifc_buf;
	for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i>0; i--)
	{
		if (ifreq->ifr_flags == AF_INET)
		{
			char* IP = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
			ifreq++;
			if (!strcmp(IP, "127.0.0.1"))
				continue;
			string localIp(IP);
			allIp = allIp + localIp;
		}
	}
	localIpAdress = allIp;
	return allIp;
#endif
}
void OpenLayer::onConnect(SIOClient* client)
{
	log("HelloWorld::onConnect called");
}
void OpenLayer::onMessage(SIOClient* client, const std::string& data)
{	
	log("HelloWorld::onMessage called");
	if (data.length() == 5)
	{
		_sioClient->disconnect();
		Director::getInstance()->end();
	}
}
void OpenLayer::onClose(SIOClient* client)
{
	if (client == _sioClient)
		_sioClient = nullptr;
	log("HelloWorld::onClose called");

}
void OpenLayer::onError(SIOClient* client, const std::string& data)
{
	log("HelloWorld::onError called");
}
void OpenLayer::createCallBack(SIOClient *client, const std::string& data)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	label2->setString(data);
}
void OpenLayer::callConnect(SIOClient* client, const std::string& data)
{
	CCLOG("callConnect success");
}


