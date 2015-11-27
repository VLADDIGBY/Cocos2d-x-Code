#include "Jewel.h"
#include "GameScene.h"

Jewel* Jewel::createByType(int type, int x, int y)
{
	auto jewel = new Jewel();

	if (jewel && jewel->init(type, x, y))
	{
		jewel->autorelease();
		return jewel;
	}
	else
	{
		CC_SAFE_DELETE(jewel);
		return nullptr;
	}
}

bool Jewel::init(int type, int x, int y)
{
	Sprite::init();

	m_type = type;
	m_x = x;
	m_y = y;
	m_isSwaping = false;
	m_isCrushing = false;
	
	//������Դ����ʼ����������ԴӦ������Ϸ�������������ϵ�
	char name[100] = {0};
	sprintf(name, "jewel%d.png", m_type);
	this->initWithTexture(TextureCache::getInstance()->getTextureForKey(name));

	this->setAnchorPoint(Vec2(0, 0)); //���½�Ϊê��

	return true;
}

void Jewel::crush()
{
	//�ӷ�
	GameScene::addBonus(1);

	//��ʼ����������״̬Ϊ�棬ֱ��������������������ʯ�Ƴ���Ⱦ�ڵ㣬��������״̬Ϊ��
	m_isCrushing = true;
	auto action = FadeOut::create(0.2);
	auto call = CallFunc::create([this](){
		this->removeFromParent();
		m_isCrushing = false;
	});
	this->runAction(Sequence::create(action, call, nullptr));
}