#pragma once
#include "Stone.h"
#include <string>
#include <list>
#include <stack>
#include <string.h>

struct stChessman
{
	int _x, _y;
	void setX(int x) { _x = x; }
	int getX(){ return _x; }
	void setY(int y) { _y = y; }
	int getY() { return _y; }
	Stone::TYPE _type;
	int getType(){ return _type; }
	void setType(Stone::TYPE x){ _type = x; }
	bool _dead;
	bool getDead(){ return _dead; }
	void setDead(bool x) { _dead = x;}
	bool _red;
	bool getRed(){ return _red; }
	void setRed(bool b){ _red = b; }
	stChessman(){
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
			ret = 1000;
		else if (_type == Stone::BING)
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
			ret = 4;
		return ret;
	}
};


class CBoard
{
public:
	CBoard(Stone* s[], bool);
	CBoard();
	~CBoard();
	void setBoard(Stone* s[], bool);
	void setBoard(const stChessman s[]);
	CBoard& operator=(const CBoard& cmp);
	void moveStone(Move m);
	void reverseMove();
	Move makeRevMove(Move m);
	std::string getJumianStr();
	unsigned long long getJumianLL();
	int evaluate(int, int, bool);
	int evaluate(bool);
	bool isGameOver();
	void setHas();
	bool isOnCheck(bool turn);
	stChessman _s[32];
public:
	bool isEnemy(int a, int b);
	bool isGoOut(int mid, int x, int y);
	bool getJiangDire(int mid);
	std::list<Move> listMove(int mid);
	std::list<Move> listMoveBing(int mid);
	std::list<Move> listMoveShi(int mid);
	std::list<Move> listMovePao(int mid);
	void listMovePao(int mid, int x, int y, int dx, int dy, bool yue, std::list<Move>& lst);
	std::list<Move> listMoveMa(int mid);
	std::list<Move> listMoveJu(int mid);
	void listMoveJu(int mid, int x, int y, int dx, int dy, std::list<Move>& lst);
	std::list<Move> listMoveJiang(int mid);
	std::list<Move> listMoveXiang(int mid);
private:
	std::stack<int> _x0, _y0, _revive, _id;
	int has[10][10];
	bool _turn;
	int _firstScore;
	void init();
};

