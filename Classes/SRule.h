#pragma once
#include "Stone.h"
#include "Board.h"
#include <list>
using namespace std;
class SRule
{
public:
	SRule();
	virtual ~SRule();
	static bool canMove(int moveid, int killid, int x, int y, stChessman* _s);
	static bool canMoveJiang(int moveid, int killid, int x, int y, stChessman* _s);
	static bool canMoveShi(int moveid, int x, int y, stChessman* _s);
	static bool canMoveXiang(int moveid, int x, int y, stChessman* _s);
	static bool canMoveJu(int moveid, int x, int y, stChessman* _s);
	static bool canMoveMa(int moveid, int x, int y, stChessman* _s);
	static bool canMovePao(int moveid, int killid, int x, int y, stChessman* _s );
	static bool canMoveBing(int moveid, int x, int y, stChessman* _s );
	static int  getStone(int x, int y, stChessman* _s );
	static int  getStoneCount(int xo, int yo, int x, int y, stChessman* _s);
	static bool isValidMove(Move e, stChessman* _s );
public:
	static list<Move> listMove(int mid, stChessman* _s );
	static list<Move> listMoveBing(int mid, stChessman* _s );
	static list<Move> listMoveShi(int mid, stChessman* _s );
	static list<Move> listMovePao(int mid, stChessman* _s );
	static list<Move> listMoveMa(int mid, stChessman* _s );
	static list<Move> listMoveXiang(int mid, stChessman* _s );
};

