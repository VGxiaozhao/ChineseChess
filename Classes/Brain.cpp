#include "Brain.h"
#include "SRule.h"
#include <cstring>
#include <algorithm>
CBrain::CBrain(bool red, Stone* s[32])
{
	for (int i = 0; i < 32; i++)
		_s[i] = s[i];
	_turn = red;
}

CBrain::CBrain()
{ 
	init();
}

void CBrain::setPad(Stone* s[32], bool turn)
{
	for (int i = 0; i < 32; i++)
		_s[i] = s[i];
	_turn = turn;
}

CBrain::~CBrain()
{
	for (int i = 0; i <= _depth; i++)
		delete mp[i];
}

string CBrain::getCurJu()
{
	string ret = "";
	for (int i = 0; i < 32; i++){
		if (_s[i]->getDead()){
			ret += '99';
		}
		else {
			ret += '0' + (8 - _s[i]->getX());
			ret += '0' + (9 - _s[i]->getY());
		}
	}
	return ret;
}

unsigned long long CBrain::getJuMianLL()
{
	string str = getCurJu();
	unsigned long long ret = 0;
	for (char c : str)
	{
		ret = ret * 191 + c;
	}
	return ret;
}

Move CBrain::int2move(int a)
{
	Move ret;
	ret.moveid = a / 90 + 16;
	ret.x = 8 - ((a % 90) % 9);
	ret.y = 9 - ((a % 90) / 9);
	ret.killid = -1;
	if (ret.x == _s[ret.moveid]->getX() && ret.y == _s[ret.moveid]->getY())
	{
		ret.x = ret.y = 9;
		return ret;
	}
	for (int i = 0; i < 32; i++)
	{
		if (i != ret.moveid && ret.x == _s[i]->getX() && ret.y == _s[i]->getY())
			ret.killid = i;
	}
	return ret;
}

Move CBrain::think()
{
	alphaBetaSearch();
	return _bestMove;
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
	if (_s[4]->getDead() == true || _s[20]->getDead() == true)
		return true;
	return false;
}

int CBrain::evaluate(bool turn)
{
	int ret = 0;
	for (int i = 0; i < 32; i++)
	{
		if (_s[i]->getDead()) continue;
		if (_s[i]->getRed() != _turn)
			ret += _s[i]->getScore();
		else
			ret -= _s[i]->getScore();
	}
	return ret;
}

int CBrain::evaluate(int depth)
{
	if (depth % 2 == 1)
		return evaluate(_turn) - _firstScore;
	else
		return _firstScore - evaluate(_turn);
}

void CBrain::init()
{
	for (int i = 0; i < 32; i++)
		_xulie[i] = i;
	_depth = 2;
	for (int i = 0; i <= _depth; i++)
		mp[i] = new CHashMap(1e6);
}

void  CBrain::alphaBetaSearch()
{
	disorder();
	for (int i = 0; i <= _depth; i++)
		mp[i]->clear();
	_firstScore = evaluate(_turn);
	_hashcnt = _searchCount = 0;
	int val = -1111;
	int vis[10] = { 0 };
	val = alphaBetaSearch(_turn, _depth, -500, 500, vis);
	log("\n\nval: %d, cnt: %d, hash: %d", val, _searchCount, _hashcnt);
	log("mid: %d, kid: %d, x: %d, y: %d", _bestMove.moveid, _bestMove.killid, _bestMove.x, _bestMove.y);
}

int  CBrain::alphaBetaSearch(bool turn, int depth, int alpha, int beta, int vis[])
{
	_searchCount++;
	unsigned long long key = getJuMianLL();
	int tmp = 0;
	for (int i = depth + 1; i <= _depth; i++)
	{
		if (vis[i] == key)
		{
			_hashcnt++;
			return alpha;
		}
	}
	vis[depth] = key;
	if (mp[turn]->find(key, tmp))
	{
		_hashcnt++;
		return tmp;
	}
	if (isGameOver())
	{
		return evaluate(_depth + depth);
	}
	if (!depth)
	{
		return quies(turn, _depth + depth, alpha, beta);
	}
	for (int j = 0; j < 32; j++)
	{
		int i = _xulie[j];
		if (_s[i]->getRed() != turn) continue;
		list<Move> tmp = SRule::listMove(i, _s);
		for (auto m : tmp)
		{
			Move rm = makeRevMove(m);
			moveStone(m);
			int val = -alphaBetaSearch(!turn, depth - 1, -beta, -alpha, vis);
			reverseMove(rm);
			if (val > alpha){
				alpha = val;
				if (depth == _depth)
					_bestMove = m;
			}
			//if (alpha >= beta) break;
			if (val >= beta)
			{
				mp[turn]->add(key, beta);
				return beta;
			}
		}
	}
	mp[turn]->add(key, alpha);
	return alpha;
}


int  CBrain::quies(bool turn,int depth, int alpha, int beta)
{
	_searchCount++;
	int val = evaluate(depth);
	if (val >= beta){
		return beta;
	}
	if (val > alpha){
		alpha = val;
	}
	for (int j = 0; j < 32; j++)
	{
		int i = _xulie[j];
		if (_s[i]->getRed() != turn) continue;
		list<Move> tmp = SRule::listMove(i, _s);
		for (auto m : tmp)
		{
			if (m.killid == -1) continue;
			Move rm = makeRevMove(m);
			moveStone(m);
			int val = -quies(!turn, depth + 1, -beta, -alpha);
			reverseMove(rm);
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