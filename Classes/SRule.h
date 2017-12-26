#pragma once
#include "Stone.h"
#include <list>
using namespace std;
class SRule
{
public:
	SRule();
	virtual ~SRule();
	static bool canMove(int moveid, int killid, int x, int y, Stone* s[], bool redSide);
	static bool canMoveJiang(int moveid, int killid, int x, int y, Stone* s[], bool redSide);
	static bool canMoveShi(int moveid, int x, int y, Stone* s[], bool redSide);
	static bool canMoveXiang(int moveid, int x, int y, Stone* s[], bool redSide);
	static bool canMoveJu(int moveid, int x, int y, Stone* s[], bool redSide);
	static bool canMoveMa(int moveid, int x, int y, Stone* s[], bool redSide);
	static bool canMovePao(int moveid, int killid, int x, int y, Stone* s[], bool redSide);
	static bool canMoveBing(int moveid, int x, int y, Stone* s[], bool redSide);
	static int  getStone(int x, int y, Stone* s[], bool redSide);
	static int  getStoneCount(int xo, int yo, int x, int y, Stone* s[], bool redSide);
public:
	static list<Move> listMove(int mid, Stone*s[], bool redSide);
	static list<Move> listMoveJiang(int mid, Stone*s[], bool redSide);
	static list<Move> listMoveShi(int mid, Stone*s[], bool redSide);
	static list<Move> listMovePao(int mid, Stone*s[], bool redSide);
	static list<Move> listMoveMa(int mid, Stone*s[], bool redSide);
	static list<Move> listMoveXiang(int mid, Stone*s[], bool redSide);
};

