#include "Brain.h"
#include "SRule.h"
#include <cstring>
#include <algorithm>

CBrain::CBrain(bool red, Stone* s[32])
{
	for (int i = 0; i < 32; i++)
		_s[i] = s[i];
	_turn = red;
	for (int i = 0; i < 32; i++)
		_xulie[i] = i;
	disorder();
}


CBrain::~CBrain()
{

}

Move CBrain::think()
{
	evaluate(_myturn, _yourturn);
	_depth = 5;
	int val = alphaBetaSearch(_turn, _depth, -500, 500);
	log("val: %d", val);
	log("mid: %d, kid: %d, x: %d, y: %d\n", _bestMove.moveid, _bestMove.killid, _bestMove.x, _bestMove.y);
	return _bestMove;
}

int CBrain::getScore(int id)
{
	if (id == -1) return 0;
	int type = _s[id]->getType();
	if (type == Stone::JIANG)
		return 1000;
	else if (type == Stone::BING)
		return 2;
	else if (type == Stone::JU)
		return 18;
	else if (type == Stone::PAO)
		return 9;
	else if (type == Stone::MA)
		return 8;
	else if (type == Stone::SHI)
		return 4;
	else if (type == Stone::XIANG)
		return 4;
}

void CBrain::moveStone(Move m)
{
	_s[m.moveid]->setX(m.x);
	_s[m.moveid]->setY(m.y);
	if (m.killid!=-1)
		_s[m.killid]->setDead(true);
}

void CBrain::reverseMove(Move m)
{
	_s[m.moveid]->setX(m.x);
	_s[m.moveid]->setY(m.y);
	if (m.killid != -1)
		_s[m.killid]->setDead(false);
}

Move CBrain::makeRevMove(Move m)
{
	Move ret;
	ret = m;
	ret.x = _s[m.moveid]->getX();
	ret.y = _s[m.moveid]->getY();
	return ret;
}

void CBrain::disorder()
{
	for (int i = 0; i < 32; i++)
	{
		int x = rand() % 32;
		std::swap(_xulie[i], _xulie[x]);
	}
}

bool CBrain::isGameOver()
{
	for (int i = 0; i < 32; i++)
	{
		if (_s[i]->getType() == Stone::JIANG && _s[i]->getDead() == true)
			return true;
	}
	return false;
}

void CBrain::evaluate(int& myturn, int& yourturn)
{
	myturn = yourturn = 0;
	for (int i = 0; i < 32; i++)
	{
		if (_s[i]->getDead()) continue;
		if (_s[i]->getRed() == _turn)
			myturn += getScore(i);
		else
			yourturn += getScore(i);
	}
}

int  CBrain::alphaBetaSearch(bool turn, int depth, int alpha, int beta)
{
	if (!depth || isGameOver())
	{
		int m, u;
		evaluate(m, u);
		if ((_depth+depth) % 2 == 0)
			return _yourturn - u + m - _myturn;
		else
			return -(_yourturn - u + m - _myturn);
	}
	for (int j = 0; j < 32; j++)
	{
		int i = _xulie[j];
		if (_s[i]->getRed() != turn) continue;;
		list<Move> lst = SRule::listMove(i, _s);
		for (auto m : lst)
		{
			Move rm = makeRevMove(m);
			moveStone(m);
			int val = -alphaBetaSearch(!turn, depth - 1, -beta, -alpha);
			reverseMove(rm);
			if (val > alpha){
				alpha = val;
				if (depth == _depth)
					_bestMove = m;
			}
			//if (alpha >= beta) break;
			if (val >= beta)
				return beta;
		}
	}
	return alpha;
}