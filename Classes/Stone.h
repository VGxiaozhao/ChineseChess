#ifndef  _Stone_H_
#define  _Stone_H_

#include "cocos2d.h"
USING_NS_CC;

struct Move{
	Move(){}
	Move(int _moveid, int _killid, int _x, int _y){
		moveid = _moveid;
		killid = _killid;
		x = _x;
		y = _y;
	}
	void operator=(const Move& cmp){
		moveid = cmp.moveid;
		killid = cmp.killid;
		x = cmp.x;
		y = cmp.y;
	}
	bool operator==(const Move& cmp){
		return (moveid == cmp.moveid &&	killid == cmp.killid && x == cmp.x && y == cmp.y);
	}
	void zero(){
		moveid = killid = x = y = 0;
	}
	int moveid, killid, x, y;
};

//������ 
class Stone : public Sprite
{
public:
    Stone();
    //���ӵ�����:����ʿ���ࡢ���ڡ����� ��
    enum TYPE { BING, SHI, XIANG, MA, PAO, JU, JIANG};
	//���Χ
	enum LOC {CENTER, SIDELINE, CONER};
	//����
	enum SIT {OPEN, MIDDLE, MESS};
    //�������� 
    //��һ�����������ӵ����� 
    //�ڶ������������ӵ���ɫ 
    static Stone* create(int id, bool red);
    //��ʼ������ 
    bool init(int id, bool red);
    //������ 
    void reset(bool red);
    //����÷�����ӵĳ�ʼλ�� 
    static struct InitPos
    {
        int _x;
        int _y;
       Stone::TYPE _type;
	}_initPos[16];
	int getScore();
	int _score;
    //
	CC_SYNTHESIZE(TYPE, _type, Type)
	//���ӵ�λ��(����) 
	CC_SYNTHESIZE(int, _x, X)
	CC_SYNTHESIZE(int, _y, Y)
	//���ӵ�ID  0~31(һ����32������) 
	CC_SYNTHESIZE(int, _id, ID)
	//�ж������Ƿ񱻳��� 
	CC_SYNTHESIZE(bool, _dead, Dead)
	//�ж����ӵ���ɫ 
	CC_SYNTHESIZE(bool, _red, Red)
	int getLoc();
private:
	int calculate(Stone::TYPE );
};

#endif
