#include "Brain.h"
#include "SRule.h"
#include <cstring>
#include <algorithm>

CBrain::CBrain(bool red, Stone* s[32], bool redSide)
{
	for (int i = 0; i < 32; i++)
		_s[i] = s[i];
	_turn = red;
	for (int i = 0; i < 32; i++)
		_xulie[i] = i;
	disorder();
	_best = 0;
	_worst = 0;
}


CBrain::~CBrain()
{

}

Move CBrain::think()
{
	return fuck();
	//return Alpah();
	//return testAI();
}

Move CBrain::Alpah()
{
	disorder();
	_best = 0;
	_worst = 0;
	_bestMove.zero();
	for (int x = 0; x < 32; x++)
	{
		int i = _xulie[x];
		if (_s[i]->getRed() == _turn && !_s[i]->getDead())
		{
			list<Move> lst = SRule::listMove(i, _s);
			for (auto m : lst)
			{
				int best = 0, worst = 0;
				bool out = false;
				Move rmove = makeRevMove(m);
				moveStone(m);
				dfs(!_turn, getScore(m.killid), 1, out, best, worst);
				reverseMove(rmove);
				if (/*worst >= _worst&&*/best >= _best)
				{
					_worst = worst;
					_best = best;
					_bestMove = m;
				}
			}
		}
	}
	int id = _bestMove.moveid;
	log("cur x: %d, cur y: %d", _s[id]->getX(), _s[id]->getY());
	log("current mid %d, kid %d, x %d, y %d\n", _bestMove.moveid, _bestMove.killid, _bestMove.x, _bestMove.y);
	log("worst %d, best %d\n", _worst, _best);
	return _bestMove;
}

void CBrain::dfs(bool turn, int sum, int depth, bool& out, int& best, int& worst)
{
	//坏情况剪枝经常发生，应该优化！
	/*if (out)
		return;*/
	if (depth >= 3)
	{
		worst = min(worst, sum);
		best = max(best, sum);
		if (worst < _worst)
			out = true;
		return;
	}
	//disorder();
	for (int x = 0; x < 32; x++)
	{
		int i = _xulie[x];
		if (_s[i]->getRed() == turn && !_s[i]->getDead())
		{
			list<Move> lst = SRule::listMove(i, _s);
			for (auto m : lst)
			{
				Move rmove = makeRevMove(m);
				moveStone(m);
				if (turn == _turn)
					dfs(!turn, sum + getScore(m.killid), depth + 1, out, best, worst);
				else
					dfs(!turn, sum - getScore(m.killid), depth + 1, out, best, worst);
				reverseMove(rmove);
			}
		}
	}
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
	//for (int x = 0; x < 32; x++)
	//{
	//	int i = _xulie[x];
	//	if (_s[i]->getType() == Stone::JIANG)
	//	{
	//		if (_s[i]->getRed() == true)
	//			swap(_xulie[x], _xulie[31]);
	//		else
	//			swap(_xulie[x], _xulie[30]);
	//	}
	//}
}

Move CBrain::testAI()
{
	int mid, x, y, kid;
	int kred, mred;
	do{
		int end = 16, start = 31;
		mid = (int)(CCRANDOM_0_1()*(end - start + 1) + start);
		x = (int)(CCRANDOM_0_1() * 8);
		y = (int)(CCRANDOM_0_1() * 9);
		kid = SRule::getStone(x, y, _s);
		mred = _s[mid]->getRed();
		if (kid != -1)
			kred = _s[kid]->getRed();
		else
			kred = !mred;
	} while (!SRule::canMove(mid, kid, x, y, _s));
	return Move(mid, kid, x, y);
}

Move CBrain::fuck()
{
	_bestMove.zero();
	_depth = 3;
	int alpha = -500;
	disorder();
	evaluate(_myturn, _yourturn);
	for (int x = 0; x < 32; x++)
	{
		int i = _xulie[x];
		//if (i != 23 && i != 10)
		//	continue;
		if (_s[i]->getRed() == _turn && !_s[i]->getDead())
		{
			list<Move> lst = SRule::listMove(i, _s);

			for (auto m : lst)
			{
				Move rmove = makeRevMove(m);
				moveStone(m);
				_best = 0;
				int val = -alphaBetaSearch(!_turn, 3, -500, 500) + getScore(m.killid);
				reverseMove(rmove);
				if (val >= alpha)
				{
					alpha = val;
					_bestMove = m;
				}
			}
		}
	}

	int id = _bestMove.moveid;
	log("alpha: %d", alpha);
	log("cur x: %d, cur y: %d", _s[id]->getX(), _s[id]->getY());
	log("current mid %d, kid %d, x %d, y %d\n", _bestMove.moveid, _bestMove.killid, _bestMove.x, _bestMove.y);
	return _bestMove;
}

int CBrain::alphaBetaSearch(int turn, int depth, int alpha, int beta)
{
	if (!depth || isGameOver(turn))
	{
		return 0;
		int my, your;
		evaluate(my, your);
		return my - _myturn + _yourturn - your;
	}

	int val = 0;
	for (int x = 0; x < 32; x++)
	{
		int i = _xulie[x];
		//if (i != 23 && i != 10)
		//	continue;
		if (_s[i]->getRed() == turn && !_s[i]->getDead())
		{
			list<Move> lst = SRule::listMove(i, _s);

			for (auto m : lst)
			{
				Move rmove = makeRevMove(m);
				moveStone(m);
				int ns = 0;
				if (turn == _turn)
					ns += getScore(m.killid);
				else
					ns -= getScore(m.killid);
				int val = -alphaBetaSearch(!turn, depth - 1, -beta, -alpha) + getScore(m.killid);
				reverseMove(rmove);
				if (val >= beta)
					return beta;
				if (val > alpha)
					alpha = val;
				//if (alpha >= beta)
				//	break;
			}
		}
	}
	return alpha;
}

bool CBrain::isGameOver(bool turn)
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
		if (_s[i]->getRed() == _turn)
			myturn += getScore(i);
		else
			yourturn += getScore(i);
	}
}