#include"SceneManger.h"
#include"LoadLayer.h"
bool LoadLayer::init()
{
	Size winSize = Director::getInstance()->getWinSize(); //窗口大小

	
	Label *label1 = Label::createWithSystemFont(SceneManger::WStrToUTF8(L"Load. . ."), "Times New Roman", 30);
	label1->setColor(Color3B(0, 0, 0));
	label1->setPosition(Vec2(winSize.width/2, winSize.height*0.18 ));
	this->addChild(label1,1);   //loading...
	


	Label *label2 = Label::createWithTTF(SceneManger::WStrToUTF8(L"中国科协技术协会"), "fj3.ttf", 60);
	label2->setPosition(Vec2(winSize.width / 2, winSize.height*0.8));
	label2->setColor(Color3B(0, 0, 0));
	this->addChild(label2, 1);    //抬头

	Label *label3 = Label::createWithTTF(SceneManger::WStrToUTF8(L"制作：河海大学"), "fj3.ttf", 30);
	label3->setPosition(Vec2(winSize.width / 2, winSize.height*0.55));
	label3->setColor(Color3B(0, 0, 0));
	this->addChild(label3, 1);
	
	auto jindutiao = Sprite::create("res/jindutiao3.png");
	jindutiao->setPosition(Vec2(winSize.width*0.5, winSize.height*0.25));
	jindutiao->setScale(0.8);
	this->addChild(jindutiao,1); //进度条底部
	
	progTime = ProgressTimer::create(Sprite::create("res/jindutiao2.png"));
	progTime->setPosition(Vec2(winSize.width*0.5, winSize.height*0.25));
	progTime->setScale(0.8);
	progTime->setPercentage(0.0f);
	progTime->setType(ProgressTimer::Type::BAR);
	progTime->setBarChangeRate(Vec2(1, 0));
	progTime->setMidpoint(Vec2(0, 0));
	this->addChild(progTime,2);   //滑动进度条
	
	auto _background = Sprite::create("res/loadScene.jpg");
	_background->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	_background->setScale(1.45);
	this->addChild(_background);   //背景图
	
	return true;
}
void LoadLayer::onScheduleOnce(float t)
{ 
	
	tsm->goOpenScene();
	
}
void LoadLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
	ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
	ValueVector effects = map.at("Sounds").asValueVector();
	ValueVector musics = map.at("Musics").asValueVector();
	_sourceCount = spriteSheets.size() + effects.size() + musics.size();
	_progressInterval = 100 / _sourceCount;
	loadMusic(musics);
	loadEffect(effects);
	loadSpriteSheets(spriteSheets);

}
void LoadLayer::loadMusic(ValueVector musicFiles)
{
	for (int i = 0; i < musicFiles.size(); i++)
	{
		Value v = musicFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(v.asString().c_str());
		progressUpdate();
	}
}
void LoadLayer::loadEffect(ValueVector effectFiles)
{
	for (int i = 0; i < effectFiles.size(); i++)
	{
		Value v = effectFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadEffect(v.asString().c_str());
		progressUpdate();
	}
}
void LoadLayer::loadSpriteSheets(ValueVector spriteSheets)
{
	for (int i = 0; i < spriteSheets.size(); i++)
	{
		Value v = spriteSheets.at(i);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
		progressUpdate();
	}
}
void LoadLayer::progressUpdate()
{
	if (--_sourceCount)
	{
		progTime->setPercentage(100.0f - (_progressInterval*_sourceCount));
	}
	else
	{
	auto pft = ProgressFromTo::create(3, progTime->getPercentage(), 100);
	progTime->runAction(pft);
	scheduleOnce(CC_SCHEDULE_SELECTOR(LoadLayer::onScheduleOnce), 3.0);  //定时跳转
	}
}