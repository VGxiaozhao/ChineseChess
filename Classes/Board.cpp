#include "Board.h"


CBoard::CBoard(Stone* s[], bool turn)
{
	init();
	setBoard(s, turn);
}

CBoard::CBoard()
{
	init();
}

void CBoard::init()
{
	while (!_x0.empty()) _x0.pop();
	while (!_y0.empty()) _y0.pop();
	while (!_id.empty()) _id.pop();
	while (!_revive.empty()) _revive.pop();
	_turn = false;
}

void CBoard::setBoard(Stone* s[], bool turn)
{
	for (int i = 0; i < 32; i++)
		_s[i] = *s[i];
	_turn = turn;
}

void CBoard::setBoard(const stChessman s[])
{
	for (int i = 0; i < 32; i++)
		_s[i] = s[i];
}

CBoard::~CBoard()
{
}

CBoard& CBoard::operator=(const CBoard& cmp)
{
	setBoard(cmp._s);
	_turn = cmp._turn;
	return *this;
}

void CBoard::moveStone(Move m)
{
	_id.push(m.moveid);
	_x0.push(_s[m.moveid]._x);
	_y0.push(_s[m.moveid]._y);
	_revive.push(m.killid);

	_s[m.moveid]._x = m.x;
	_s[m.moveid]._y = m.y;
	if (m.killid != -1)
	{
		_s[m.killid]._dead = true;
	}
	_turn = !_turn;
}

void CBoard::reverseMove()
{
	if (_id.empty()) return;
	if (_revive.top() != -1)
		_s[_revive.top()]._dead = false;
	int id = _id.top();
	_s[id]._x = _x0.top();
	_s[id]._y = _y0.top();

	_x0.pop(); _y0.pop();
	_id.pop(); _revive.pop();
}

Move CBoard::makeRevMove(Move m)
{
	Move ret;
	ret = m;
	ret.x = _s[m.moveid]._x;
	ret.y = _s[m.moveid]._y;
	return ret;
}

std::string CBoard::getJumianStr()
{
	std::string ret = "";
	for (int i = 0; i < 32; i++){
		if (_s[i]._dead){
			ret += '99';
		}
		else {
			ret += '0' + (8 - _s[i]._x);
			ret += '0' + (9 - _s[i]._y);
		}
	}
	return ret;
}

unsigned long long CBoard::getJumianLL()
{
	std::string str = getJumianStr();
	unsigned long long ret = 0;
	for (char c : str)
	{
		ret = ret * 191 + c;
	}
	return ret;
}

int CBoard::evaluate(bool turn)
{
	int ret = 0;
	for (int i = 0; i < 32; i++)
	{
		if (_s[i]._dead) continue;
		if (_s[i]._red != turn)
			ret += _s[i].getScore();
		else
			ret -= _s[i].getScore();
	}
	return ret;
}

int CBoard::evaluate(int depth, int firstScore, bool turn)
{
	//return evaluate(turn) - firstScore;
	if (depth % 2 == 1)
		return evaluate(turn) - firstScore;
	else
		return firstScore - evaluate(turn);
}

bool CBoard::isGameOver()
{
	if (_s[4]._dead == true || _s[20]._dead == true)
		return true;
	return false;
}

bool CBoard::isOnCheck(bool turn)
{
	int Jiang = -1;
	for (int i = 0; i < 32; i++)
	{
		if (_s[i].getRed() == turn && _s[i].getType() == Stone::JIANG && _s[i].getDead() == false)
		{
			Jiang = i;
			break;
		}
	}
	int &x = _s[Jiang]._x;
	int &y = _s[Jiang]._y;
	for (int i = 0; i < 32; i++)
	{
		if (_s[i].getRed() != turn && _s[i].getDead() == false)
		{
			
		}
	}
	return false;
}

void CBoard::setHas()
{
	memset(has, -1, sizeof(int)* 100);
	for (int i = 0; i < 32; i++)
	{
		if (_s[i].getDead()) continue;
		if (_s[i].getRed())
			has[_s[i]._x][_s[i]._y] = i;
		else
			has[_s[i]._x][_s[i]._y] = i;
	}
}

std::list<Move> CBoard::listMove(int mid)
{
	stChessman s = _s[mid];
	std::list<Move> ret;
	ret.clear();
	if (s.getDead())
	{
		return ret;
	}
	setHas();
	switch (s.getType())
	{
	case Stone::BING:
		ret = listMoveBing(mid);
		break;
	case Stone::SHI:
		ret = listMoveShi(mid);
		break;
	case Stone::XIANG:
		ret = listMoveXiang(mid);
		break;
	case Stone::JIANG:
		ret = listMoveJiang(mid);
		break;
	case Stone::JU:
		ret = listMoveJu(mid);
		break;
	case Stone::PAO:
		ret = listMovePao(mid);
		break;
	case Stone::MA:
		ret = listMoveMa(mid);
		break;
	default:
		break;
	}
	std::list<Move> xret;
	for (auto m : ret)
	{
		if (m.killid != -1)
			xret.push_front(m);
		else
			xret.push_back(m);
	}
	return xret;
}

bool CBoard::isEnemy(int a, int b)
{
	if (a == -1 || b == -1) return true;
	if (a <= 15 && b > 15) return true;
	if (b <= 15 && a > 15) return true;
	return false;
}

bool CBoard::isGoOut(int mid, int x, int y)
{
	bool down = getJiangDire(mid);
	if (down && y >= 5) return true;
	else if (!down && y < 5) return true;
	return false;
}

bool CBoard::getJiangDire(int mid)
{
	int Jiang = 4;
	if (mid >= 16) Jiang = 20;
	bool down = false;
	if (_s[Jiang]._y <= 2) down = true;
	return down;
}

std::list<Move> CBoard::listMoveBing(int mid)
{
	std::list<Move> ret;
	int& x = _s[mid]._x;
	int& y = _s[mid]._y;
	if (x - 1 >= 0 && isEnemy(mid, has[x - 1][y]) && isGoOut(mid, _s[mid]._x, _s[mid]._y))
		ret.push_back(Move(mid, has[x - 1][y], x - 1, y));
	if (x + 1 <= 8 && isEnemy(mid, has[x + 1][y]) && isGoOut(mid, _s[mid]._x, _s[mid]._y))
		ret.push_back(Move(mid, has[x + 1][y], x + 1, y));
	int d = getJiangDire(mid) ? 1 : -1;
	if (y + d >= 0 && y + d <= 9 && isEnemy(mid, has[x][y + d]))
		ret.push_back(Move(mid, has[x][y + d], x, y + d));
	return ret;
}

std::list<Move> CBoard::listMoveXiang(int mid)
{
	std::list<Move> ret;
	int& x = _s[mid]._x;
	int& y = _s[mid]._y;
	int dx[] = { -2, -2, 2, 2 };
	int dy[] = { -2, 2, -2, 2 };
	int ax[] = { -1, -1, 1, 1 };
	int ay[] = { -1, 1, -1, 1 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9 && !isGoOut(mid, nx, ny) && isEnemy(mid, has[nx][ny]))
		{
			int xx = x + ax[d];
			int yy = y + ay[d];
			if (xx >= 0 && xx <= 8 && yy >= 0 && yy <= 9 && has[xx][yy] == -1)
			{
				ret.push_back(Move(mid, has[nx][ny], nx, ny));
			}
		}
	}
	return ret;
}

std::list<Move> CBoard::listMoveShi(int mid)
{
	std::list<Move> ret;
	int& x = _s[mid]._x;
	int& y = _s[mid]._y;
	int dx[] = { -1, -1, 1, 1 };
	int dy[] = { -1, 1, -1, 1 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 3 && nx <= 5 && ny >= 0 && ny <= 9 && isEnemy(mid, has[nx][ny]))
		{
			if (getJiangDire(mid) && ny<=2)
				ret.push_back(Move(mid, has[nx][ny], nx, ny));
			else if (!getJiangDire(mid) && ny>=7)
				ret.push_back(Move(mid, has[nx][ny], nx, ny));
		}
	}
	return ret;
}

std::list<Move> CBoard::listMoveJiang(int mid)
{
	std::list<Move> ret;
	int& x = _s[mid]._x;
	int& y = _s[mid]._y;
	int dx[] = { 0, -1, 1, 0 };
	int dy[] = { -1, 0, 0, 1 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 3 && nx <= 5 && ny >= 0 && ny <= 9 && isEnemy(mid, has[nx][ny]))
		{
			if (getJiangDire(mid) && ny <= 2)
				ret.push_back(Move(mid, has[nx][ny], nx, ny));
			else if (!getJiangDire(mid) && ny >= 7)
				ret.push_back(Move(mid, has[nx][ny], nx, ny));
		}
	}
	//∂‘√Ê–¶…±
	bool down = getJiangDire(mid);
	int st = 1, ed = 10, step = 1;
	if (!down)
	{
		st = -1;
		ed = -10;
		step = -1;
	}
	for (int d = st; d <= ed; d += step)
	{
		int ny = y + d;
		if (ny >= 0 && ny <= 9)
		{
			if (has[x][ny] != -1)
			{
				if (_s[has[x][ny]].getType() == Stone::JIANG)
				{
					ret.push_back(Move(mid, has[x][ny], x, ny));
				}
				break;
			}
		}
	}
	return ret;
}

std::list<Move> CBoard::listMoveMa(int mid)
{
	std::list<Move> ret;
	int& x = _s[mid]._x;
	int& y = _s[mid]._y;
	int dx[] = { -2, -2, -1, -1, 1, 1, 2, 2 };
	int dy[] = { -1, 1, -2, 2, -2, 2, -1, 1 };
	int ax[] = {-1, -1, 0, 0, 0, 0, 1, 1};
	int ay[] = {0, 0, -1, 1, -1, 1, 0, 0};
	for (int d = 0; d < 8; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9 && isEnemy(mid, has[nx][ny]))
		{
			if (x + ax[d] >= 0 && x + ax[d] <= 8 && y + ay[d] >= 0 && y + ay[d] <= 9 && has[x + ax[d]][y + ay[d]] == -1)
				ret.push_back(Move(mid, has[nx][ny], nx, ny));;
		}
	}
	return ret;
}

std::list<Move> CBoard::listMoveJu(int mid)
{
	std::list<Move> ret;
	listMoveJu(mid, _s[mid]._x, _s[mid]._y, -1, 0, ret);
	listMoveJu(mid, _s[mid]._x, _s[mid]._y, 1, 0, ret);
	listMoveJu(mid, _s[mid]._x, _s[mid]._y, 0, -1, ret);
	listMoveJu(mid, _s[mid]._x, _s[mid]._y, 0, 1, ret);
	return ret;
}

void CBoard::listMoveJu(int mid, int x, int y, int dx, int dy, std::list<Move>& lst)
{
	int nx = x + dx;
	int ny = y + dy;
	if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9);
	else return;
	if (isEnemy(mid, has[nx][ny]))
		lst.push_back(Move(mid, has[nx][ny], nx, ny));
	if (has[nx][ny] == -1)
		listMoveJu(mid, nx, ny, dx, dy, lst);
}


std::list<Move> CBoard::listMovePao(int mid)
{
	std::list<Move> ret;
	listMovePao(mid, _s[mid]._x, _s[mid]._y, -1, 0, false,  ret);
	listMovePao(mid, _s[mid]._x, _s[mid]._y, 1, 0, false, ret);
	listMovePao(mid, _s[mid]._x, _s[mid]._y, 0, -1, false, ret);
	listMovePao(mid, _s[mid]._x, _s[mid]._y, 0, 1, false, ret);
	return ret;
}

void CBoard::listMovePao(int mid, int x, int y, int dx, int dy, bool yue, std::list<Move>& lst)
{
	int nx = x + dx;
	int ny = y + dy;
	if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9);
	else return;
	if (has[nx][ny] == -1)
	{
		if (!yue)
		{
			lst.push_back(Move(mid, has[nx][ny], nx, ny));
		}
		listMovePao(mid, nx, ny, dx, dy, yue, lst);
	}
	else if (has[nx][ny] != -1 && !yue)
	{
		listMovePao(mid, nx, ny, dx, dy, true, lst);
	}
	else if (yue && has[nx][ny] != -1 && isEnemy(mid, has[nx][ny]))
	{
		lst.push_back(Move(mid, has[nx][ny], nx, ny));
	}
}