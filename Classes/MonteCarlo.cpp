#include "MonteCarlo.h"


MonteCarlo::MonteCarlo(Stone* s[], bool turn)
{
	_arrNode.clear();
	CBoard newBoard(s, turn);
	_arrNode.push_back(Node(newBoard, -1));
	_turn = turn;
	_initScore = newBoard.getScore(_turn);
	_c = 0.5;
	_tol = 0;
	memset(_head, -1, sizeof(_head));
}

MonteCarlo::MonteCarlo(CBoard s, bool turn)
{
	_arrNode.clear();
	_arrNode.push_back(Node(s, -1));
	_turn = turn;
	_initScore = s.getScore(_turn);
	_c = 0.5;
	_tol = 0;
	memset(_head, -1, sizeof(_head));
}

MonteCarlo::~MonteCarlo()
{

}

Move MonteCarlo::UCTSearch()
{
	int st = clock();
	while (clock() - st <= 5000)
	//for (int i = 0; i < 1000; i++)
	{
		int v = treePolicy(0);
		int d = defaultPolicy(_arrNode[v]._board);
		backup(v, d);
	}
	int v = bestChild(0);
	//int v = bestChildUsePoss(0);
	for (int i = _head[0], j = _arrNode[0]._all-1; i != -1; i = _edge[i].next, j--)
	{
		if (_edge[i].v == v)
		{
			show(_arrNode[0]._move[j], v);
			return _arrNode[0]._move[j];
		}
	}
}

int MonteCarlo::treePolicy(int u)
{
	int f = u;
	int tmp = _arrNode[u]._all;

	while (u != -1 && _arrNode[u]._n!=0 && _arrNode[u]._all==_arrNode[u]._move.size())
	{
		u = bestChild(u);
		//u = bestChildUsePoss(u);
	}
	if (u == -1) return f;

	int i = _arrNode[u]._all;
	Move& m = _arrNode[u]._move[i];
	//移动棋子以生成新棋盘
	CBoard newBoard = _arrNode[u]._board;
	newBoard.moveStone(m);

	//添加新节点
	_arrNode.push_back(Node(newBoard, u));
	_arrNode[u]._all++;
	int v = _arrNode.size() - 1;
	addEdge(u, v);
	return v;
}

void MonteCarlo::addEdge(int u, int v)
{
	_edge[_tol].v = v;
	_edge[_tol].next = _head[u];
	_head[u] = _tol++;
}

int MonteCarlo::bestChild(int u)
{
	double mx = -1222222;
	int ret = -1;
	std::vector<int> vec;
	for (int i = _head[u]; i!=-1; i=_edge[i].next)
	{
		int v = _edge[i].v;
		vec.push_back(v);
	}
	//打乱顺序
	srand(time(NULL));
	for (int i = 0; i < vec.size(); i++)
	{
		int j = rand() % vec.size();
		std::swap(vec[i], vec[j]);
	}
	//找到最好的点
	for (int v : vec)
	{
		double tmp = 0.0;
		if (_arrNode[v]._n)
			tmp = ((1.0*_arrNode[v]._q) / _arrNode[v]._n) + _c*(sqrt(2.0*log(_arrNode[u]._n) / _arrNode[v]._n));
		else
			continue;
		if (tmp > mx)
		{
			mx = tmp;
			ret = v;
		}
	}
	return ret;
}

int MonteCarlo::bestChildUsePoss(int u)
{
	double mx = -1222222;
	int ret = -1, j = _arrNode[u]._poss.size()-1;
	for (int i = _head[u]; i != -1; i = _edge[i].next)
	{
		int v = _edge[i].v;
		double poss = _arrNode[u]._poss[j--];
		double tmp = (1.0 * _arrNode[v]._q + poss) / _arrNode[v]._n + _c*(sqrt(2.0*log(_arrNode[u]._n) / _arrNode[v]._n));
		if (tmp > mx)
		{
			mx = tmp;
			ret = v;
		}
	}
	return ret;
}

int MonteCarlo::defaultPolicy(CBoard board)
{
	for (int i = 0; i < 1 && !board.isGameOver(); i++)
	{
		CBrain d(board);
		Move m = d.alphaBetaSearch(1);
		//Move m = board.getNNMove();
		board.moveStone(m);
	}
	int ed = board.getScore(_turn);
	int d = ed - _initScore;
	if (_turn == board.getTurn())
		d = _initScore - ed;
	return d > 0 ? 1 : 0;
}

void MonteCarlo::backup(int v, int d)
{
	while (v != -1)
	{
		_arrNode[v]._n += 1;
		_arrNode[v]._q += d;
		v = _arrNode[v]._father;
	}
}

void MonteCarlo::show(Move m, int wv)
{
	auto fp = fopen("d:/cmd.txt", "w+");
	fprintf(fp, "mid: %d, kid : %d, x : %d, y : %d\n", m.moveid, m.killid, m.x, m.y);
	fprintf(fp, "choose %d\n", wv);
	for (int i = 0; i < _arrNode.size(); i++)
	{
		fprintf(fp, "%d->%d(%d/%d): ", _arrNode[i]._father, i, _arrNode[i]._q, _arrNode[i]._n);
		int cnt = _arrNode[i]._move.size()-1;
		for (int j = _head[i]; j != -1; j = _edge[j].next)
		{
			int v = _edge[j].v;
			auto m = _arrNode[i]._move[cnt--];
			double tmp = ((1.0*_arrNode[v]._q) / _arrNode[v]._n) + _c*(sqrt(2.0*log(_arrNode[i]._n) / _arrNode[v]._n));
			fprintf(fp, "%d(mid:%d kid:%d x:%d y:%d || %.3lf=%d/%d) ",v, m.moveid, m.killid, m.x, m.y, tmp, _arrNode[v]._q, _arrNode[v]._n);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}