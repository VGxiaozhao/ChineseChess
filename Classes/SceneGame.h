#ifndef _SceneGame_H_
#define _SceneGame_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Stone.h"
#include "Brain.h"
USING_NS_CC;
using namespace CocosDenshion;

//����һ���� 
//��¼ÿ��һ�������Ϣ 
class Step : public Object
{
public:
    int _moveid;//��Ҫ�ƶ������ӵ�id 
    int _killid;//ͨ���������λ���жϴ��������Ƿ������� 
    int _xFrom; //���ӵ�ǰ��λ�õ�x���� 
    int _yFrom; //���ӵ�ǰ��λ�õ�y���� 
    int _xTo;   //�����ƶ����λ�õ�x���� 
    int _yTo;   //�����ƶ����λ�õ�y���� 
    
    static Step* create(int moveid, int killid, int xFrom, int yFrom, int xTo, int yTo)
    {
        Step* step = new Step;
        step->_moveid = moveid;
        step->_killid = killid;
        step->_xFrom = xFrom;
        step->_yFrom = yFrom;
        step->_xTo = xTo;
        step->_yTo = yTo;
        step->autorelease();

        return step;
    }
};

class SceneGame : public Layer
{
public:
     SceneGame();
    ~SceneGame()
    {
        _steps->release();
    }
    static cocos2d::Scene* createScene(bool red);
    //�Զ���init���� 
    virtual bool init(bool red);
    //�Զ���create���� 
    //red�����ж���ҽ�����Ϸʱʱѡ�е����ӵ���ɫ 
    //�����ѡ�к���ʱ����ҳֺ��� 
    //�����ѡ�к���ʱ����ҳֺ��� 
    static SceneGame*  create(bool red);
	//����
	CBrain _brain;
    //���̵�ƫ���� 
    Point _plateOffset;
    //���ӵ�ƫ���� 
    Point _stoneOffset; 
    //���ӵ�ֱ�� 
    float _d;
    //����Ƿ��ߺ��� 
    bool _redTurn;
    //������ҵ������Ǻ�ɫ���Ǻ�ɫ 
    bool _redSide;
    //���ڴ���ѡ���(��������) 
    Sprite* _selectSprite;
    //����ѡ�е����ӵ�id 
    int _selectid;
    //����ÿ���ߵ����� 
    Array* _steps;
    //�������Ӷ�������(������һ����32������) 
    Stone* _s[32];
    //���ڴ���������ʾ��Ϸ��� 
    Sprite* sprite;
    //�ж���Ϸ�������ʾ״̬ 
    bool visible;
    //���ð���ʱ,���ӵ�λ�� 
    void SetRealPos(Stone* s);
    //�õ�������������ϵ������ 
    //���������λ���������ⷵ��false 
    //ͨ�������������������� 
    bool getClickPos(Point ptInWin, int &x, int &y);
    //ͨ��������±��ȡ���ӵ�ID 
    int getStone(int x, int y);
    //ѡ������ 
    void setSelectId(int id);
    //�ƶ����� 
    //��һ���������ƶ������� 
    //�ڶ�����������ɱ�������� 
    void moveStone(int moveId, int killId, int x, int y);
	void moveStone(Move);
    //�����̵�����ת���ɴ��ڵ����� 
    Point getStonePos(int x, int y);
    //����(xo,yo)��(x,y)֮��������� 
    //���������Ϊ-1,��ʾ(xo,yo)��(x,y)����һ��ֱ���� 
    int getStoneCount(int xo, int yo, int x, int y);
    //�¾� 
    void New(Object*);
    //���� 
    void Back(Object*);
    //��ʼ��Ϸ 
    void Start(Object*);
    //��ͣ��Ϸ 
    void Pause(Object*);
    //������Ϸ�Ѷ� 
    void Difficulty(Object*);
    //���ű������� 
    void Voice(Object*);
    void moveComplete(Node*, void*);
	//�����˹���������
	void callAI();
};

#endif // SCENEGAME_H
