#include "SRule.h"

SRule::SRule()
{
}


SRule::~SRule()
{
}

//������� 
bool SRule::canMove(int moveid, int killid, int x, int y, stChessman* _s)
{
	//���ѡ�е����� 
	stChessman s = _s[moveid];
	//���Ӳ����ƶ�
	if (s.getDead())
		return false;
	//ͬɫ���ɳ�
	if (killid != -1)
	{
		int mred = s.getRed();
		int kred = _s[killid].getRed();
		if (mred == kred)
			return false;
	}
	//ͬλ������
	if (_s[moveid].getX() == x && _s[moveid].getY() == y)
		return false;
	//���ӵ����� 
	switch (s.getType())
	{
		//����������� 
	case Stone::JIANG:
	{
		 return canMoveJiang(moveid, killid, x, y, _s);
	}
		break;
		//ʿ��������� 
	case Stone::SHI:
	{
		   return canMoveShi(moveid, x, y, _s);
	}
		break;
		//���������� 
	case Stone::XIANG:
	{
			 return canMoveXiang(moveid, x, y, _s);
	}
		break;
		//����������� 
	case Stone::JU:
	{
		   return canMoveJu(moveid, x, y, _s);
	}
		break;
		//���������� 
	case Stone::MA:
	{
		  return canMoveMa(moveid, x, y, _s);
	}
		break;
		//�ڵ�������� 
	case Stone::PAO:
	{
		 return canMovePao(moveid, killid, x, y, _s);
	}
		break;
		//����������� 
	case Stone::BING:
	{
		return canMoveBing(moveid, x, y, _s);
	}
		break;
	default:
		break;
	}

	return false;
}

//����������� 
bool SRule::canMoveJiang(int moveid, int killid, int x, int y, stChessman* _s)
{
	if (killid > -1){
		stChessman skill = _s[killid];
		//�����������
		//1��һ����һ�� 
		//2�����ܳ��Ź��� 

		//���Ķ�ɱ 
		if (skill.getType() == Stone::JIANG)
		{
			return canMoveJu(moveid, x, y, _s);
		}
	}
	//ͨ�����ӵ�ID�õ����� 
	stChessman s = _s[moveid];
	//��ý���ǰ��λ�� 
	int xo = s.getX();
	int yo = s.getY();
	//��ý��ߵĸ��� 
	//(x,y)��ʾ���ߵ���λ�� 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//�߽���ʱ����������� 
	//xoff=1, yoff=0������������� 
	//xoff=0, yoff=1������ǰ����� 
	if (d != 1 && d != 10)
	{
		return false;
	}
	//�жϽ��Ƿ���˾Ź� 
	//��ɫ�Ľ��ͺ�ɫ�Ľ���x����ķ�Χ����3<=x<=5 
	if (x<3 || x>5)
	{
		return false;
	}
	if ((y >= 0 && y <= 3) || (y >= 7 && y <= 9))
	{
		return true;
	}
	return false;
}


//ʿ��������� 
bool SRule::canMoveShi(int moveid, int x, int y, stChessman* _s)
{
	//ʿ���������
	//1��һ����һ�� 
	//2�����ܳ��Ź��� 
	//3��б���� 
	// 
	//ͨ�����ӵ�ID�õ����� 
	stChessman s = _s[moveid];
	//���������ǰ��λ�� 
	int xo = s.getX();
	int yo = s.getY();
	//������ߵĸ��� 
	//(x,y)��ʾ���ߵ���λ�� 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//ʿÿ��һ��x������1��,y������1�� 
	//���ߵĸ�������1��ʱ 
	//����false 
	if (d != 11)
	{
		return false;
	}
	//�ж�ʿ�Ƿ���˾Ź� 
	int ax[] = { 3, 3, 3, 3, 4, 4, 5, 5, 5, 5 };
	int ay[] = { 0, 2, 7, 9, 1, 8, 0, 2, 7, 9 };
	bool bret = false;
	for (int i = 0; i < 10; i++)
	{
		if (x == ax[i] && y == ay[i])
		{
			bret = true;
			break;
		}
	}
	return bret;
}

//���������� 
bool SRule::canMoveXiang(int moveid, int x, int y, stChessman* _s)
{
	//���������� 
	//ÿ��һ��x�ƶ�2��,y�ƶ�2�� 
	//���ܹ��� 
	// 
	//ͨ�����ӵ�ID�õ����� 
	stChessman s = _s[moveid];
	//���������ǰ��λ�� 
	int xo = s.getX();
	int yo = s.getY();
	//������ߵĸ��� 
	//(x,y)��ʾ���ߵ���λ�� 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//��ÿһ��x������2����,y������2�� 
	//���ߵĸ�������2��ʱ 
	//����false 
	if (d != 22)
	{
		return false;
	}
	//��������������е����� 
	int xm = (xo + x) / 2;
	int ym = (yo + y) / 2;
	//�õ�(xm,ym)�ϵ����� 
	int id = getStone(xm, ym, _s);
	//��(xm,ym)�������ӵ�ʱ�� 
	if (id != -1)
	{
		//�������� 
		return false;
	}
	//�����಻�ܹ��� 
	int ax[] = { 0, 0, 2, 2, 2, 2, 4, 4, 6, 6, 6, 6, 8, 8 };
	int ay[] = { 2, 7, 0, 4, 5, 9, 2, 7, 0, 4, 5, 9, 2, 7 };
	bool bret = false;
	for (int i = 0; i < 14; i++)
	{
		if (x == ax[i] && y == ay[i])
		{
			bret = true;
			break;
		}
	}
	return bret;
}

//����������� 
bool SRule::canMoveJu(int moveid, int x, int y, stChessman* _s)
{
	//ͨ�����ӵ�ID�õ����� 
	stChessman s = _s[moveid];
	//��ó�����ǰ��λ�� 
	int xo = s.getX();
	int yo = s.getY();
	//������֮�������ӵ�ʱ�򳵲����� 
	if (getStoneCount(xo, yo, x, y, _s) != 0)
	{
		return false;
	}
	return true;
}

//���������� 
bool SRule::canMoveMa(int moveid, int x, int y, stChessman* _s)
{
	//ͨ�����ӵ�ID�õ����� 
	stChessman s = _s[moveid];
	//���������ǰ��λ�� 
	int xo = s.getX();
	int yo = s.getY();
	//CCLog("xo=%d", xo);
	//CCLog("yo=%d", yo);
	//������ߵĸ��� 
	//(x,y)��ʾ���ߵ���λ�� 
	//�������������
	//��һ�������������ǰ�������1�����������������2�� 
	//�ڶ�����������������������1��������ǰ�������2�� 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	//CCLog("x=%d", x);
	//CCLog("y=%d", y);
	int d = xoff * 10 + yoff;
	//CCLog("d=%d", d);
	if (d != 12 && d != 21)
	{
		return false;
	}
	int xm, ym;//��¼��ŵ����� 
	if (d == 12)//�����ߵ��ǵ�һ����� 
	{
		xm = xo;//��ŵ��x����Ϊ����ǰ���x���� 
		ym = (yo + y) / 2;//��ŵ��y����Ϊ����ǰ���y�������������y������е����� 
	}
	else//�����ߵ��ǵڶ������ 
	{
		xm = (xo + x) / 2;//��ŵ��x����Ϊ����ǰ���x�������������x������е����� 
		ym = yo;//��ŵ��y����Ϊ����ǰ���y���� 
	}
	//CCLog("xm=%d", xm); 
	//CCLog("ym=%d", ym); 
	//����ŵ�������ʱ,������ 
	if (getStone(xm, ym, _s) != -1)
	{
		return false;
	}
	return true;
}

//�ڵ�������� 
bool SRule::canMovePao(int moveid, int killid, int x, int y, stChessman* _s)
{
	//ͨ�����ӵ�ID�õ����� 
	stChessman s = _s[moveid];
	//���������ǰ��λ�� 
	int xo = s.getX();
	int yo = s.getY();
	//������������һ������ 
	//��������֮��ֻ��һ�����ӵ�ʱ�� 
	//�ڳԵ��������ϵ����� 
	if (killid != -1 && getStoneCount(xo, yo, x, y, _s) == 1)
	{
		return true;
	}
	if (killid == -1 && getStoneCount(xo, yo, x, y, _s) == 0)
	{
		return true;
	}
	return false;
}

//����������� 
bool SRule::canMoveBing(int moveid, int x, int y, stChessman* _s)
{
	//����������� 
	//1��һ����һ�� 
	//2��ǰ��һ����ܺ��� 
	//3�����Ӻ�ſ��������ƶ� 
	//ͨ�����ӵ�ID�õ����� 
	stChessman s = _s[moveid];
	//��ý���ǰ��λ�� 
	int xo = s.getX();
	int yo = s.getY();
	//��ñ��ߵĸ��� 
	//(x,y)��ʾ���ߵ���λ�� 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//�߽���ʱ����������� 
	//xoff=1, yoff=0������������� 
	//xoff=0, yoff=1������ǰ����� 
	if (d != 1 && d != 10)
	{
		return false;
	}
	//�����ҵ������Ǻ��� 
	if (moveid < 16)
	{
		//���ƺ�ɫ�ı����ܺ��� 
		if (y < yo)
		{
			return false;
		}
		//��ɫ�ı�û�й��Ӳ��������ƶ� 
		if (yo <= 4 && y == yo)
		{
			return false;
		}
	}
	else//�жϺ�ɫ�ı� 
	{
		//���ƺ�ɫ�ı����ܺ��� 
		if (y > yo)
		{
			return false;
		}
		//��ɫ�ı�û�й��Ӳ��������ƶ� 
		if (yo >= 5 && y == yo)
		{
			return false;
		}
	}
	return true;
}

//����(xo,yo)��(x,y)֮��������� 
//���������Ϊ-1,��ʾ(xo,yo)��(x,y)����һ��ֱ���� 
int SRule::getStoneCount(int xo, int yo, int x, int y, stChessman* _s)
{
	int ret = 0;//��¼����֮������ӵĸ��� 
	//(xo,yo)��(x,y)����ͬһ��ֱ���� 
	if (xo != x && yo != y)
	{
		return -1;
	}
	//(xo,yo)��(x,y)��ͬһ���� 
	if (xo == x && yo == y)
	{
		return -1;
	}
	//������ͬһ�������� 
	if (xo == x)
	{
		//minΪ��������y������С�ĵ��y���� 
		int min = yo < y ? yo : y;
		//maxΪ��������y�������ĵ��y���� 
		int max = yo > y ? yo : y;
		//����ͬһ������������֮��������� 
		for (int yy = min + 1; yy<max; yy++)
		{
			//������֮�������ӵ�ʱ�� 
			if (getStone(x, yy, _s) != -1)
			{
				++ret;//��������1 
			}
		}
	}
	else//������ͬһ��������yo == y 
	{
		//minΪ��������x������С�ĵ��x���� 
		int min = xo < x ? xo : x;
		//maxΪ��������x�������ĵ��x���� 
		int max = xo > x ? xo : x;
		//����ͬһ������������֮��������� 
		for (int xx = min + 1; xx<max; xx++)
		{
			//������֮�������ӵ�ʱ�� 
			if (getStone(xx, y, _s) != -1)
			{
				++ret;//��������1 
			}
		}
	}
	//��������֮��������� 
	return ret;
}

//ͨ��������±��ȡ���ӵ�ID 
//���������û������,����-1 
int SRule::getStone(int x, int y, stChessman* _s)
{
	stChessman s;
	//����32������ 
	for (int i = 0; i<32; i++)
	{
		s = _s[i];
		if (s.getX() == x && s.getY() == y && !s.getDead())
		{
			//�õ����ӵ�ID 
			return i;
		}
	}

	return -1;
}


list<Move> SRule::listMove(int mid, stChessman* _s)
{
	stChessman s = _s[mid];
	list<Move> ret;
	ret.clear();
	if (s.getDead())
	{
		return ret;
	}
	//���ӵ����� 
	switch (s.getType())
	{
		//����������� 
		case Stone::BING:
			ret = listMoveBing(mid, _s);
		break;
		//ʿ��������� 
		case Stone::SHI:
			ret = listMoveShi(mid, _s);
		break;
		//���������� 
		case Stone::XIANG:
			ret = listMoveXiang(mid, _s);
		break;
		//����������� 
		case Stone::JIANG:
		case Stone::JU:
		case Stone::PAO:
			ret = listMovePao(mid, _s);
		break;
		//���������� 
		case Stone::MA:
			ret = listMoveMa(mid, _s);
		break;
		default:
			break;
	}
	list<Move> xret; xret.clear();
	for (auto m : ret){
		if (m.killid == -1)
		{
			//if (isValidMove(m, _s))
				xret.push_back(m);
		}
		else
			xret.push_front(m);
	}
	return xret;
}

bool SRule::isValidMove(Move e, stChessman* _s)
{
	bool turn = _s[e.moveid].getRed();
	bool danger = false;
	for (int i = 0; i < 32; i++)
	{
		if (i != e.moveid)
		{
			if (canMove(i, -1, e.x, e.y, _s))
			{
				if (_s[i].getRed() == _s[e.moveid].getRed() && _s[i].getScore() >= _s[e.moveid].getScore())
					return true;
				else if (_s[i].getRed() != _s[e.moveid].getRed() && _s[i].getScore() < _s[e.moveid].getScore())
					return false;
				else
					danger = true;
			}
		}
	}
	if (danger)
		return false;
	return true;
}

list<Move> SRule::listMovePao(int mid, stChessman* _s)
{
	list<Move> ret; ret.clear();
	int x = _s[mid].getX();
	int y = _s[mid].getY();
	for (int d = -9; d <= 9; d++)
	{
		int nx = x + d;
		int ny = y + d;
		if (nx >= 0 && nx <= 8)
		{
			int kid = getStone(nx, y, _s);
			if (canMove(mid, kid, nx, y, _s))
				ret.push_back(Move(mid, kid, nx, y));
		}
		if (ny >= 0 && ny <= 9)
		{
			int kid = getStone(x, ny, _s);
			if (canMove(mid, kid, x, ny, _s))
				ret.push_back(Move(mid, kid, x, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveShi(int mid, stChessman* _s)
{
	list<Move> ret; ret.clear();
	int x = _s[mid].getX();
	int y = _s[mid].getY();
	int dx[] = { -1, -1, 1, 1 };
	int dy[] = { -1, 1, -1, 1 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s);
			if (canMove(mid, kid, nx, ny, _s))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveBing(int mid, stChessman* _s)
{
	list<Move> ret; ret.clear();
	int x = _s[mid].getX();
	int y = _s[mid].getY();
	int dx[] = { 0, -1, 0, 1 };
	int dy[] = { -1, 0, 1, 0 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s);
			if (canMove(mid, kid, nx, ny, _s))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveXiang(int mid, stChessman* _s)
{
	list<Move> ret; ret.clear();
	int x = _s[mid].getX();
	int y = _s[mid].getY();
	int dx[] = { -2, -2, 2, 2 };
	int dy[] = { -2, 2, -2, 2 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s);
			if (canMove(mid, kid, nx, ny, _s))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveMa(int mid, stChessman* _s)
{
	list<Move> ret; ret.clear();
	int x = _s[mid].getX();
	int y = _s[mid].getY();
	int dx[] = { -2, -2, -1, -1, 1, 1, 2, 2 };
	int dy[] = { -1, 1, -2, 2, -2, 2, -1, 1 };
	for (int d = 0; d < 8; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s);
			if (canMove(mid, kid, nx, ny, _s))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}