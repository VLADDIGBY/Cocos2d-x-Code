#include "JewelsGrid.h"
#include "Jewel.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

JewelsGrid* JewelsGrid::create(int row, int col)
{
	auto jewelsgrid = new JewelsGrid();
	if (jewelsgrid && jewelsgrid->init(row, col))
	{
		jewelsgrid->autorelease();
		return jewelsgrid;
	}
	else
	{
		CC_SAFE_DELETE(jewelsgrid);
		return nullptr;
	}
}

bool JewelsGrid::init(int row, int col)
{
	Node::init();

	m_row = row;
	m_col = col;

	m_jewelSelected = nullptr;
	m_jewelSwapped = nullptr;

	//�������г�ʼ��һ���յı�ʯ������С
	m_JewelsBox.resize(m_row);
	for (auto &vec : m_JewelsBox)
		vec.resize(m_col);

	//1.���ݲ��ִ�С��������ʯ����
	//2.�������������½�Ϊԭ�㣬x��y��Ϊ������
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			m_JewelsBox[x][y] = createAJewel(x, y); 
		}
	}

	while (isDeadMap())
	{
		log("dead map! need to update");
		updateMap();
	}
	
	//���봥������
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(JewelsGrid::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(JewelsGrid::onTouchMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	log("JewelsGrid init!");
	return true;
}

void JewelsGrid::updateMap()
{
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			m_JewelsBox[x][y]->removeFromParent();
			m_JewelsBox[x][y] = createAJewel(x, y); 
		}
	}

	log("update a new map!");
}

bool JewelsGrid::isDeadMap()
{
	//ģ�⽻�����жϽ������Ƿ����������粻�ܣ���ô���Ǹ���ͼ
	auto swap = [](Jewel** a, Jewel** b)
	{
		auto temp = *a;
		*a = *b;
		*b = temp;
	};

	bool isDeadMap = true;

	//����ÿһ�ű�ʯ
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			//����ߵĽ���
			if (x > 0)
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x-1][y]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x-1][y]);
			}

			//���ұߵĽ���
			if (x < m_col - 1)
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x+1][y]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x+1][y]);
			}

			//������Ľ���
			if (y < m_row - 1)
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y+1]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y+1]);
			}

			//������Ľ���
			if (y > 0)
			{
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y-1]);
				if (canCrush())
					isDeadMap = false;
				swap(&m_JewelsBox[x][y], &m_JewelsBox[x][y-1]);
			}
		}
	}

	//canCrush��洢�������ı�ʯ��ȥ��������ģ�⽻�������Ի�Ҫ���
	m_crushJewelBox.clear();

	return isDeadMap;
}

Jewel* JewelsGrid::createAJewel(int x, int y)
{
	//1.���ݲ������괴��һ�ű�ʯ���������
	//2.�жϸñ�ʯ�Ƿ�Ϸ�������������
	//3.���øñ�ʯ����������
	//4.���ñ�ʯ������Ⱦ�ڵ�
	Jewel* jewel = nullptr;

	while(1)
	{
		jewel = Jewel::createByType(random(FIRST_JEWEL_ID, LAST_JEWEL_ID), x, y);
		
		if (isJewelLegal(jewel, x, y))
		{
			break;
		}
	}	

	setJewelPixPos(jewel, x, y);
	addChild(jewel);

	//log("add a jewel!---type:%d---x:%d---y:%d", jewel->getType(), x, y);

	return jewel;
}

bool JewelsGrid::isJewelLegal(Jewel* jewel, int x, int y)
{
	//1.�ֱ��ж��¼���ı�ʯ��x��y�᷽���Ƿ������
	//2.�����Ǵ���������뱦ʯ�����ֻ�����������ж�
	//3.x��y����С�ڵ���1�����ж�
	//4.����ͬʱ�Ϸ����Ϸ�
	bool isXLegal = true;
	bool isYLegal = true;

	if (x > 1)
	{
		//��x�Ḻ����ֱ�Ƚ���λ�������ʯ���Ͷ�һ������ô��ʾ�������ñ�ʯ���Ϸ�
		if (jewel->getType() == m_JewelsBox[x-1][y]->getType() && 
			jewel->getType() == m_JewelsBox[x-2][y]->getType()
			)
		{
			isXLegal = false;
		}
		else
			isXLegal = true;
	}
	
	if (y > 1)
	{
		//��y�Ḻ����ֱ�Ƚ���λ�������ʯ���Ͷ�һ������ô��ʾ�������ñ�ʯ���Ϸ�
		if (jewel->getType() == m_JewelsBox[x][y-1]->getType() && 
			jewel->getType() == m_JewelsBox[x][y-2]->getType())
		{
			isYLegal = false;
		}
		else
			isYLegal = true;
	}

	return isXLegal && isYLegal;
}

void JewelsGrid::setJewelPixPos(Jewel* jewel, float x, float y)
{
	jewel->setPosition(x * GRID_WIDTH, y * GRID_WIDTH); 
}

bool JewelsGrid::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	//�������������ת��Ϊģ������
	auto pos = this->convertToNodeSpace(pTouch->getLocation());
	
	//�Ƿ��а��ڱ�ʯ������
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		//�õ���������
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		//�õ���ǰѡ�еı�ʯ
		m_jewelSelected = m_JewelsBox[x][y];

		//log("touch coordinate: x=%d,y=%d jewel's type:%d", x, y, m_jewelSelected->getType());

		return true;
	}
	else
	{
		return false;
	}
}

void JewelsGrid::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	//���û��ѡ��ʯ����ô����
	if (!m_jewelSelected)
	{
		return;
	}

	//��ѡ��ʯ�Ĳ�������
	int startX = m_jewelSelected->getX();
	int startY = m_jewelSelected->getY();

	//������Ĳ�������
	auto pos = this->convertToNodeSpace(pTouch->getLocation());
	int touchX = pos.x / GRID_WIDTH;
	int touchY = pos.y / GRID_WIDTH;

	//��������㲻�ڲ����ڣ����ߴ����㲼���������ѡ��ʯ��������һ������ô����
	if (!Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) || Vec2(touchX, touchY) == Vec2(startX, startY)) 
	{
		return;
	}

	//�ж���ѡ��ʯ�Ĳ��������봥����Ĳ��������Ƿ�ֱ�����һ����λ
	if (abs(startX - touchX) + abs(startY - touchY) != 1)
	{
		//log("touch pos not on border");
		return;
	}

	//���µ����������������ı�ʯ���������н����ı�ʯ
	//ͨ��������������ȡ�������ı�ʯ
	m_jewelSwapped = m_JewelsBox[touchX][touchY];

	//������ʯ����������״̬��׽�������ڽ�����ɺ��ж��Ƿ����������
	swapJewels(m_jewelSelected, m_jewelSwapped);
	schedule(schedule_selector(JewelsGrid::onJewelsSwaping));
}

void JewelsGrid::swapJewels(Jewel *jewelA, Jewel *jewelB)
{
	_eventDispatcher->pauseEventListenersForTarget(this); //������ʼ���رմ�������

	//1.������ʯ�����ڵı�ʯָ��
	//2.������ʯ����
	//3.��ʯ�ƶ����µ�λ��
	auto temp = m_JewelsBox[jewelA->getX()][jewelA->getY()];
	m_JewelsBox[jewelA->getX()][jewelA->getY()] = m_JewelsBox[jewelB->getX()][jewelB->getY()];
	m_JewelsBox[jewelB->getX()][jewelB->getY()] = temp;

	auto tempX = jewelA->getX();
	jewelA->setX(jewelB->getX());
	jewelB->setX(tempX);

	auto tempY = jewelA->getY();
	jewelA->setY(jewelB->getY());
	jewelB->setY(tempY);

	swapJewelToNewPos(jewelA);
	swapJewelToNewPos(jewelB);
}

void JewelsGrid::swapJewelToNewPos(Jewel* jewel)
{
	//�ƶ���ʼ���ñ�ʯ����״̬Ϊ�棬�ƶ�����������Ϊ��
	jewel->setSwapingState(true);
	auto move = MoveTo::create(MOVE_SPEED, Vec2(jewel->getX() * GRID_WIDTH, jewel->getY() * GRID_WIDTH));
	auto call = CallFunc::create([jewel](){
		jewel->setSwapingState(false);
	});
	jewel->runAction(Sequence::create(move, call, nullptr));
}

bool JewelsGrid::canCrush()
{
	//�ú����жϵ�ǰ״̬�ı�ʯ�����Ƿ�������
	//���������ı�ʯ����������ʯ���ӣ��ȴ�����

	int count = 0; //������
	Jewel *JewelBegin = nullptr; //��ʼ�����ı�ʯ
	Jewel *JewelNext = nullptr; //����ʼ��ʯ��ʼ��ǰ�����ı�ʯ

	//����ÿһ��
	for (int x = 0; x < m_col; x++) 
	{
		for (int y = 0; y < m_row - 1;)
		{
			count = 1;
			JewelBegin = m_JewelsBox[x][y];
			JewelNext = m_JewelsBox[x][y + 1];

			 //�����������ͬ����
			while (JewelBegin->getType() == JewelNext->getType())
			{
				count++;
				int nextIndex = y + count;
				if (nextIndex > m_row - 1)
					break;
				JewelNext = m_JewelsBox[x][nextIndex];
			}
			//������������ڵ���3����ô��������Щ��ʯӦ�������������Ǵ���������ʯ����
			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto jewel = m_JewelsBox[x][y+n];
					m_crushJewelBox.pushBack(jewel);
				}
			}
			y += count;
		}
	}

	//����ÿһ�У��߼��ͱ���ÿһ����һ����
	for (int y = 0; y < m_row; y++) 
	{
		for (int x = 0; x < m_col - 1;)
		{
			count = 1;
			JewelBegin = m_JewelsBox[x][y];
			JewelNext = m_JewelsBox[x+1][y];

			while (JewelBegin->getType() == JewelNext->getType())
			{
				count++;
				int nextIndex = x + count;
				if (nextIndex > m_col - 1)
					break;
				JewelNext = m_JewelsBox[nextIndex][y];
			}
			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto jewel = m_JewelsBox[x+n][y];
					//�п����б�ʯͬʱ���п���������ô�����ظ��洢��������ʯ���ӣ��������һ���ж�
					if (m_crushJewelBox.find(jewel) != m_crushJewelBox.end())
					{
						continue;
					}
					m_crushJewelBox.pushBack(jewel);
				}
			}
			x += count;
		}
	}	

	//���������ʯ���Ӳ�Ϊ�գ���ô˵�������п�������������
	if (!m_crushJewelBox.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void JewelsGrid::goCrush()
{
	//����������ʯ���ӣ������еı�ʯ������������
	for (auto jewel : m_crushJewelBox)
	{
		//�����µı�ʯ�������������ʼλ����������һ�е��ϱ�һ�У�������һ��
		auto newJewel = Jewel::createByType(random(FIRST_JEWEL_ID, LAST_JEWEL_ID), jewel->getX(), m_row); 
		setJewelPixPos(newJewel, newJewel->getX(), m_row); 
		addChild(newJewel);

		//���±�ʯ�ŵ��±�ʯ�����ڣ��ȴ����벼��
		m_newJewelBox.pushBack(newJewel);

		//��ʯ������Ӧ��ˢ�µı�ʯ��ʱ��Ϊ��
		m_JewelsBox[jewel->getX()][jewel->getY()] = nullptr;

		//ԭ�б�ʯ��������
		jewel->crush();
	}
}

void JewelsGrid::refreshJewelsGrid()
{
	//�����У���������п�λ����ôӦ��ˢ��
	for (int x = 0; x < m_col; x++)
	{
		int empty_count = 0; //һ���ܵĿո�����

		for (int y = 0; y < m_row; y++)
		{
			//��������������ʯ�����ڵı�ʯָ�룬���Ϊ�գ���ô˵��������λ��Ϊ��
			auto jewel = m_JewelsBox[x][y];
			if (!jewel)
				empty_count++;
		}
		if (empty_count)
		{
			//log("the %d col has %d empty", x, empty_count);
			//�ҵ��п�λ���У�ˢ�¸��еı�ʯ
			refreshJewelsToNewPos(x);
		}
	}
}

void JewelsGrid::refreshJewelsToNewPos(int col)
{
	//ˢ�¸�������ı�ʯ
	int n = 0; //��ǰ�������Ŀ�λ��
	auto pJewelsbox = &m_JewelsBox; //����һ����ʯ���ӵ�ָ�룬����Ϊ�������ܴ���lamda

	//�������еı�ʯ����
	for (int y = 0; y < m_row; y++)
	{
		auto jewel = m_JewelsBox[col][y];

		if (!jewel)
		{
			n++;
			continue;
		}
		else if (n != 0)
		{
			jewel->setY(jewel->getY() - n);
			auto move = MoveBy::create(0.2, Vec2(0, -n*GRID_WIDTH));
			auto call = CallFunc::create([pJewelsbox, jewel](){
				//���±�ʯ�����ڵ�����
				(*pJewelsbox)[jewel->getX()][jewel->getY()] = jewel;
			});

			jewel->runAction(Sequence::create(move, call, nullptr));
		}
	}
	
	//�����±�ʯ����
	int i = n;
	int delta = 1;

	for (auto jewel : m_newJewelBox)
	{
		if (jewel->getX() == col)
		{
			jewel->setY(m_row - i);

			auto delay = DelayTime::create(0.2);
			//��������ٶ�������һЩ
			auto move = MoveBy::create(0.2*delta++, Vec2(0, -i--*GRID_WIDTH));
			auto call = CallFunc::create([jewel, pJewelsbox, this](){
				(*pJewelsbox)[jewel->getX()][jewel->getY()] = jewel;
				//���±�ʯ�������Ƴ��ñ�ʯ
				m_newJewelBox.eraseObject(jewel);
			});

			jewel->runAction(Sequence::create(delay, move, call, nullptr));
		}
	}
}

void JewelsGrid::onJewelsSwaping(float dt)
{
	//��׽�������ڽ����ı�ʯ�Ľ��������Ƿ��Ѿ�ֹͣ�����ûֹͣ�����ؼ�����׽
	if (m_jewelSelected->isSwaping() || m_jewelSwapped->isSwaping())
	{
		return;
	}
	//�����ʯ��������ִ�����
	else
	{
		unschedule(schedule_selector(JewelsGrid::onJewelsSwaping));  //ֹͣ��׽

		log("swap over!");

		log("is it can crush?");
		
		//�ж��Ƿ�ǰ״̬��������
		if (canCrush())
		{
			log("yes,crush!");

			m_jewelSelected = nullptr;

			//��ʼ��������������״̬��׽����(��׽��������Ϻ�ˢ�²���)����һ��������ʽ��ʼ
			SimpleAudioEngine::getInstance()->playEffect("crush.ogg");
			goCrush();
			schedule(schedule_selector(JewelsGrid::onJewelsCrushing));
		}
		else
		{
			log("no, cant crush!");

			//����������������ȥ��������������ʱ�Ĳ�׽����(��׽��������Ϻ󣬿�����������)
			SimpleAudioEngine::getInstance()->playEffect("swapback.ogg");
			swapJewels(m_jewelSelected, m_jewelSwapped);
			schedule(schedule_selector(JewelsGrid::onJewelsSwapingBack));
		}
	}
}

void JewelsGrid::onJewelsSwapingBack(float dt)
{
	//��׽�������ڽ����ı�ʯ�Ľ��������Ƿ��Ѿ�ֹͣ�����ûֹͣ�����ؼ�����׽
	if (m_jewelSelected->isSwaping() || m_jewelSwapped->isSwaping())
	{
		return;
	}
	else
	{
		unschedule(schedule_selector(JewelsGrid::onJewelsSwapingBack)); //ֹͣ��׽

		log("swap back!");

		m_jewelSelected = nullptr;

		_eventDispatcher->resumeEventListenersForTarget(this); //���¿�ʼ��������
	}
}

void JewelsGrid::onJewelsCrushing(float dt)
{
	//��׽��ʯ����״̬������б�ʯ������������ô������׽
	for (auto jewel : m_crushJewelBox)
	{
		if (jewel->isCrushing())
		{
			//log("crushing");
			return;
		}
	}
	
	//���ȫ����ʯ�Ѿ�������ϣ�ֹͣ��׽����
	unschedule(schedule_selector(JewelsGrid::onJewelsCrushing));

	m_crushJewelBox.clear(); //���������ʯ����
	
	log("crush over!");
	log("begin to refresh!");

	//ˢ�±�ʯ���У�������ˢ��״̬��׽������ˢ��һ������������ж��������Ƿ��������
	refreshJewelsGrid();
	schedule(schedule_selector(JewelsGrid::onJewelsRefreshing));
}

void JewelsGrid::onJewelsRefreshing(float dt)
{
	//��׽��ʯˢ��״̬������±�ʯ���ӻ��б�ʯ�����±�ʯ����ˢ�µ��У�����ô������׽
	if (m_newJewelBox.size() != 0)
	{
		//log("refreshing!");
		return;
	}
	else
	{
		unschedule(schedule_selector(JewelsGrid::onJewelsRefreshing));

		log("refresh over!");
		log("and now, is it can crush?");

		if (canCrush())
		{
			log("yes, crush again!");

			//�������������ô��������
			SimpleAudioEngine::getInstance()->playEffect("crush.ogg");
			goCrush();
			schedule(schedule_selector(JewelsGrid::onJewelsCrushing));
		}
		else
		{
			log("no, cant crush! over!");

			//�ж��Ƿ�Ϊ��ͼ������ǣ���ִ��һ�����ֶ�������ʾ�������µ�ͼ
			if (isDeadMap())
			{
				log("cant crush any more, updating a new map!");

				auto winSize = Director::getInstance()->getWinSize();
				auto label = Label::createWithTTF("Cant Crush Any More, Change!", "fonts/Marker Felt.ttf", 24);
				label->setTextColor(Color4B::BLACK);
				label->setPosition(winSize.width / 2, winSize.height / 2);
				label->setOpacity(0);
				this->getParent()->addChild(label);

				//��ʾ���ֵ��뵭���󣬸��µ�ͼ���ٿ�����������
				auto fadein = FadeIn::create(0.5);
				auto fadeout = FadeOut::create(0.5);

				auto call = CallFunc::create([this, label](){
					do
					{
						updateMap();
					} while (isDeadMap());

					label->removeFromParent();

					_eventDispatcher->resumeEventListenersForTarget(this);
				});

				label->runAction(Sequence::create(fadein, DelayTime::create(2), fadeout, call, nullptr));
			}
			else
			{
				//���������ͼ����ô��ֱ�ӿ��������������ȴ���һ�ֵĽ�������
				_eventDispatcher->resumeEventListenersForTarget(this);
			}
		}
	}
}