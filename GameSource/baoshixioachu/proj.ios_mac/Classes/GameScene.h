#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"
//#include "ui\CocosGUI.h"
#include "CocosGUI.h"

USING_NS_CC;
using namespace ui;

class JewelsGrid;

class GameScene : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(GameScene);
	bool init();

public:
	static void addBonus(int bonus);

private:
	static LoadingBar* m_bonusbar;
	static int m_score;
	static Label* m_scorelabel;

private:
	void onUpdateMenuCallback(Ref*);

	void onReducingBonus(float dt); //��������ʱ

	void publishScore(); //�洢��Ϸ����

	JewelsGrid* m_jewelsgrid;
	Sprite* m_bg;
};

#endif