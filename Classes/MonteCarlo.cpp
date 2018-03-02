#include "MonteCarlo.h"


MonteCarlo::MonteCarlo(Stone* s[], bool turn)
{
	_arrNode.clear();
	_tree.clear();
	CBoard newBoard(s, turn);
	_arrNode.push_back(Node(newBoard, -1));
	std::vector<int> tmp;
	_tree.push_back(tmp);
	_turn = turn;
	_initScore = newBoard.getScore(_turn);
	_c = 1.5;
	_tol = 0;
	memset(_head, -1, sizeof(_head));
}


MonteCarlo::~MonteCarlo()
{

}

Move MonteCarlo::UCTSearch()
{
	int st = clock();
	//while (clock() - st <= 5000)
	for (int i = 0; i < 500; i++)
	{
		int v = treePolicy(0);
		int d = defaultPolicy(_arrNode[v]._board);
		backup(v, d);
	}
	int v = bestChild(0);
	for (int i = 0; i < _tree[0].size(); i++)
	{
		if (_tree[0][i] == v)
			return _arrNode[0]._move[i];
	}
}

int MonteCarlo::treePolicy(int u)
{
	int f = u;
	int tmp = _arrNode[u]._all;

	while (_arrNode[u]._n!=0 && _arrNode[u]._all>=_arrNode[u]._move.size() && u!=-1 && !_arrNode[u]._board.isGameOver())
	{
		u = bestChild(u);
	}
	if (u == -1) return f;
	int v = expand(u);
	return v;
}

int MonteCarlo::expand(int u)
{
	if (u < 0) return u;
	for (int i = _arrNode[u]._all; i < _arrNode[u]._move.size(); i++)
	{
		Move& m = _arrNode[u]._move[i];
		_arrNode[u]._board.moveStone(m);
		CBoard newBoard = _arrNode[u]._board;
		_arrNode[u]._board.reverseMove();
		_arrNode.push_back(Node(newBoard, u));
		_tree[u].push_back(_arrNode.size()-1);
		std::vector<int> tmp;
		_tree.push_back(tmp);
		_arrNode[u]._all++;
		break;
	}
	return _arrNode.size() - 1;
}

int MonteCarlo::bestChild(int u)
{
	double mx = -1222222;
	int ret = -1;
	for (int i = 0; i<_tree[u].size(); i++)
	{
		int v = _tree[u][i];
		double tmp = 0.0;
		if (_arrNode[v]._n)
			tmp = 1.0*_arrNode[v]._q / _arrNode[v]._n * _c*(sqrt(2.0*log(_arrNode[u]._n) / _arrNode[v]._n));
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

int MonteCarlo::defaultPolicy(CBoard board)
{
	for (int i = 0; i < 3 && !board.isGameOver(); i++)
	{
		CBrain d(board);
		auto m = d.alphaBetaSearch(1);
		board.moveStone(m);
	}
	int ed = board.getScore(_turn);
	return ed>_initScore ? 1 : 0;
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

void MonteCarlo::show()
{
	auto fp = fopen("d:/cmd.txt", "w+");
	for (int u = 0; u < _tree.size(); u++)
	{
		if (_arrNode[u]._move.size() == 0) continue;
		fprintf(fp, "%d->%d(%d/%d): ", _arrNode[u]._father, u, _arrNode[u]._q, _arrNode[u]._n);
		for (auto id : _tree[u])
		{
			fprintf(fp, "%d(%d/%d) ",id, _arrNode[id]._q, _arrNode[id]._n);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}