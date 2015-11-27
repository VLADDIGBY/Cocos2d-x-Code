#include "LoadingScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Scene* LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadingScene::init()
{
	Layer::init();

	auto winSize = Director::getInstance()->getWinSize();

	m_texture_num = 0; //��ǰ�Ѽ��ص�ͼƬ
	int *ptexture_num = &m_texture_num;

	auto label = Label::createWithSystemFont("Loading...", "Arial", 36);
	label->setPosition(winSize.width / 2, winSize.height / 2);
	addChild(label);

	//������ϻص�
	auto addTextureCallback = [ptexture_num](Texture2D* texture)
	{
		(*ptexture_num)++;
		log("load a texture async");
	};

	//�첽Ԥ���ر�ʯ
	TextureCache::getInstance()->addImageAsync("jewel1.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("jewel2.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("jewel3.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("jewel4.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("jewel5.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("jewel6.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("jewel7.png", addTextureCallback);

	//�첽Ԥ���ر���ͼ
	TextureCache::getInstance()->addImageAsync("bground1.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bground2.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bground3.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bground4.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("gameover.png", addTextureCallback);

	//�첽����һЩ��������ͼ
	TextureCache::getInstance()->addImageAsync("board.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bonusbar.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bonusbar_fill.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("bonus.png", addTextureCallback);
	
	//�������ؽ��ȼ��
	schedule(schedule_selector(LoadingScene::onTextureLoading));

	//Ԥ������Ч
	SimpleAudioEngine::getInstance()->preloadEffect("crush.ogg");
	SimpleAudioEngine::getInstance()->preloadEffect("swapback.ogg");

	return true;
}

void LoadingScene::onTextureLoading(float dt)
{
	//һ��ͼƬ������ϣ���ô������Ϸ����
	if (m_texture_num == 16)
	{
		unschedule(schedule_selector(LoadingScene::onTextureLoading));
		log("loading down!");

		auto call = CallFunc::create([](){
			auto scene = GameScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
		});

		//�ȴ�һ���������
		this->runAction(Sequence::create(DelayTime::create(0.51), call, nullptr));
	}
}