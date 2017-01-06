#include"FishLayer.h"
#include"HRocker.h"
#include"SceneManger.h"
#include<random>
#include"Skill.h"
#include"OpenLayer.h"
#define PI 3.1415926
#define fishCount 30
#define fishEye 200 
#define lightFishEye 400
int FishLayer::level = 0;
int FishLayer::T = 0;
float FishLayer::speed = 3.5;
float FishLayer::rotationSpeed = 2.5;

bool FishLayer::init()
{ 
	initServer();
	_timeLost = 30 + level * 15;
	qipao = NULL;
	lightFishCall = false;
	_connected = false;
	visibleSize = Director::getInstance()->getVisibleSize();
	string a[] = { "background1.png", "background2.png", "background3.png" };

	_background1 = this->setBackground1(a[level], 1);
	
	rocker = HRocker::createHRocker("center.png", "back.png", Vec2(100, 100));
	rocker->startRocker(true);
	this->addChild(rocker,4);
	rocker->setOpacity(100);
	
	std::uniform_int_distribution<unsigned> u(0, 1000);
	std::default_random_engine e;
	for (int i = 0; i < fishCount; i++)
	{
		Fish *fishNode = Fish::create();
		fishNode->setTag(i);
		fish.insert(i, fishNode);
		this->addChild(fishNode, 2);
		float randomX =u(e)/1000.0*visibleSize.width*2;
		float randomY = u(e)/1000.0*visibleSize.height*2;
		fishNode->setPosition(randomX, randomY);
	}
	waterWeedsInit();
	shark =Shark::create();
	this->addChild(shark, 2);	
	shark->setPosition(Vec2(80,300));
	shark->setScale(0.7);

	lightFish = LightFish::create();
	this->addChild(lightFish, 2);
	lightFish->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    label1 = Label::createWithSystemFont("", "Times New Roman", 30);
	label1->setPosition(Vec2(visibleSize.width-50, visibleSize.height-30));
	this->addChild(label1, 5);

	label2 = Label::createWithSystemFont("", "Times New Roman", 30);
	label2->setPosition(Vec2(180, visibleSize.height-30));
	this->addChild(label2, 5);

	label3 = Label::createWithSystemFont(SceneManger::WStrToUTF8(L"倒计时："), "Times New Roman", 30);
	label3->setPosition(Vec2(100, visibleSize.height - 30));
	this->addChild(label3, 5);

	label4 = Label::createWithSystemFont("", "Times New Roman", 30);
	label4->setPosition(Vec2(500,500));
	this->addChild(label4, 5);

	if (!_connected)
	{
		label5 = Label::createWithSystemFont(SceneManger::WStrToUTF8(L"请等待连接..."), "Times New Roman", 80);
		label5->setPosition(Vec2(500, 400));
		this->addChild(label5, 5);
	}
	else
	{
		label5 = nullptr;
	}
	auto label_7 = Label::createWithTTF(SceneManger::WStrToUTF8(L"退出游戏"), "fj3.ttf", 30);
	label_7->enableOutline(Color4B::BLUE, 3);
	auto menu_7 = MenuItemLabel::create(label_7, [=](Ref *pSender){if (OpenLayer::localIpAdress.empty())
	{
		SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
		Director::getInstance()->end();
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
		_sioClient->emit("remove", OpenLayer::localIpAdress);
	} });

	menu_7->setPosition(Vec2(visibleSize.width*0.9, visibleSize.height*0.1));

	blood = LoadingBar::create("blood.png",100);
	blood->setDirection(LoadingBar::Direction::LEFT);
	blood->setScale(1.2);
	blood->setScaleX(1.2);
	blood->setPercent(100);
	this->addChild(blood, 5);
	blood->setPosition(Vec2(350, visibleSize.height - 30));

	blood_2 = LoadingBar::create("blood.png", 100);
	blood_2->setDirection(LoadingBar::Direction::LEFT);
	blood_2->setScale(1.2);
	blood_2->setScaleX(1.2);
	blood_2->setPercent(100);
	this->addChild(blood_2, 5);
	blood_2->setPosition(Vec2(700, visibleSize.height - 30));
	
	sharkHead = cutShark1(shark->zSprite);
	sharkBody = cutShark2(shark->zSprite);
	lightFishBody = cutLightFish(lightFish->lightFishImage);
	skill = new Skill();
	if (level == 0)
	{
		auto button=skill->create("button1.png", 0, shark,this);
		menu = Menu::create(button, menu_7, NULL);
		this->addChild(menu, 5);
		menu->setPosition(Vec2::ZERO);
		button->setPosition(4 * visibleSize.width / 5, visibleSize.height / 5);
	}
	if (level ==1 )
	{
		auto button_0 = skill->create("button1.png", 0, shark, this);
		auto button_1 = skill->create("button2.png", 1, shark, this);
		menu = Menu::create(button_0, button_1, menu_7, NULL);
		this->addChild(menu, 5);
		menu->setPosition(Vec2::ZERO);
		button_0->setPosition(4 * visibleSize.width / 5, visibleSize.height / 5);
		button_1->setPosition(4 * visibleSize.width / 5 + 70, visibleSize.height / 5);
	}
	if (level == 2)
	{
		auto button_0 = skill->create("button1.png", 0, shark, this);
		auto button_1 = skill->create("button2.png", 1, shark, this);
		auto button_2 = skill->create("button6.png", 2, shark, this);
		menu = Menu::create(button_0, button_1, button_2, menu_7, NULL);
		this->addChild(menu, 5);
		menu->setPosition(Vec2::ZERO);
		button_0->setPosition(4 * visibleSize.width / 5, visibleSize.height / 5);
		button_1->setPosition(4 * visibleSize.width / 5 + 70, visibleSize.height / 5);
		button_2->setPosition(4 * visibleSize.width / 5 + 140, visibleSize.height / 5);	

	}
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm_scene2.mp3", true);
	}
	
	_sioClient = SocketIO::connect(OpenLayer::socketIoIp, *this);
	_sioClient->on("farDisconnect", CC_CALLBACK_2(FishLayer::farDisconnect, this));
	_sioClient->on("callConnect", CC_CALLBACK_2(FishLayer::callConnect, this));
	return true; 
}
void FishLayer::update(float delata)
{
	eraseFish = 0;
	if (rocker->isCanMove)
	{
		sharkMove();
	}
	for (int i = 0; i < fishCount; i++)
	{
		if (fish.find(i) == fish.end())
		{
			continue;
		}
		Vec2 fish_a = fish.at(i)->getPosition();
		fish.at(i)->reLocation(fish_a);
		if (getBoundingBox(sharkHead).intersectsRect(getBoundingBox(fish.at(i)->fishSprite)))
		{
			this->removeChild(fish.at(i));
			fish.erase(i);
			eraseFish = i;
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
			{
				SimpleAudioEngine::getInstance()->playEffect("shark_eat2.mp3");
			}
			continue;
		}
		if (qipao != NULL&&getBoundingBox(qipao).intersectsRect(getBoundingBox(fish.at(i)->fishSprite)))
		{
			auto explosion = ParticleExplosion::create();
			this->addChild(explosion,2);
			explosion->setLife(0.1f);
			explosion->setAutoRemoveOnFinish(true);
			explosion->setPosition(qipao->getPosition());
			this->removeChild(qipao);
			qipao = NULL;
			this->removeChild(fish.at(i));
			fish.erase(i);
			eraseFish = i;
			SimpleAudioEngine::getInstance()->playEffect("sfx_slot_stop.mp3");
			continue;
		}
		if (find(lock.begin(), lock.end(), i) != lock.end())
		{
			continue;
		}
		Vec2 sharkPoint = shark->getPosition();
		float distance_fishAndShark = sqrt(pow(sharkPoint.x - fish_a.x, 2) + pow(sharkPoint.y - fish_a.y, 2));
		float distance_fishAndFish = 0;
		Vec2 fish_b;
		float temp = 100;
		int k = fishCount+10;
		int roundCount = 0;
		if (lightFishCall)
		{
			Vec2 lightFishPoint = lightFish->getPosition();
			float x = lightFishPoint.x - fish_a.x;
			float y = lightFishPoint.y - fish_a.y;
			float dis = sqrt(pow(x, 2) + pow(y, 2));
			if (dis < 400&&dis>100)
			{
				fish.at(i)->setPosition(fish_a + 1.5*ccpNormalize(Vec2(x, y)));
				if (lightFishPoint.x < fish_a.x)
				{
					fish.at(i)->fishSprite->setFlippedX(false);
				}
				else
				{
					fish.at(i)->fishSprite->setFlippedX(true);
				}
				continue;
			}
		}
		if (distance_fishAndShark < 500)
		{
			for (int j = 0; j < fishCount; j++)
			{
				if (fish.find(j) == fish.end())
				{
					continue;
				}
				if (find(lock.begin(), lock.end(), j)!=lock.end())
				{
					continue;
				}
				if (i != j)
				{
					fish_b = fish.at(j)->getPosition();
					distance_fishAndFish = sqrt(pow(fish_b.x - fish_a.x, 2) + pow(fish_b.y - fish_a.y, 2));
					if (distance_fishAndFish<60)
						roundCount++;
					if (temp>distance_fishAndFish&&(sharkPoint.x<fish_a.x&&fish_a.x<fish_b.x)&&(fish_b.x<fish_a.x&&fish_a.x<sharkPoint.x))
					{
						temp = distance_fishAndFish;
						k = j;
					}
				}
			}
		}
		else 
		{
			auto iter = find(lock.begin(), lock.end(), i);
			if (iter != lock.end())
			{
				lock.erase(iter);
				continue;
			}
		}
		if (temp < 200 && sharkPoint.x<fish_a.x&&fish_a.x<fish_b.x || temp < 200 && sharkPoint.x >= fish_a.x&&fish_a.x >= fish_b.x)
		{
			float xie = sqrt(pow(fish_b.x - fish_a.x, 2) + pow(fish_b.y - fish_a.y, 2));
			float x = fish_b.x - fish_a.x;
			float y = fish_b.y - fish_b.x;
			float cosAngle = x / xie;
			float rad = acos(cosAngle);
			if (fish_b.y < fish_a.y)
			{
				rad = -rad;
			}
			float x_move = cos(rad);
			float y_move = sin(rad);
			fish.at(i)->setPosition(Vec2(fish_a.x + x_move, fish_a.y + y_move));
		}
		auto rotation = shark->getRotation();
		if (sharkPoint.x < fish_a.x&&sharkPoint.y>fish_a.y&&distance_fishAndShark<250+10*roundCount&&(abs(rotation)<=60||abs(rotation)>=120))
		{
			fish.at(i)->fishSprite->setFlipX(true);
			fish.at(i)->setPosition(fish_a.x + 1.5, fish_a.y - CCRANDOM_0_1());
		}
		else if (sharkPoint.x < fish_a.x&&sharkPoint.y <= fish_a.y&&distance_fishAndShark<250 + 10 * roundCount && (abs(rotation) <= 60 || abs(rotation) >= 120))
		{
			fish.at(i)->fishSprite->setFlipX(true);
			fish.at(i)->setPosition(fish_a.x + 1.5, fish_a.y + CCRANDOM_0_1());
		}
		else if (sharkPoint.x >= fish_a.x&&sharkPoint.y > fish_a.y&&distance_fishAndShark < 250 + 10 * roundCount && (abs(rotation) <= 60 || abs(rotation) >= 120))
		{
			fish.at(i)->fishSprite->setFlipX(false);
			fish.at(i)->setPosition(fish_a.x - 1.5, fish_a.y - CCRANDOM_0_1());
		}
		else if (sharkPoint.x >= fish_a.x&&sharkPoint.y <= fish_a.y&&distance_fishAndShark < 250 + 10 * roundCount && (abs(rotation) <= 60 || abs(rotation) >= 120))
		{
			fish.at(i)->fishSprite->setFlipX(false);
			fish.at(i)->setPosition(fish_a.x - 1.5, fish_a.y + CCRANDOM_0_1());
		}
		else if ((abs(rotation) > 60 && abs(rotation) < 120) && distance_fishAndShark < 250 + 10 * roundCount&&testDirection(fish.at(i)) == 1)
		{
			fish.at(i)->fishSprite->setFlipX(true);
			fish.at(i)->setPosition(fish_a.x + 1.5, fish_a.y);
		}
		else if ((abs(rotation) > 60 && abs(rotation) < 120) && distance_fishAndShark < 250 + 10 * roundCount&&testDirection(fish.at(i)) == 0)
		{
			fish.at(i)->fishSprite->setFlipX(false);
			fish.at(i)->setPosition(fish_a.x - 1.5, fish_a.y);
		}
		fish.at(i)->move();
	}
     std::string str2 = StringUtils::format("%d/%d", fishCount-fish.size(),level*5+10);
     label1->setString(str2);

	for (auto it = bubble.begin(); it != bubble.end(); )
	{
		if ((*it)->getPosition().y == -20)
		{
			this->removeChild((*it));
			it = bubble.erase(it);
		}
		if (getBoundingBox(sharkHead).intersectsRect(getBoundingBox((*it)->bubbleImage)))
		{
			blood->setPercent(blood->getPercent() + 5);
			this->removeChild((*it));
			it = bubble.erase(it);
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
			{
				SimpleAudioEngine::getInstance()->playEffect("bubble2.mp3");
			}
			continue;
		}
		if (getBoundingBox(lightFishBody).intersectsRect(getBoundingBox((*it)->bubbleImage)))
		{
			auto explosion = ParticleExplosion::create();
			this->addChild(explosion, 2);
			explosion->setAutoRemoveOnFinish(true);
			explosion->setPosition((*it)->getPosition());
			explosion->setEmissionRate(200);
			explosion->setLife(0.3);
			explosion->setStartColor(Color4F(1, 0, 1, 1));
			explosion->setEndColor(Color4F(1, 0, 1, 1));
			this->removeChild((*it));
			it = bubble.erase(it);
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
			{
				SimpleAudioEngine::getInstance()->playEffect("bubble2.mp3");
			}
			continue;
		}
		if (getBoundingBox(sharkBody).intersectsRect(getBoundingBox((*it)->bubbleImage)))
		{
			auto explosion = ParticleExplosion::create();
			this->addChild(explosion, 2);
			explosion->setAutoRemoveOnFinish(true);
			explosion->setPosition((*it)->getPosition());
			explosion->setEmissionRate(200);
			explosion->setLife(0.3);
			explosion->setStartColor(Color4F(1,0,1,1));
			explosion->setEndColor(Color4F(1, 0, 1, 1));
			blood->setPercent(blood->getPercent() - (*it)->getScale() * 20);
			this->removeChild((*it));
			it=bubble.erase(it);
			if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
			{
				SimpleAudioEngine::getInstance()->playEffect("bubble2.mp3");
			}
			continue;
		}
		else
		{
			++it;
		}	
	}
	if (_timeLost < 0||blood->getPercent()<=0)
	{
		lose();
		unscheduleUpdate();
		unschedule(CC_SCHEDULE_SELECTOR(FishLayer::timeLost));
	}
	if (fishCount-fish.size() == 10+level*5||blood_2->getPercent()<=0)
	{
		win();
		unscheduleUpdate();
		unschedule(CC_SCHEDULE_SELECTOR(FishLayer::timeLost));
	}
	
}
void FishLayer::setViewPointCenter(float duration, Vec2 position1,Vec2 position2)
{
	int x = position2.x - position1.x;
	this->setPosition(Vec2(this->getPosition().x - x, this->getPosition().y));
}
void FishLayer::sharkMove()
{
	Vec2 point = shark->getPosition();

	if (point.x < 20 || point.y < 20 || point.y > visibleSize.height - 20 || point.x>2*visibleSize.width-20)
	{
		shark->setPosition(Vec2(shark->getPosition().x - 4 * cos(-shark->getRotation()*PI / 180), shark->getPosition().y - 4 * sin(-shark->getRotation()*PI / 180)));
		return;
	}
	if (shark->getRotation() > 180)
	{
		float a = shark->getRotation() - 180;
		shark->setRotation(-180 + a);
	}
	else if (shark->getRotation() < -180)
	{
		float a = shark->getRotation() + 180;
		shark->setRotation(180 + a);
	}
	if (abs(rocker->angle) >= shark->getRotation()*PI / 180 && rocker->angle <= 0 && shark->getRotation() >= 0)
	{
		if (abs(rocker->angle*180/PI +shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() + 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	else if (abs(rocker->angle) < shark->getRotation()*PI / 180 && rocker->angle <= 0 && shark->getRotation() >= 0)
	{
		if (abs(rocker->angle * 180 / PI + shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() - 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	else if (rocker->angle>0 && shark->getRotation()<0 && (rocker->angle)>abs(shark->getRotation()*PI / 180))
	{
		if (abs(rocker->angle * 180 / PI + shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() - 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	else if (rocker->angle > 0 && shark->getRotation() < 0 && (rocker->angle) < abs(shark->getRotation()*PI / 180))
	{
		if (abs(rocker->angle * 180 / PI + shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() + 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	else if (rocker->angle>0 && shark->getRotation() >= 0 && rocker->angle * 180 / PI + shark->getRotation() <= 180)
	{
		if (abs(rocker->angle * 180 / PI + shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() - 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	else if (rocker->angle > 0 && shark->getRotation() >= 0 && rocker->angle * 180 / PI + shark->getRotation() > 180)
	{
		if (abs(rocker->angle * 180 / PI + shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() + 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	else if (rocker->angle < 0 && shark->getRotation() < 0 && (abs(rocker->angle * 180 / PI) + abs(shark->getRotation())) >= 180)
	{
		if (abs(rocker->angle * 180 / PI + shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() - 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	else if (rocker->angle < 0 && shark->getRotation() < 0 && abs(rocker->angle * 180 / PI) + abs(shark->getRotation()) < 180)
	{
		if (abs(rocker->angle * 180 / PI + shark->getRotation())>2)
		{
			shark->setRotation(shark->getRotation() + 2);
			shark->setPosition(Vec2(shark->getPosition().x + rotationSpeed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + rotationSpeed * sin(-shark->getRotation()*PI / 180)));
		}
		else
		{
			shark->setPosition(Vec2(shark->getPosition().x + speed * cos(-shark->getRotation()*PI / 180), shark->getPosition().y + speed * sin(-shark->getRotation()*PI / 180)));
		}
	}
	if (shark->getPosition().x > visibleSize.width / 2 && shark->getPosition().x < 3 * visibleSize.width / 2)
	{
		if (shark->getPosition().y>20 && shark->getPosition().y < visibleSize.height-20)
		{
				float x = shark->getPosition().x - point.x;
				moveNode(x);
		}
	}
}
Rect FishLayer::getBoundingBox(Sprite *sprite)const
{
	Rect rect(0, 0, sprite->getBoundingBox().size.width, sprite->getBoundingBox().size.height);
	return RectApplyAffineTransform(rect, sprite->getNodeToWorldAffineTransform());
}
Sprite *FishLayer::cutShark1(Sprite* sprite_shark)
{
	auto sprite = Sprite::create();
	sprite->setContentSize(Size(1,30));
	sprite_shark->addChild(sprite);
	Size size = sprite_shark->getBoundingBox().size;
	sprite->setPosition(Vec2(size.width-10,size.height/2));
	return sprite;
}
void FishLayer::updateTimesPerSecond(float delta)
{
		auto aaa = Bubble::create("4.png",CCRANDOM_0_1()*3+3);
		aaa->setScale(CCRANDOM_0_1());
		bubble.pushBack(aaa);
		this->addChild(aaa,3);
		Vec2 random = aaa->randomGenerate();
		aaa->setPosition(random);
		aaa->down();
		if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
		{
			SimpleAudioEngine::getInstance()->playEffect("bubble.mp3");
			SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
		}
		sendBubbleData(aaa->second, random, aaa->getScale());
}
Sprite* FishLayer::cutShark2(Sprite* sprite_shark)
{
	auto sprite = Sprite::create();
	sprite->setContentSize(Size(250,20));
	sprite_shark->addChild(sprite);
	Size size = sprite_shark->getBoundingBox().size;
	sprite->setPosition(Vec2(size.width/2, size.height / 2));
	return sprite;
}
Sprite *FishLayer::cutLightFish(Sprite *lightFish)
{
	auto sprite = Sprite::create();
	sprite->setContentSize(Size(150, 20));
	lightFish->addChild(sprite);
	Size size = lightFish->getBoundingBox().size;
	sprite->setPosition(Vec2(size.width / 2, size.height / 2));
	return sprite;
}
void FishLayer::timeLost(float delta)
{
	std::string str2 = StringUtils::format("%d", _timeLost--);
	label2->setString(str2);
}

void FishLayer::onNewConnect(HSocket socket)
{
	CCLOG("socketServr onNewConnct");
	_connected = true;
	this->removeChild(label5);
	scheduleUpdate();
	schedule(CC_SCHEDULE_SELECTOR(FishLayer::timeLost), 1.0f);
	schedule(CC_SCHEDULE_SELECTOR(FishLayer::updateTimesPerSecond), 1.0f - level*0.3);
	schedule(CC_SCHEDULE_SELECTOR(FishLayer::sendData));

}
int FishLayer::testDirection(Fish* fish)
{
	auto rotation = -shark->getRotation();
	auto fish_point = fish->getPosition();
	auto shark_point = shark->getPosition();
	float x = (fish_point.y - shark_point.y) / tan(rotation*PI/180) + shark_point.x;
	if (x >= fish_point.x)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
void FishLayer::win()
{
	if (level == 2)
	{
		_sioClient->emit("reset", OpenLayer::localIpAdress);
		GameLayer::farIpAddress = "";
		auto label = Label::createWithTTF(SceneManger::WStrToUTF8(L"您赢得了胜利"), "fj3.ttf", 50);
		label->enableOutline(Color4B::RED, 3);
		this->addChild(label, 2);
		Vec2 point = Vec2(visibleSize.width / 2, visibleSize.height / 2);
		label->setPosition(label->convertToNodeSpace(point));
		auto label_1 = Label::createWithTTF(SceneManger::WStrToUTF8(L"返回"), "fj3.ttf", 30);
		label_1->enableOutline(Color4B::BLUE, 3);
		auto menuItem = MenuItemLabel::create(label_1,
			[this](Object *pSender){
			SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
			FishLayer::level = 0;
			auto layer = GameLayer::create();
			Scene *scene = (Scene*)this->getParent();
			layer->tsm = SceneManger::p;
			scene->addChild(layer);
			this->removeFromParent();
		});
		auto menu = Menu::create(menuItem, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 2);
		menuItem->setPosition(menuItem->convertToNodeSpace(point) + Vec2(-30, -100));
		delete(skill);
		skill = NULL;
	}
	else
	{
		auto label = Label::createWithTTF(SceneManger::WStrToUTF8(L"本关胜利"), "fj3.ttf", 30);
		label->enableOutline(Color4B::RED, 3);
		this->addChild(label, 2);
		Vec2 point = Vec2(visibleSize.width / 2, 2 * visibleSize.height / 3);
		label->setPosition(label->convertToNodeSpace(point));
		auto label_2 = Label::createWithTTF(SceneManger::WStrToUTF8(L"进入下一关"), "fj3.ttf", 30);
		label_2->enableOutline(Color4B::BLUE, 3);
		auto itemFont = MenuItemLabel::create(label_2, CC_CALLBACK_1(FishLayer::nextLevel, this));
		auto menu = Menu::create(itemFont, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 2);
		itemFont->setPosition(itemFont->convertToNodeSpace(point) + Vec2(-75, -100));
		delete(skill);
		skill = NULL;
	}
}
void FishLayer::lose()
{
	if (FishLayer::level == 2)
	{
		_sioClient->emit("reset", OpenLayer::localIpAdress);
		GameLayer::farIpAddress = "";
		auto label = Label::createWithTTF(SceneManger::WStrToUTF8(L"您输给了对手"), "fj3.ttf", 50);
		label->enableOutline(Color4B::RED, 3);
		this->addChild(label, 2);
		Vec2 point = Vec2(visibleSize.width / 2, visibleSize.height / 2);
		label->setPosition(label->convertToNodeSpace(point));
		auto label_1 = Label::createWithTTF(SceneManger::WStrToUTF8(L"返回"), "fj3.ttf", 30);
		label_1->enableOutline(Color4B::BLUE, 3);
		auto menuItem = MenuItemLabel::create(label_1,
			[this](Object *pSender){
			FishLayer::level = 0;
			auto layer = GameLayer::create();
			Scene *scene = (Scene*)this->getParent();
			layer->tsm = SceneManger::p;
			scene->addChild(layer);

			this->removeFromParent();
		});
		auto menu = Menu::create(menuItem, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 2);
		menuItem->setPosition(menuItem->convertToNodeSpace(point) + Vec2(-30, -100));
		delete(skill);
		skill = NULL;
	}
	else
	{
		auto label_1 = Label::createWithTTF(SceneManger::WStrToUTF8(L"进入下一关"), "fj3.ttf", 30);
		label_1->enableOutline(Color4B::BLUE, 3);
		auto itemFont = MenuItemLabel::create(label_1, CC_CALLBACK_1(FishLayer::nextLevel, this));
		auto menu = Menu::create(itemFont, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 2);
		Vec2 point2 = Vec2(visibleSize.width / 2 - 70, visibleSize.height / 2);
		itemFont->setPosition(itemFont->convertToNodeSpace(point2));
		auto label = Label::createWithTTF(SceneManger::WStrToUTF8(L"胜败乃兵家常事"), "fj3.ttf", 30);
		label->enableOutline(Color4B::RED, 3);
		this->addChild(label, 2);
		Vec2 point = Vec2(visibleSize.width / 2, 2 * visibleSize.height / 3);
		label->setPosition(label->convertToNodeSpace(point));

		delete(skill);
		skill = NULL;
	}
}
void FishLayer::restart(Ref* psend)
{
	SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
	auto tsm = this->getParent();
	auto layer = FishLayer::create();
	tsm->addChild(layer);
	this->removeFromParent();
}
void FishLayer::nextLevel(Ref* psend)
{
	SimpleAudioEngine::getInstance()->playEffect("sound_button.mp3");
	level++;
	auto layer = FishLayer::create();
    auto tsm = this->getParent();
	tsm->addChild(layer);
	this->removeFromParent();
}
void FishLayer::moveNode(float x)
{
	this->setPosition(Vec2(this->getPosition().x - x, this->getPosition().y));
	label1->setPosition(Vec2(label1->getPosition().x + x, label1->getPosition().y));
	label2->setPosition(Vec2(label2->getPosition().x + x, label2->getPosition().y));
	label3->setPosition(Vec2(label3->getPosition().x + x, label3->getPosition().y));
	label4->setPosition(Vec2(label4->getPosition().x + x, label4->getPosition().y));
	blood->setPosition(Vec2(blood->getPosition().x + x, blood->getPosition().y));
	blood_2->setPosition(Vec2(blood_2->getPosition().x + x, blood_2->getPosition().y));
	rocker->setPosition(Vec2(rocker->getPosition().x + x, rocker->getPosition().y));
	menu->setPosition(Vec2(menu->getPosition().x + x, menu->getPosition().y));
	if (skill->progress_1 != NULL)
	{
		skill->progress_1->setPosition(Vec2(skill->progress_1->getPosition().x+x, skill->progress_1->getPosition().y));
	}
	if (skill->progress_2 != NULL)
	{
		skill->progress_2->setPosition(Vec2(skill->progress_2->getPosition().x + x, skill->progress_2->getPosition().y));
	}
	if (skill->progress_3 != NULL)
	{
		skill->progress_3->setPosition(Vec2(skill->progress_3->getPosition().x + x, skill->progress_3->getPosition().y));
	}
	
}
void FishLayer::waterWeedsInit()
{
	auto waterweeds_1 = Sprite::createWithSpriteFrameName("hc_01.png");
	this->addChild(waterweeds_1, 3);
	waterweeds_1->setPosition(Vec2(100, 50));
	auto animation = Animation::create();
	for (int i = 1; i <= 16; i++)
	{
		if (i >= 10)
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("hc_%d.png", i)));
		}
		else
		{
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("hc_0%d.png", i)));
		}
	}
	animation->setDelayPerUnit(4.0f / 30.0f);
	AnimationCache::getInstance()->addAnimation(animation, "action_hc");
	auto action = AnimationCache::getInstance()->getAnimation("action_hc");
	auto animate = Animate::create(action);
	auto repeatAnimate = RepeatForever::create(animate);
	waterweeds_1->runAction(repeatAnimate);

	auto waterweeds_2 = Sprite::createWithSpriteFrameName("hc_01.png");
	this->addChild(waterweeds_2, 3);
	waterweeds_2->setPosition(Vec2(800, 50));
	waterweeds_2->setFlippedX(true);
	auto action_2 = AnimationCache::getInstance()->getAnimation("action_hc");
	auto animate_2 = Animate::create(action_2);
	auto repeatAnimate_2 = RepeatForever::create(animate_2);
	waterweeds_2->runAction(repeatAnimate_2);

	auto waterweeds_3 = Sprite::createWithSpriteFrameName("hc_01.png");
	this->addChild(waterweeds_3, 3);
	waterweeds_3->setScale(1.5);
	waterweeds_3->setPosition(Vec2(1200, 50));
	auto action_3 = AnimationCache::getInstance()->getAnimation("action_hc");
	auto animate_3 = Animate::create(action_3);
	auto repeatAnimate_3 = RepeatForever::create(animate_3);
	waterweeds_3->runAction(repeatAnimate_3);
}
void FishLayer::initServer()
{
	_server = SocketServer::getInstance();
	_server->startServer(9000);
	_server->onNewConnection = CC_CALLBACK_1(FishLayer::onNewConnect, this);
	_server->onRecv = CC_CALLBACK_3(FishLayer::onRecv, this);
	_server->onDisconnect = CC_CALLBACK_1(FishLayer::onDisconnect, this);
}
void FishLayer::sendData(float delta)
{
	CCLOG("server sendData");
	GameData_send data;
	data.dataSize = sizeof(GameData_send);
	data.sharkData.position = shark->getPosition();
	data.sharkData.rotation = shark->getRotation();
	data.sharkData.scale = shark->getScale();
	data.bubbleData.second = 0;
	data.bubbleData.scale = 0;
	data.bubbleData.position = Vec2(0,0);
	data.bloodPercent = blood->getPercent();
	data.erase = eraseFish;
	data._timeLost = this->_timeLost;
	auto it = fish.begin();
	for (it; it != fish.end(); it++)
	{
		data.fishData[it->first].position = it->second->getPosition();
		data.fishData[it->first].Flipx = it->second->fishSprite->isFlippedX();		
	}
	if (qipao != nullptr)
	{
		data.qipaoData.position = qipao->getPosition();
	}
	else
	{
		data.qipaoData.position = Vec2(0, 0);
	}
	_server->sendMessage((const char*)&data, sizeof(GameData_send));
}
void FishLayer::sendBubbleData(float second,Vec2 position,float scale)
{
	GameData_send data;
	data.bubbleData.second= second;
	data.bubbleData.scale = scale;
	data.bubbleData.position = position;
	data.sharkData.position = shark->getPosition();
	data.sharkData.rotation = shark->getRotation();
	data.sharkData.scale = shark->getScale();
	data.bloodPercent = blood->getPercent();
	data.erase = eraseFish;
	data._timeLost = this->_timeLost;
	data.dataSize = sizeof(GameData_send);
	if (qipao != nullptr)
	{
		data.qipaoData.position = qipao->getPosition();
	}
	else
	{
		data.qipaoData.position = Vec2(0, 0);
	}
	_server->sendMessage((const char*)&data, sizeof(GameData_send));
}
void FishLayer::onRecv(HSocket socket, const char* data, int count)
{
	GameData_recv *gameData_recv = (GameData_recv*)data;
	lightFish->setPosition(gameData_recv->lightFishData.position);
	lightFish->setRotation(gameData_recv->lightFishData.rotation);
	lightFish->setScale(gameData_recv->lightFishData.scale);
	lightFish->sun->setScale(gameData_recv->lightFishData.sunScale);
	this->lightFishCall = gameData_recv->lightFishData.lightFishCall;
	blood_2->setPercent(gameData_recv->lightFishData.bloodPercent);
}
void FishLayer::onDisconnect(HSocket socket)
{
	CCLOG("socketServer onDisconnect");
	unschedule(CC_SCHEDULE_SELECTOR(FishLayer::sendData));
	unschedule(CC_SCHEDULE_SELECTOR(FishLayer::updateTimesPerSecond));
	auto delay = DelayTime::create(2);
	auto fun = CallFunc::create([=](){	_server->destroyInstance();
	                                    _server = nullptr;
										 });
   this->runAction(Sequence::create(delay, fun, NULL));
	CCLOG("_server =nullptr");
}
void FishLayer::onConnect(SIOClient* client)
{
	_sioClient->emit("recordIp", OpenLayer::localIpAdress);
}
void FishLayer::onMessage(SIOClient* client, const std::string& data)
{
	log("HelloWorld::onMessage called");
	if (data.length() == 5)
	{
		_sioClient->disconnect();
		Director::getInstance()->end();
	}
}
void FishLayer::onClose(SIOClient* client)
{
	if (client == _sioClient)
		_sioClient = nullptr;
	log("HelloWorld::onClose called");

}
void FishLayer::onError(SIOClient* client, const std::string& data)
{
	log("HelloWorld::onError called");
}
void FishLayer::farDisconnect(SIOClient* client, const std::string& data)
{
	FishLayer::level = 2;
	win();
	unscheduleUpdate();
	unschedule(CC_SCHEDULE_SELECTOR(FishLayer::timeLost));
}
void FishLayer::callConnect(SIOClient* client, const std::string& data)
{
	_sioClient->emit("recordIp", OpenLayer::localIpAdress);
	CCLOG("callConnect success");
}
