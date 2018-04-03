#include "Brain.h"
#include "SRule.h"
#include "MonteCarlo.h"
#include <cstring>
#include <algorithm>

CBrain::CBrain(bool red, Stone* s[32])
{
	init();
	_turn = red;
	_board = new CBoard(s, red);
	setPad(s, red);
}

CBrain::CBrain(CBoard board)
{
	init();
	_board = new CBoard(board);
	_turn = _board->getTurn();
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
	delete _board;
}

void CBrain::disorder()
{
	for (int i = 0; i < 32; i++)
	{
		int x = rand() % 32;
		std::swap(_xulie[i], _xulie[x]);
	}
}

void CBrain::init()
{
	for (int i = 0; i < 32; i++)
		_xulie[i] = i;
	_depth = 2;
	_board = NULL;
}

Move CBrain::getNNMove()
{
	return _board->getNNMove();
}

Move CBrain::think()
{
	//调试阶段修改这里的参数
	//alphaBetaSearch(3);
	//alphaBetaSearchWithSL(3);
	//蒙树
	MonteCarlo mc(*_board, _turn);
	_bestMove = mc.UCTSearch();
	//监督学习
	//_bestMove = _board->getNNMove();
	return _bestMove;
}

Move CBrain::alphaBetaSearch(int depth)
{
	disorder();
	_bestMove.zero();
	int val = stChessman::JIANGVAL/2;
	int win = 2;
	for (int i = 1; i <= depth; i++)
	{
		_depth = i;
		_firstScore = _board->initEvaluate();
		_hashcnt = _searchCount = 0;
		if (i==1)
			val = alphaBetaSearch(_turn, _depth, -stChessman::INFVAL, stChessman::INFVAL);
		else
			val = alphaBetaSearch(_turn, _depth, val-win, val+win);
		log("\n\ndepth: %d, val: %d, cnt: %d, hash: %d", i, val, _searchCount, _hashcnt);
		log("mid: %d, kid: %d, x: %d, y: %d", _bestMove.moveid, _bestMove.killid, _bestMove.x, _bestMove.y);
	}
	return _bestMove;
}

int  CBrain::alphaBetaSearch(bool turn, int depth, int alpha, int beta)
{
	_searchCount++;
	if (_board->isGameOver())
	{
		return _board->evaluate(turn);
	}
	if (!depth)
	{
		return quies(turn, _depth + depth, alpha, beta);
	}
	//顶层，使用上一次迭代的最优方案
	if (depth == _depth && !_bestMove.isZero())
	{
		_board->moveStone(_bestMove);
		int val = -alphaBetaSearch(!turn, depth - 1, -beta, -alpha);
		_board->reverseMove();
		if (val > alpha){
			alpha = val;
			if (depth == _depth)
				_bestMove = _bestMove;
		}
		//if (alpha >= beta) break;
		if (val >= beta)
		{
			return beta;
		}
	}
	for (int j = 0; j < 32; j++)
	{
		int i = _xulie[j];
		if (_board->_s[i].getRed() != turn || _board->_s[i].getDead() ) continue;
		//list<Move> tmp = SRule::listMove(i, _board->_s);
		auto tmp = _board->listMove(i);
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

Move CBrain::alphaBetaSearchWithSL(int depth)
{
	_bestMove.zero();
	int val = stChessman::JIANGVAL / 2;
	_firstScore = _board->initEvaluate();
	_hashcnt = _searchCount = 0;
	_depth = depth;
	val = alphaBetaSearchWithSL(_turn, depth, -val, val);
	log("\n\nval: %d, cnt: %d, hash: %d", val, _searchCount, _hashcnt);
	log("mid: %d, kid: %d, x: %d, y: %d", _bestMove.moveid, _bestMove.killid, _bestMove.x, _bestMove.y);
	return _bestMove;
}

int  CBrain::alphaBetaSearchWithSL(bool turn, int depth, int alpha, int beta)
{
	_searchCount++;
	if (_board->isGameOver())
	{
		return _board->evaluate(turn);
	}
	if (!depth)
	{
		return quies(turn, _depth + depth, alpha, beta);
	}

	std::vector<Move> vec;
	if (depth == _depth)
		vec = _board->listAllMove();
	else
		vec = _board->get10NNMove();
	for (auto m : vec)
	{
		_board->moveStone(m);
		int val = -alphaBetaSearch(!turn, depth - 1, -beta, -alpha);
		_board->reverseMove();
		if (val > alpha) {
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
	return alpha;
}

int  CBrain::quies(bool turn, int depth, int alpha, int beta)
{
	_hashcnt++;
	int tmp = _board->evaluate(turn);
	if (tmp >= beta){
		return beta;
	}
	if (tmp > alpha){
		alpha = tmp;
	}
	auto allmove = _board->listKillMove();
	for (auto m : allmove)
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
	return alpha;
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