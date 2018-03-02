#pragma once
#include "Board.h"
#include "Stone.h"
#include "Brain.h"
#include <time.h>
#include <vector>
#include <stdio.h>

class MonteCarlo
{
private:
	struct Node
	{
		//胜利次数， 此已扩展分枝数， 分枝数， 父节点编号
		int _q, _n, _all, _father;
		int _v, _next;
		CBoard _board;
		std::vector<Move> _move;
		Node(){}
		Node(CBoard board, int u){
			_board = board;
			_move = _board.listKillMove();
			_all = _q = _n = 0;
			_father = u;
			//_move.clear();
		}
	}_node[100005];
public:
	MonteCarlo(Stone* s[], bool turn);
	~MonteCarlo();
public:
	Move UCTSearch();
	int treePolicy(int);
	int expand(int);
	int bestChild(int v);
	int defaultPolicy(CBoard v);
	void backup(int v, int d);
	void show();
protected:
	std::vector<std::vector<int> > _tree;
	std::vector<MonteCarlo::Node> _arrNode;
	void add(int u, int v, CBoard board)
	{
		_node[_tol]._v = v;
		_node[_tol]._father = u;
		_node[_tol]._next = _head[u];
		_node[_tol]._board = board;
		_node[_tol]._move = board.listKillMove();
		_node[_tol]._q = _node[_tol]._n = _node[_tol]._all = 0;
		_head[u] = _tol++;
	}
	double _c;
	int _head[100005];
	int _tol;
	int _initScore;
	bool _turn;
};

