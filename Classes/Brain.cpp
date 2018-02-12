#include "Brain.h"
#include "SRule.h"
#include <cstring>
#include <algorithm>
CBrain::CBrain(bool red, Stone* s[32])
{
	init();
	_turn = red;
	_board = new CBoard(s, red);
	setPad(s, red);
}

CBrain::CBrain()
{ 
	init();
}

void CBrain::setPad(Stone* s[32], bool turn)
{
	_turn = turn;
	if (_board == NULL)
		_board = new CBoard(s, turn);
	else
		_board->setBoard(s, turn);
}


CBrain::~CBrain()
{
	for (int i = 0; i <= _depth; i++)
		delete mp[i];
	delete _board;
}

//Move CBrain::int2move(int a)
//{
//	Move ret;
//	ret.moveid = a / 90 + 16;
//	ret.x = 8 - ((a % 90) % 9);
//	ret.y = 9 - ((a % 90) / 9);
//	ret.killid = -1;
//	if (ret.x == _s[ret.moveid]->getX() && ret.y == _s[ret.moveid]->getY())
//	{
//		ret.x = ret.y = 9;
//		return ret;
//	}
//	for (int i = 0; i < 32; i++)
//	{
//		if (i != ret.moveid && ret.x == _s[i]->getX() && ret.y == _s[i]->getY())
//			ret.killid = i;
//	}
//	return ret;
//}

void CBrain::disorder()
{
	for (int i = 0; i < 32; i++)
	{
		int x = rand() % 32;
		std::swap(_xulie[i], _xulie[x]);
	}
}

Move CBrain::think()
{
	alphaBetaSearch();
	return _bestMove;
}

void CBrain::init()
{
	for (int i = 0; i < 32; i++)
		_xulie[i] = i;
	_depth = 2;
	for (int i = 0; i <= _depth; i++)
		mp[i] = new CHashMap(1e6);
	_board = NULL;
}

void  CBrain::alphaBetaSearch()
{
	disorder();
	for (int i = 0; i <= _depth; i++)
		mp[i]->clear();
	_firstScore = _board->evaluate(_turn);
	_hashcnt = _searchCount = 0;
	int val = -1111;
	val = alphaBetaSearch(_turn, _depth, -500, 500);
	log("\n\nval: %d, cnt: %d, hash: %d", val, _searchCount, _hashcnt);
	log("mid: %d, kid: %d, x: %d, y: %d", _bestMove.moveid, _bestMove.killid, _bestMove.x, _bestMove.y);
}

int  CBrain::alphaBetaSearch(bool turn, int depth, int alpha, int beta)
{
	_searchCount++;
	if (_board->isGameOver())
	{
		return _board->evaluate(_depth + depth, _firstScore, _turn);
	}
	if (!depth)
	{
		return quies(turn, _depth + depth, alpha, beta);
	}
	for (int j = 0; j < 32; j++)
	{
		int i = _xulie[j];
		if (_board->_s[i].getRed() != turn || _board->_s[i].getDead() ) continue;
		//list<Move> tmp = SRule::listMove(i, _board->_s);
		list<Move> tmp = _board->listMove(i);
		for (auto m : tmp)
		{
			_board->moveStone(m);
			int val = -alphaBetaSearch(!turn, depth - 1, -beta, -alpha);
			_board->reverseMove();
			if (val > alpha){
				alpha = val;
				if (depth == _depth)
					_bestMove = m;
			}
			//if (alpha >= beta) break;
			if (val >= beta)
			{
				return beta;
			}
		}
	}
	return alpha;
}


int  CBrain::quies(bool turn, int depth, int alpha, int beta)
{
	_searchCount++;
	int tmp = _board->evaluate(depth, _firstScore, _turn);
	if (tmp >= beta){
		return beta;
	}
	if (tmp > alpha){
		alpha = tmp;
	}
	for (int j = 0; j < 32; j++)
	{
		int i = _xulie[j];
		if (_board->_s[i].getRed() != turn || _board->_s[i].getDead()) continue;
		//list<Move> tmp = SRule::listMove(i, _board->_s);
		list<Move> tmp = _board->listMove(i);
		for (auto m : tmp)
		{
			if (m.killid == -1) break;
			_board->moveStone(m);
			int val = -quies(!turn, depth + 1, -beta, -alpha);
			_board->reverseMove();
			if (val >= beta)
			{
				return beta;
			}
			if (val > alpha){
				alpha = val;
			}
		}
	}
	return alpha;
}