#include"FishLayer.h"
#include"Skill.h"
#include"SimpleAudioEngine.h"
#define PI 3.1415926
using namespace CocosDenshion;
MenuItemImage* Skill::create(const string&filename, int number, Sprite *shark, FishLayer*layer)
{
	blood = NULL;	
	switch (number)
	{
	case 0:
		button_1 = MenuItemImage::create(filename, filename, CC_CALLBACK_0(Skill::accelerate, this, shark, layer));
		return button_1;
		break;
	case 1:
		button_2 = MenuItemImage::create(filename, filename, CC_CALLBACK_0(Skill::lash, this, shark, layer));
		return button_2;
		break;
	case 2:
		button_3 = MenuItemImage::create(filename, filename, CC_CALLBACK_0(Skill::addBlood, this, shark, layer));
		blood = layer->blood;
		return button_3;
		break; 
	}
}
void Skill::accelerate(Sprite *shark,FishLayer*layer)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	button_1->setVisible(false);
	shark->setScale(0.9);
	FishLayer::speed = 5;
	FishLayer::rotationSpeed = 3.5;
	auto time = DelayTime::create(2.5);
	auto callNormal = CallFunc::create([=]{shark->setScale(0.7); FishLayer::speed = 3.5; FishLayer::rotationSpeed = 2.5; });
	shark->runAction(Sequence::create(time, callNormal, NULL));
	progress_1 = ProgressTimer::create(Sprite::create("button1.png"));
	progress_1->setPercentage(0.0f);
	progress_1->setType(ProgressTimer::Type::RADIAL);
	progress_1->setReverseDirection(false);
	layer->addChild(progress_1, 2);
	progress_1->setPosition(button_1->getPosition() - layer->getPosition());
	auto pp = ProgressFromTo::create(8, 0, 100);
	auto call = CallFunc::create([=]{button_1->setVisible(true);
	                                 layer->removeChild(progress_1);
									 progress_1 = NULL; });
	auto sequence = Sequence::create(pp, call, NULL);
	progress_1->runAction(sequence);
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
	{
		SimpleAudioEngine::getInstance()->playEffect("shark_rush.mp3");
	}
}
void Skill::lash(Sprite *shark, FishLayer*layer)
{
	SimpleAudioEngine::getInstance()->playEffect("shoot.mp3");
	auto visibleSize = Director::getInstance()->getVisibleSize();
	button_2->setVisible(false);
	progress_2 = ProgressTimer::create(Sprite::create("button2.png"));
	progress_2->setPercentage(0.0f);
	progress_2->setType(ProgressTimer::Type::RADIAL);
	progress_2->setReverseDirection(false);
	layer->addChild(progress_2, 2);
	progress_2->setPosition(button_1->getPosition() - layer->getPosition() + Vec2(70, 0));
	auto pp = ProgressFromTo::create(6, 0, 100);
	auto call = CallFunc::create([=]{button_2->setVisible(true);
	                                layer->removeChild(progress_2); 
									progress_2 = NULL; });
	auto sequence = Sequence::create(pp, call, NULL);
	progress_2->runAction(sequence);

	auto rotation = shark->getRotation();
	auto point = shark->getPosition();
	layer->qipao = Sprite::create("3.png");
	layer->addChild(layer->qipao,2);
	layer->qipao->setPosition(point);
	float speed = 500;

	Point normalizeVector = Vec2(cos(-rotation*PI / 180), sin(-rotation*PI / 180));
	float distanceMax = sqrt(pow(visibleSize.height, 2) + pow(2 * visibleSize.width, 2));
	Point moveVector = distanceMax*normalizeVector;
	float t = distanceMax / speed;
	auto moveBy = MoveBy::create(t, moveVector);
	auto call_2 = CallFunc::create([=]{layer->removeChild(layer->qipao); 
	                                   layer->qipao = NULL; });
	auto sequence_2 = Sequence::create(moveBy, call_2,NULL);
	layer->qipao->runAction(sequence_2);
}
void Skill::addBlood(Sprite *shark, FishLayer*layer)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	button_3->setVisible(false);
	progress_3 = ProgressTimer::create(Sprite::create("button6.png"));
	progress_3->setPercentage(0.0f);
	progress_3->setType(ProgressTimer::Type::RADIAL);
	progress_3->setReverseDirection(false);
	layer->addChild(progress_3, 2);
	progress_3->setPosition(button_2->getPosition() - layer->getPosition() + Vec2(70, 0));
	auto pp = ProgressFromTo::create(8, 0, 100);
	auto call = CallFunc::create([=]{button_3->setVisible(true);
	                                  layer->removeChild(progress_3); 
									  progress_3 = NULL; });
	auto sequence = Sequence::create(pp, call, NULL);
	progress_3->runAction(sequence);
	SimpleAudioEngine::getInstance()->playEffect("sfx_levelup.mp3");
	blood->setPercent(blood->getPercent() + 10);
}

