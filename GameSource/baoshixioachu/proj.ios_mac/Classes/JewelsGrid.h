#ifndef JEWELSJewelsGrid_H
#define JEWELSJewelsGrid_H

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

#define GRID_WIDTH 40 //һ����������Ϊ40
#define MOVE_SPEED 0.2 //��ʯ�ƶ��ٶ�
#define FIRST_JEWEL_ID 1 //��һ����ʯ��ID����ʯIDĬ�����ε�����
#define LAST_JEWEL_ID 7 //���һ����ʯ��ID

class Jewel;

//�����࣬�����˱�ʯ�ڲ����ڵ��߼�
//�����񲼾����������½�Ϊԭ�㣬x��y��Ϊ������
class JewelsGrid : public Node
{
public:
	static JewelsGrid* create(int row, int col); //�����и��д�������
	bool init(int row, int col);

	void updateMap(); //���±�ʯ����
	bool isDeadMap(); //�жϵ�ǰ�����Ƿ���ͼ���������Ҳ����������

private:
	//���ڴ�����ʯ�ķ���
	Jewel* createAJewel(int x, int y); //���ݲ������괴��һ����ʯ
	bool isJewelLegal(Jewel* jewel, int x, int y); //�жϴ����ı�ʯ�Ƿ񲻻����������Ϸ���
	void setJewelPixPos(Jewel* jewel, float x, float y); //��������Ϸʵ������λ�á����������������½�Ϊԭ�㣬x��y��Ϊ����

	//���ڲ�����ʯ�ķ���
	void swapJewels(Jewel *jewelA, Jewel *jewelB); //����������ʯ
	void swapJewelToNewPos(Jewel* jewel); //�ƶ�����λ��

	void refreshJewelsGrid(); //ˢ��������ı�ʯ����
	void refreshJewelsToNewPos(int col); //ˢ��һ�б�ʯ

private:
	//�¼���Ӧ����
	bool onTouchBegan(Touch*, Event*);
	void onTouchMoved(Touch*, Event*);

private:
	//��Ϸ�߼�����
	bool canCrush(); //�жϵ�ǰ״̬�ı�ʯ�����Ƿ�������
	void goCrush(); //��ʼcrush

	//��׽��������׽���������Ƿ���ɣ�Ȼ�������������
	void onJewelsSwaping(float dt);
	void onJewelsSwapingBack(float dt);
	void onJewelsCrushing(float dt);
	void onJewelsRefreshing(float dt);

public:
	int getRow() { return m_row; }
	int getCol() { return m_col; }

private:
	int m_row; //����
	int m_col; //����

	Jewel* m_jewelSelected; //��ǰѡ��ı�ʯ
	Jewel* m_jewelSwapped; //�������ı�ʯ

	vector<vector<Jewel*>> m_JewelsBox; //��ű�ʯ�����������ͨ������������ʯ
	Vector<Jewel*> m_crushJewelBox; //׼�������ı�ʯ����
	Vector<Jewel*> m_newJewelBox; //׼�����벼�ֵı�ʯ������
};

#endif

