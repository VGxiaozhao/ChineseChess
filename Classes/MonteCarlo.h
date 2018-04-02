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
		CBoard _board;
		std::vector<Move> _move;
		Node(){}
		Node(CBoard board, int u){
			_board = board;
			//_move = _board.listAllMove();
			_move = _board.get10NNMove();
			_all = _q = _n = 0;
			_father = u;
			//_move.clear();
		}
	};
	struct Edge
	{
		int v, next;
	};
public:
	MonteCarlo(Stone* s[], bool turn);
	MonteCarlo(CBoard s, bool turn);
	~MonteCarlo();
public:
	Move UCTSearch();
	int treePolicy(int);
	int bestChild(int v);
	int defaultPolicy(CBoard v);
	void backup(int v, int d);
	void show(Move, int);
protected:
	void addEdge(int u, int v);
	std::vector<MonteCarlo::Node> _arrNode;
	double _c;
	int _head[11005];
	int _tol;
	int _initScore;
	bool _turn;
	Edge _edge[11005];
};

