#ifndef _SceneGame_H_
#define _SceneGame_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Stone.h"
#include "Brain.h"
USING_NS_CC;
using namespace CocosDenshion;

//创建一个类 
//记录每走一步棋的信息 
class Step : public Object
{
public:
    int _moveid;//需要移动的棋子的id 
    int _killid;//通过触摸点的位置判断触摸点上是否有棋子 
    int _xFrom; //棋子当前的位置的x坐标 
    int _yFrom; //棋子当前的位置的y坐标 
    int _xTo;   //棋子移动后的位置的x坐标 
    int _yTo;   //棋子移动后的位置的y坐标 
    
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
    //自定义init函数 
    virtual bool init(bool red);
    //自定义create函数 
    //red用于判断玩家进入游戏时时选中的棋子的颜色 
    //当玩家选中红棋时，玩家持红棋 
    //当玩家选中黑棋时，玩家持黑棋 
    static SceneGame*  create(bool red);
	//大脑
	CBrain _brain;
    //棋盘的偏移量 
    Point _plateOffset;
    //棋子的偏移量 
    Point _stoneOffset; 
    //棋子的直径 
    float _d;
    //标记是否走红棋 
    bool _redTurn;
    //设置玩家的棋子是红色还是黑色 
    bool _redSide;
    //用于创建选择框(创建精灵) 
    Sprite* _selectSprite;
    //保存选中的棋子的id 
    int _selectid;
    //保存每步走的棋子 
    Array* _steps;
    //定义棋子对象数组(象棋中一共有32颗棋子) 
    Stone* _s[32];
    //用于创建精灵显示游戏结果 
    Sprite* sprite;
    //判断游戏结果的显示状态 
    bool visible;
    //设置摆棋时,棋子的位置 
    void SetRealPos(Stone* s);
    //得到鼠标点击在棋盘上的坐标点 
    //当鼠标点击的位置在棋盘外返回false 
    //通过窗口坐标获得棋盘坐标 
    bool getClickPos(Point ptInWin, int &x, int &y);
    //通过坐标的下标获取棋子的ID 
    int getStone(int x, int y);
    //选择棋子 
    void setSelectId(int id);
    //移动棋子 
    //第一个参数：移动的棋子 
    //第二个参数：被杀掉的棋子 
    void moveStone(int moveId, int killId, int x, int y);
	//记录棋谱
	void record();
	void moveStone(Move);
    //将棋盘的坐标转换成窗口的坐标 
    Point getStonePos(int x, int y);
    //新局 
    void New(Object*);
    //悔棋 
    void Back(Object*);
    //开始游戏 
    void Start(Object*);
    void moveComplete(Node*, void*);
	//测试人工智障下棋
	void callAI();
};

#endif // SCENEGAME_H
