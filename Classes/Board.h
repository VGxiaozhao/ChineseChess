#pragma once
#include "Stone.h"
#include <string>
#include <list>
#include <stack>
#include <vector>
#include <string.h>



static const int position_score[7][10][9] = {
	{//Bing
		{ 9,  9,  9, 11, 13, 11,  9,  9,  9 },
		{ 29, 39, 59, 74, 79, 74, 59, 39, 29 },
		{ 29, 39, 54, 64, 64, 64, 54, 39, 29 },
		{ 29, 36, 39, 49, 51, 49, 39, 36, 29 },
		{ 19, 27, 31, 44, 49, 44, 31, 27, 19 },
		{ 7,  0, 13,  0, 16,  0, 13,  0,  7 },
		{ 7,  0,  7,  0, 15,  0,  7,  0,  7 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 }
	},
	{//Shi
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  20, 0, 20,  0,  0,  0 },
		{ 0,  0,  0,  0, 23,  0,  0,  0,  0 },
		{ 0,  0,  0, 20,  0, 20,  0,  0,  0 }
	},
	{//Xiang
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0, 20,  0,  0,  0, 20,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 18, 0,  0,  0, 23,  0,  0, 0,  18 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0, 20,  0,  0,  0, 20,  0,  0 }
	},
	{//Ma
		{ 90, 90, 90, 96, 90, 96, 90, 90, 90 },
		{ 90, 96,103, 97, 94, 97,103, 96, 90 },
		{ 92, 98, 99,103, 99,103, 99, 98, 92 },
		{ 93,108,100,107,100,107,100,108, 93 },
		{ 90,100, 99,103,104,103, 99,100, 90 },
		{ 90, 98,101,102,103,102,101, 98, 90 },
		{ 92, 94, 98, 95, 98, 95, 98, 94, 92 },
		{ 93, 92, 94, 95, 92, 95, 94, 92, 93 },
		{ 85, 90, 92, 93, 78, 93, 92, 90, 85 },
		{ 88, 85, 90, 88, 90, 88, 90, 85, 88 }
	},
	{//Pao
		{ 100,100, 96, 91, 90, 91, 96,100,100 },
		{ 98, 98, 96, 92, 89, 92, 96, 98, 98 },
		{ 97, 97, 96, 91, 92, 91, 96, 97, 97 },
		{ 96, 99, 99, 98,100, 98, 99, 99, 96 },
		{ 96, 96, 96, 96,100, 96, 96, 96, 96 },
		{ 95, 96, 99, 96,100, 96, 99, 96, 95 },
		{ 96, 96, 96, 96, 96, 96, 96, 96, 96 },
		{ 97, 96,100, 99,101, 99,100, 96, 97 },
		{ 96, 97, 98, 98, 98, 98, 98, 97, 96 },
		{ 96, 96, 97, 99, 99, 99, 97, 96, 96 }
	},
	{//Ju
		{ 206,208,207,213,214,213,207,208,206 },
		{ 206,212,209,216,233,216,209,212,206 },
		{ 206,208,207,214,216,214,207,208,206 },
		{ 206,213,213,216,216,216,213,213,206 },
		{ 208,211,211,214,215,214,211,211,208 },
		{ 208,212,212,214,215,214,212,212,208 },
		{ 204,209,204,212,214,212,204,209,204 },
		{ 198,208,204,212,212,212,204,208,198 },
		{ 200,208,206,212,200,212,206,208,200 },
		{ 194,206,204,212,200,212,204,206,194 }
	},
	{//Jiang
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  0,  0,  0,  0,  0,  0 },
		{ 0,  0,  0,  1,  1,  1,  0,  0,  0 },
		{ 0,  0,  0,  2,  2,  2,  0,  0,  0 },
		{ 0,  0,  0, 11, 15, 11,  0,  0,  0 }
	}
};

struct stChessman
{
	static const int JIANGVAL = 100000;
	static const int INFVAL = 50000;
	int _x, _y;
	void setX(int x) { _x = x; }
	int getX() { return _x; }
	void setY(int y) { _y = y; }
	int getY() { return _y; }
	Stone::TYPE _type;
	int getType() { return _type; }
	void setType(Stone::TYPE x) { _type = x; }
	bool _dead;
	bool getDead() { return _dead; }
	void setDead(bool x) { _dead = x; }
	bool _red;
	bool getRed() { return _red; }
	void setRed(bool b) { _red = b; }
	stChessman() {
		_x = _y = 0;
		_dead = _red = false;
	}
	void operator=(const Stone& cmp)
	{
		_x = cmp.getX();
		_y = cmp.getY();
		_type = cmp.getType();
		_dead = cmp.getDead();
		_red = cmp.getRed();
	}
	int getScore()
	{
		int ret = 0;
		if (_type == Stone::JIANG)
			ret = JIANGVAL;
		/*else if (_type == Stone::BING)
		ret = 2;
		else if (_type == Stone::JU)
		ret = 18;
		else if (_type == Stone::PAO)
		ret = 9;
		else if (_type == Stone::MA)
		ret = 8;
		else if (_type == Stone::SHI)
		ret = 4;
		else if (_type == Stone::XIANG)
		ret = 4;*/
		int x = _y;
		int y = _x;
		if (_red)
		{
			x = 9 - x;
		}
		return ret + ::position_score[_type][x][y];
	}
};

///棋盘类，具有模拟移动棋子，和生成着法的功能
class CBoard
{
public:
	//用于交互python的相关文件
	static char* TXTBOARD;
	static char* TAGBOARD;
	static char* TXTMOVE;
	static char* TAGMOVE;
public:
	CBoard(Stone* s[], bool);
	CBoard(const CBoard& C);
	CBoard();
	~CBoard();
	void setBoard(Stone* s[], bool);
	void setBoard(const stChessman s[]);
	bool getTurn();
	CBoard& operator=(const CBoard& cmp);
	void moveStone(Move m);
	void reverseMove();
	std::string getJumianStr();
	unsigned long long getJumianLL();
	int evaluate(bool);
	int getScore(bool);
	int initEvaluate();
	//游戏结束
	bool isGameOver();
	//设置棋盘表，标记某个格子是否有棋子，对应他的id
	void setHas();
	//是否被将
	bool isOnCheck(bool turn);
	std::string  toString();
	//获得神经网络推荐着法
	Move getNNMove();
	std::vector<Move> get10NNMove();
	std::vector<double> getNNMovePossi();
	//将神经网络推荐着法返回的数字转为Move类型
	Move intToMove(int tmp);
public:
	//对应两个id是否敌人
	bool isEnemy(int a, int b);
	//是否越界，过河
	bool isGoOut(int mid, int x, int y);
	//获得自己老大的位置是不是在y轴小的方向
	bool getJiangDire(int mid);
	//列举所有着法（无排序）
	std::list<Move> listMove(int mid);
	//单列吃子着法
	std::vector<Move> listKillMove();
	//列举防止被杀将着法（杀手启发用）
	std::vector<Move> listDefendMove();
	//列举所有着法（有排序）
	std::vector<Move> listAllMove();
	static bool cmp(Move a, Move b);
	//是否有将军棋
	bool isMoveContainJiang(std::vector<Move> mvlst);
private: 
	std::list<Move> listMoveBing(int mid);
	std::list<Move> listMoveShi(int mid);
	std::list<Move> listMovePao(int mid);
	void listMovePao(int mid, int x, int y, int dx, int dy, bool yue, std::list<Move>& lst);
	std::list<Move> listMoveMa(int mid);
	std::list<Move> listMoveJu(int mid);
	void listMoveJu(int mid, int x, int y, int dx, int dy, std::list<Move>& lst);
	std::list<Move> listMoveJiang(int mid);
	std::list<Move> listMoveXiang(int mid);
	//栈用于撤销走法
	std::stack<int> _x0, _y0, _revive, _id;
	//记录棋盘上的点是哪个棋子
	int has[10][10];
	bool _turn;
	int _firstScore;
	void init();
public:
	stChessman _s[32];
};

