#ifndef JEWEL_H
#define JEWEL_H

#include "cocos2d.h"

USING_NS_CC;

class Jewel : public Sprite
{
public:
	static Jewel* createByType(int type, int x, int y); //���ݱ�ʯ�����Լ����괴����ʯ
	bool init(int type, int x, int y);

public:
	int getType() { return m_type; }
	int getX() { return m_x; }
	int getY() { return m_y; }

	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }

	bool isSwaping() { return m_isSwaping; }
	bool isCrushing() { return m_isCrushing; }
	
	void setSwapingState(bool state) { m_isSwaping = state; }
	
	void crush(); //����

private:
	//����
	int m_type;

	//����
	int m_x;
	int m_y;

	bool m_isSwaping; //�Ƿ����ڽ���
	bool m_isCrushing; //�Ƿ���������
};

#endif
