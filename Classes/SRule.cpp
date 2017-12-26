#include "SRule.h"

SRule::SRule()
{
}


SRule::~SRule()
{
}

//走棋规则 
bool SRule::canMove(int moveid, int killid, int x, int y, Stone* _s[], bool _redSide)
{
	//获得选中的棋子 
	Stone* s = _s[moveid];
	//死子不可移动
	if (s->getDead())
		return false;
	//同色不可吃
	if (killid != -1)
	{
		int mred = s->getRed();
		int kred = _s[killid]->getRed();
		if (mred == kred)
			return false;
	}
	//同位无需走
	if (_s[moveid]->getX() == x && _s[moveid]->getY() == y)
		return false;
	//棋子的类型 
	switch (s->getType())
	{
		//将的走棋规则 
	case Stone::JIANG:
	{
		 return canMoveJiang(moveid, killid, x, y, _s, _redSide);
	}
		break;
		//士的走棋规则 
	case Stone::SHI:
	{
		   return canMoveShi(moveid, x, y, _s, _redSide);
	}
		break;
		//相的走棋规则 
	case Stone::XIANG:
	{
			 return canMoveXiang(moveid, x, y, _s, _redSide);
	}
		break;
		//车的走棋规则 
	case Stone::JU:
	{
		   return canMoveJu(moveid, x, y, _s, _redSide);
	}
		break;
		//马的走棋规则 
	case Stone::MA:
	{
		  return canMoveMa(moveid, x, y, _s, _redSide);
	}
		break;
		//炮的走棋规则 
	case Stone::PAO:
	{
		 return canMovePao(moveid, killid, x, y, _s, _redSide);
	}
		break;
		//兵的走棋规则 
	case Stone::BING:
	{
		return canMoveBing(moveid, x, y, _s, _redSide);
	}
		break;
	default:
		break;
	}

	return false;
}

//将的走棋规则 
bool SRule::canMoveJiang(int moveid, int killid, int x, int y, Stone* _s[], bool _redSide)
{
	if (killid > -1){
		Stone* skill = _s[killid];
		//将的走棋规则：
		//1、一次走一格 
		//2、不能出九宫格 

		//将的对杀 
		if (skill->getType() == Stone::JIANG)
		{
			return canMoveJu(moveid, x, y, _s, _redSide);
		}
	}
	//通过棋子的ID得到棋子 
	Stone* s = _s[moveid];
	//获得将当前的位置 
	int xo = s->getX();
	int yo = s->getY();
	//获得将走的格数 
	//(x,y)表示将走到的位置 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//走将的时候有两种情况 
	//xoff=1, yoff=0：将向左或向右 
	//xoff=0, yoff=1：将向前或向后 
	if (d != 1 && d != 10)
	{
		return false;
	}
	//判断将是否出了九宫 
	//红色的将和黑色的将的x坐标的范围都是3<=x<=5 
	if (x<3 || x>5)
	{
		return false;
	}
	//如果玩家的棋子是红棋 
	if (_redSide == s->getRed())
	{
		//判断将是否出了九宫 
		if (y<0 || y>2)
		{
			return false;
		}
	}
	else//判断黑色的将的范围 
	{
		//判断将是否出了九宫 
		if (y>9 || y<7)
		{
			return false;
		}
	}

	return true;
}


//士的走棋规则 
bool SRule::canMoveShi(int moveid, int x, int y, Stone* _s[], bool _redSide)
{
	//士的走棋规则：
	//1、一次走一格 
	//2、不能出九宫格 
	//3、斜着走 
	// 
	//通过棋子的ID得到棋子 
	Stone* s = _s[moveid];
	//获得相走棋前的位置 
	int xo = s->getX();
	int yo = s->getY();
	//获得相走的格数 
	//(x,y)表示将走到的位置 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//士每走一步x方向走1格,y方向走1格 
	//当走的格数大于1格时 
	//返回false 
	if (d != 11)
	{
		return false;
	}
	//判断士是否出了九宫 
	//红色的士和黑色的士的x坐标的范围都是3<=x<=5 
	if (x<3 || x>5)
	{
		return false;
	}
	//如果玩家的棋子是红棋 
	if (_redSide == s->getRed())
	{
		//判断士是否出了九宫 
		if (y<0 || y>2)
		{
			return false;
		}
	}
	else//判断黑色的士的范围 
	{
		//判断士是否出了九宫 
		if (y>9 || y<7)
		{
			return false;
		}
	}

	return true;
}

//相的走棋规则 
bool SRule::canMoveXiang(int moveid, int x, int y, Stone* _s[], bool _redSide)
{
	//相的走棋规则： 
	//每走一次x移动2格,y移动2格 
	//不能过河 
	// 
	//通过棋子的ID得到棋子 
	Stone* s = _s[moveid];
	//获得相走棋前的位置 
	int xo = s->getX();
	int yo = s->getY();
	//获得相走的格数 
	//(x,y)表示将走到的位置 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//相每一次x方向走2格子,y方向走2格 
	//当走的格数大于2格时 
	//返回false 
	if (d != 22)
	{
		return false;
	}
	//计算两个坐标的中点坐标 
	int xm = (xo + x) / 2;
	int ym = (yo + y) / 2;
	//得到(xm,ym)上的棋子 
	int id = getStone(xm, ym, _s, _redSide);
	//当(xm,ym)上有棋子的时候 
	if (id != -1)
	{
		//不能走相 
		return false;
	}
	//限制相不能过河 
	//如果玩家的棋子是红棋 
	if (_redSide == s->getRed())
	{
		//判断相是否过了河 
		if (y > 4)
		{
			return false;
		}
	}
	else//判断黑色的相的范围 
	{
		//判断相是否过了河 
		if (y < 5)
		{
			return false;
		}
	}
	return true;
}

//车的走棋规则 
bool SRule::canMoveJu(int moveid, int x, int y, Stone* _s[], bool _redSide)
{
	//通过棋子的ID得到棋子 
	Stone* s = _s[moveid];
	//获得车走棋前的位置 
	int xo = s->getX();
	int yo = s->getY();
	//当两点之间有棋子的时候车不能走 
	if (getStoneCount(xo, yo, x, y, _s, _redSide) != 0)
	{
		return false;
	}
	return true;
}

//马的走棋规则 
bool SRule::canMoveMa(int moveid, int x, int y, Stone* _s[], bool _redSide)
{
	//通过棋子的ID得到棋子 
	Stone* s = _s[moveid];
	//获得马走棋前的位置 
	int xo = s->getX();
	int yo = s->getY();
	//CCLog("xo=%d", xo);
	//CCLog("yo=%d", yo);
	//获得马走的格数 
	//(x,y)表示马走到的位置 
	//马有两种情况：
	//第一种情况：马先向前或向后走1步，再向左或向右走2步 
	//第二种情况：马先向左或向右走1不，再向前或向后走2步 
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
	int xm, ym;//记录绑脚点坐标 
	if (d == 12)//当马走的是第一种情况 
	{
		xm = xo;//绑脚点的x坐标为走棋前马的x坐标 
		ym = (yo + y) / 2;//绑脚点的y坐标为走棋前马的y坐标和走棋后马的y坐标的中点坐标 
	}
	else//当马走的是第二种情况 
	{
		xm = (xo + x) / 2;//绑脚点的x坐标为走棋前马的x坐标和走棋后马的x坐标的中点坐标 
		ym = yo;//绑脚点的y坐标为走棋前马的y坐标 
	}
	//CCLog("xm=%d", xm); 
	//CCLog("ym=%d", ym); 
	//当绑脚点有棋子时,不能走 
	if (getStone(xm, ym, _s, _redSide) != -1)
	{
		return false;
	}
	return true;
}

//炮的走棋规则 
bool SRule::canMovePao(int moveid, int killid, int x, int y, Stone* _s[], bool _redSide)
{
	//通过棋子的ID得到棋子 
	Stone* s = _s[moveid];
	//获得炮走棋前的位置 
	int xo = s->getX();
	int yo = s->getY();
	//当触摸点上有一个棋子 
	//而且两点之间只有一个棋子的时候 
	//炮吃掉触摸点上的棋子 
	if (killid != -1 && getStoneCount(xo, yo, x, y, _s, _redSide) == 1)
	{
		return true;
	}
	if (killid == -1 && getStoneCount(xo, yo, x, y, _s, _redSide) == 0)
	{
		return true;
	}
	return false;
}

//兵的走棋规则 
bool SRule::canMoveBing(int moveid, int x, int y, Stone* _s[], bool _redSide)
{
	//兵的走棋规则： 
	//1、一次走一格 
	//2、前进一格后不能后退 
	//3、过河后才可以左右移动 
	//通过棋子的ID得到棋子 
	Stone* s = _s[moveid];
	//获得将当前的位置 
	int xo = s->getX();
	int yo = s->getY();
	//获得兵走的格数 
	//(x,y)表示将走到的位置 
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	//走将的时候有两种情况 
	//xoff=1, yoff=0：将向左或向右 
	//xoff=0, yoff=1：将向前或向后 
	if (d != 1 && d != 10)
	{
		return false;
	}
	//如果玩家的棋子是红棋 
	if (_redSide == s->getRed())
	{
		//限制红色的兵不能后退 
		if (y < yo)
		{
			return false;
		}
		//红色的兵没有过河不能左右移动 
		if (yo <= 4 && y == yo)
		{
			return false;
		}
	}
	else//判断黑色的兵 
	{
		//限制黑色的兵不能后退 
		if (y > yo)
		{
			return false;
		}
		//黑色的兵没有过河不能左右移动 
		if (yo >= 5 && y == yo)
		{
			return false;
		}
	}
	return true;
}

//计算(xo,yo)和(x,y)之间的棋子数 
//如果棋子数为-1,表示(xo,yo)和(x,y)不在一条直线上 
int SRule::getStoneCount(int xo, int yo, int x, int y, Stone* _s[], bool _redSide)
{
	int ret = 0;//记录两点之间的棋子的个数 
	//(xo,yo)和(x,y)不在同一条直线上 
	if (xo != x && yo != y)
	{
		return -1;
	}
	//(xo,yo)和(x,y)在同一点上 
	if (xo == x && yo == y)
	{
		return -1;
	}
	//两点在同一条竖线上 
	if (xo == x)
	{
		//min为两个点中y坐标最小的点的y坐标 
		int min = yo < y ? yo : y;
		//max为两个点中y坐标最大的点的y坐标 
		int max = yo > y ? yo : y;
		//查找同一条竖线上两点之间的棋子数 
		for (int yy = min + 1; yy<max; yy++)
		{
			//当两点之间有棋子的时候 
			if (getStone(x, yy, _s, _redSide) != -1)
			{
				++ret;//棋子数加1 
			}
		}
	}
	else//两点在同一条横线上yo == y 
	{
		//min为两个点中x坐标最小的点的x坐标 
		int min = xo < x ? xo : x;
		//max为两个点中x坐标最大的点的x坐标 
		int max = xo > x ? xo : x;
		//查找同一条竖线上两点之间的棋子数 
		for (int xx = min + 1; xx<max; xx++)
		{
			//当两点之间有棋子的时候 
			if (getStone(xx, y, _s, _redSide) != -1)
			{
				++ret;//棋子数加1 
			}
		}
	}
	//返回两点之间的棋子数 
	return ret;
}

//通过坐标的下标获取棋子的ID 
//如果坐标上没有棋子,返回-1 
int SRule::getStone(int x, int y, Stone* _s[], bool _redSide)
{
	Stone* s;
	//遍历32个棋子 
	for (int i = 0; i<32; i++)
	{
		s = _s[i];
		if (s->getX() == x && s->getY() == y && !s->getDead())
		{
			//得到棋子的ID 
			return s->getID();
		}
	}

	return -1;
}


list<Move> SRule::listMove(int mid, Stone* _s[], bool redSide)
{
	Stone* s = _s[mid];
	list<Move> ret;
	ret.clear();
	if (s->getDead())
	{
		return ret;
	}
	//棋子的类型 
	switch (s->getType())
	{
		//将的走棋规则 
		case Stone::JIANG:
		case Stone::BING:
			ret = listMoveJiang(mid, _s, redSide);
		break;
		//士的走棋规则 
		case Stone::SHI:
			ret = listMoveShi(mid, _s, redSide);
		break;
		//相的走棋规则 
		case Stone::XIANG:
			ret = listMoveXiang(mid, _s, redSide);
		break;
		//车的走棋规则 
		case Stone::JU:
		case Stone::PAO:
			ret = listMovePao(mid, _s, redSide);
		break;
		//马的走棋规则 
		case Stone::MA:
			ret = listMoveMa(mid, _s, redSide);
		break;
		default:
			break;
	}
	list<Move> xret; xret.clear();
	for (auto m : ret){
		if (m.killid != -1)
			xret.push_back(m);
	}
	for (auto m : ret){
		if (m.killid == -1)
			xret.push_back(m);
	}
	return xret;
}

list<Move> SRule::listMovePao(int mid, Stone* _s[], bool redSide)
{
	list<Move> ret; ret.clear();
	int x = _s[mid]->getX();
	int y = _s[mid]->getY();
	for (int d = -9; d <= 9; d++)
	{
		int nx = x + d;
		int ny = y + d;
		if (nx >= 0 && nx <= 8)
		{
			int kid = getStone(nx, y, _s, redSide);
			if (canMove(mid, kid, nx, y, _s, redSide))
				ret.push_back(Move(mid, kid, nx, y));
		}
		if (ny >= 0 && ny <= 9)
		{
			int kid = getStone(x, ny, _s, redSide);
			if (canMove(mid, kid, x, ny, _s, redSide))
				ret.push_back(Move(mid, kid, x, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveShi(int mid, Stone* _s[], bool redSide)
{
	list<Move> ret; ret.clear();
	int x = _s[mid]->getX();
	int y = _s[mid]->getY();
	int dx[] = { -1, -1, 1, 1 };
	int dy[] = { -1, 1, -1, 1 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s, redSide);
			if (canMove(mid, kid, nx, ny, _s, redSide))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveJiang(int mid, Stone* _s[], bool redSide)
{
	list<Move> ret; ret.clear();
	int x = _s[mid]->getX();
	int y = _s[mid]->getY();
	int dx[] = { 0, -1, 0, 1 };
	int dy[] = { -1, 0, 1, 0 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s, redSide);
			if (canMove(mid, kid, nx, ny, _s, redSide))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveXiang(int mid, Stone* _s[], bool redSide)
{
	list<Move> ret; ret.clear();
	int x = _s[mid]->getX();
	int y = _s[mid]->getY();
	int dx[] = { -2, -2, 2, 2 };
	int dy[] = { -2, 2, -2, 2 };
	for (int d = 0; d < 4; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s, redSide);
			if (canMove(mid, kid, nx, ny, _s, redSide))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}

list<Move> SRule::listMoveMa(int mid, Stone* _s[], bool redSide)
{
	list<Move> ret; ret.clear();
	int x = _s[mid]->getX();
	int y = _s[mid]->getY();
	int dx[] = { -2, -2, -1, -1, 1, 1, 2, 2 };
	int dy[] = { -1, 1, -2, 2, -2, 2, -1, 1 };
	for (int d = 0; d < 8; d++)
	{
		int nx = x + dx[d];
		int ny = y + dy[d];
		if (nx >= 0 && nx <= 8 && ny >= 0 && ny <= 9)
		{
			int kid = getStone(nx, ny, _s, redSide);
			if (canMove(mid, kid, nx, ny, _s, redSide))
				ret.push_back(Move(mid, kid, nx, ny));
		}
	}
	return ret;
}