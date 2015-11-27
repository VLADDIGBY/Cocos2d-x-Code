#include "GameOverScene.h"
#include "GameScene.h"

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	Layer::init();

	auto winSize = Director::getInstance()->getWinSize();

	auto texturecache = TextureCache::getInstance();

	//��ӱ���
	auto bg = Sprite::createWithTexture(texturecache->getTextureForKey("gameover.png"));
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	addChild(bg);

	//������Ϸ��ť
	auto backlabel = Label::createWithTTF("Try again", "fonts/Marker Felt.ttf", 36);
	backlabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backlabel);
	backItem->setTarget(this, menu_selector(GameOverScene::onBackItemCallback));
	auto menu = Menu::create(backItem, nullptr);
	addChild(menu);

	//��ʾ���η���
	auto score = UserDefault::getInstance()->getStringForKey("LastScore");
	auto last_score = "last score: " + score;

	auto labelscore = Label::createWithSystemFont(last_score, "Arial", 36);
	labelscore->setColor(Color3B::BLACK);
	labelscore->setAnchorPoint(Vec2(0, 0));
	labelscore->setPosition(Vec2(winSize.width / 2 - 130, winSize.height / 2 + 100));
	addChild(labelscore);
	
	//��ʾ��ѷ���
	auto bestscore = UserDefault::getInstance()->getStringForKey("BestScore");
	auto best_score = "best score: " + bestscore;

	auto labelbestscore = Label::createWithSystemFont(best_score, "Arial", 36);
	labelbestscore->setColor(Color3B::BLACK);
	labelbestscore->setAnchorPoint(Vec2(0, 0));
	labelbestscore->setPosition(Vec2(winSize.width / 2 - 130, winSize.height / 2 + 50));
	addChild(labelbestscore);

	return true;
}

void GameOverScene::onBackItemCallback(Ref*)
{
	//������Ϸ
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}
