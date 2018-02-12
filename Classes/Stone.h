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

//棋子类 
class Stone : public Sprite
{
public:
    Stone();
    //棋子的类型:兵、士、相、马、炮、车、 将
    enum TYPE { BING, SHI, XIANG, MA, PAO, JU, JIANG};
	//活动范围
	enum LOC {CENTER, SIDELINE, CONER};
	//局势
	enum SIT {OPEN, MIDDLE, MESS};
    //创建棋子 
    //第一个参数：棋子的类型 
    //第二个参数：棋子的颜色 
    static Stone* create(int id, bool red);
    //初始化棋子 
    bool init(int id, bool red);
    //摆棋子 
    void reset(bool red);
    //保存梅个棋子的初始位置 
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
	//棋子的位置(坐标) 
	CC_SYNTHESIZE(int, _x, X)
	CC_SYNTHESIZE(int, _y, Y)
	//棋子的ID  0~31(一共有32个棋子) 
	CC_SYNTHESIZE(int, _id, ID)
	//判断棋子是否被吃了 
	CC_SYNTHESIZE(bool, _dead, Dead)
	//判断棋子的颜色 
	CC_SYNTHESIZE(bool, _red, Red)
	int getLoc();
private:
	int calculate(Stone::TYPE );
};

#endif
