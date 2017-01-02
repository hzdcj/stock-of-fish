#include"SceneManger.h"
#include"GameLayer.h"
#include"FishLayer.h"
#include"OpenLayer.h"
string GameLayer::farIpAddress; 
bool GameLayer::init()
{
	Size winSize = Director::getInstance()->getWinSize();
	label4 = NULL;
	auto _background=setBackground1("bj1.png",1);
	auto label_1 = Label::createWithTTF(SceneManger::WStrToUTF8(L"返回"), "fj3.ttf", 40);
	label_1->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem_2 = MenuItemLabel::create(label_1, CC_CALLBACK_1(GameLayer::menuCallBack_Start, this));
	menuItem_2->setTag(105);
	menuItem_2->setPosition(Vec2(winSize.width*0.9, winSize.height*0.2));

	auto menu_1 = MenuItemImage::create("button_selectscene2_001-hd.png", "button_selectscene2_001-hd.png");
	menu_1->setPosition(Vec2(winSize.width / 2-300, winSize.height / 2));
	menu_1->setScale(0.5);
    
	auto menu_2 = MenuItemImage::create("button_selectscene3_001-hd.png", "button_selectscene3_001-hd.png");
	menu_2->setPosition(Vec2(winSize.width / 2 , winSize.height / 2));
	menu_2->setScale(0.5);

	auto menu_3 = MenuItemImage::create("button_selectscene5_002-hd.png", "button_selectscene5_002-hd.png");
	menu_3->setPosition(Vec2(winSize.width / 2+300, winSize.height / 2));
	menu_3->setScale(0.5);

	MenuItemImage *menu_4 = NULL;

	if (FishLayer::level == 0)
	{
		menu_4 = MenuItemImage::create("button_selectscene2_002-hd.png", "button_selectscene2_002-hd.png", [this](Ref *pSender){SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
		FishLayer::level = 0;
		if (!GameLayer::farIpAddress.empty())this->startGame(); });
		menu_4->setPosition(menu_1->getPosition());
		menu_4->setTag(106);
		menu_4->setScale(0.5);
	}
	if (FishLayer::level == 1)
	{
		menu_4 = MenuItemImage::create("button_selectscene3_002-hd.png", "button_selectscene3_002-hd.png", [this](Ref *pSender){SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
		FishLayer::level = 1;
		if (!GameLayer::farIpAddress.empty())this->startGame();  });
		menu_4->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		menu_4->setTag(107);
		menu_4->setScale(0.5);
	}
	if (FishLayer::level == 2)
	{
		menu_4 = MenuItemImage::create("button_selectscene5_001-hd.png", "button_selectscene5_001-hd.png", [this](Ref *pSender){SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
		FishLayer::level = 2;
		if (!GameLayer::farIpAddress.empty())this->startGame();  });
		menu_4->setPosition(menu_3->getPosition());
		menu_4->setTag(108);
		menu_4->setScale(0.5);
	}
	
	auto label_6 = Label::createWithTTF(SceneManger::WStrToUTF8(L"匹配"), "fj3.ttf", 50);
	label_6->enableOutline(Color4B::BLUE, 3);
	MenuItemLabel *menuItem_6 = MenuItemLabel::create(label_6, CC_CALLBACK_1(GameLayer::menuCallBack_Start, this));
	menuItem_6->setTag(107);
	menuItem_6->setPosition(Vec2(winSize.width /2,200));


	auto label_2 = Label::createWithTTF(SceneManger::WStrToUTF8(L"退出游戏"), "fj3.ttf", 30);
	label_2->enableOutline(Color4B::BLUE, 3);
	auto menu_7 = MenuItemLabel::create(label_2, [=](Ref *pSender){if (OpenLayer::localIpAdress.empty())
	{
		SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
		Director::getInstance()->end();
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
		_sioClient->emit("remove", OpenLayer::localIpAdress);
	} });

	menu_7->setPosition(Vec2(winSize.width*0.9, winSize.height*0.1));
	

	menu = Menu::create(menuItem_2, menu_1, menu_2, menu_3, menu_4, menu_7, menuItem_6, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 2);

	auto rain = ParticleRain::create();
	rain->setPosition(Vec2(500, 800));
	this->addChild(rain,2);
	rain->setDuration(ParticleSystem::DURATION_INFINITY);
	rain->setEmissionRate(300);
	rain->setLife(4.0f);
	_connected = false;
	label4 = Label::createWithSystemFont("", "Times New Roman", 30);
	label4->setPosition(Vec2(winSize.width / 2, 100));
	this->addChild(label4, 5);

	_sioClient = SocketIO::connect("http://10.4.62.100:3000/", *this);
	_sioClient->on("findAllCallBack", CC_CALLBACK_2(GameLayer::findAllCallBack, this));
	_sioClient->on("farDisconnect", CC_CALLBACK_2(GameLayer::farDisconnect, this));
	_sioClient->on("callConnect", CC_CALLBACK_2(GameLayer::callConnect, this));
	return true;

}
void GameLayer::menuCallBack_Start(Ref *pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
	Size winSize = Director::getInstance()->getWinSize();

	switch (((MenuItem*)pSender)->getTag())
	{
	case 104:	
		if (_connected)
		{
			this->startGame();
		}
		break;
	case 105:
		tsm->goOpenScene();
		break;
	case 107:
		if (farIpAddress.empty())
		{
			_sioClient->emit("findAll", OpenLayer::localIpAdress);
		}
		else
		{
			label4->setString(SceneManger::WStrToUTF8(L"已匹配过了"));
		}
	}
}
Sprite *GameLayer::setBackground1(const string& fileNamle,int n)
{
	Size winSize = Director::getInstance()->getWinSize();
	auto _background = Sprite::create(fileNamle);
	_background->setAnchorPoint(Vec2(0, 0));
	_background->setPosition(Vec2(0,0));
	_background->setScale(1.1);
	this->addChild(_background, n);
	return _background;
}
Sprite *GameLayer::setBackground2(const string& fileNamle, int n)
{
	Size winSize = Director::getInstance()->getWinSize();
	auto _background = Sprite::create(fileNamle);
	_background->setPosition(Vec2(3 * winSize.width / 2, winSize.height / 2));
	_background->setScale(1.1);
	this->addChild(_background, n);
	return _background;
}


void GameLayer::startGame()
{
	FishLayer *layer=FishLayer::create();
	(tsm->gameScene)->addChild(layer);
	layer->setTag(5);
	this->removeFromParent();
}
void GameLayer::onConnect(SIOClient* client)
{
	log("HelloWorld::onConnect called");
}
void GameLayer::onMessage(SIOClient* client, const std::string& data)
{
	log("HelloWorld::onMessage called");
	if (data.length() == 5)
	{
		_sioClient->disconnect();
		Director::getInstance()->end();
	}
}
void GameLayer::onClose(SIOClient* client)
{
	if (client == _sioClient)
		_sioClient = nullptr;
	log("HelloWorld::onClose called");

}
void GameLayer::onError(SIOClient* client, const std::string& data)
{
	log("HelloWorld::onError called");
}
void GameLayer::findAllCallBack(SIOClient *client, const std::string& data)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (data.length() != 2)
	{
		string temp = data;
		auto it_begin = temp.begin();
		temp.erase(it_begin);
		auto it_end = temp.end() - 1;
		temp.erase(it_end);
		farIpAddress = temp;
		label4->setString(SceneManger::WStrToUTF8(L"匹配成功"));
		auto label5 = Label::createWithSystemFont("", "Times New Roman", 30);
		label5->setPosition(Vec2(100, 100));
		this->addChild(label5, 5);
		label5->setString(farIpAddress);
		_connected = true;
	}
	else
	{
		label4->setString(SceneManger::WStrToUTF8(L"请等待匹配..."));
	}
}
void GameLayer::farDisconnect(SIOClient* client, const std::string& data)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	label4->setString(SceneManger::WStrToUTF8(L"对方连接断开"));
	_connected = false;
}
void GameLayer::callConnect(SIOClient* client, const std::string& data)
{
	_sioClient->emit("recordIp", OpenLayer::localIpAdress);
	CCLOG("callConnect success");
}
