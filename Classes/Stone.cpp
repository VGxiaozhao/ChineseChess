#include "Stone.h"
 
Stone::Stone()
{
}

//创建棋子 
//第一个参数：棋子的类型 
//第二个参数：棋子的颜色 
Stone* Stone::create(int id, bool red)
{
    Stone* s = new Stone();
    s->init(id, red);
    s->autorelease();
      
    return s;
}

//摆放棋子 
//参数：用于判断游戏玩家选择进入游戏场景时是否 
//点了红色的棋子 
void Stone::reset(bool red)
{
   //刚摆棋子的时候棋子没被吃掉 
   this->setDead(false);
   if(red)//当玩家点红色的棋子进入游戏场景时 
    {//玩家的棋子的颜色为红色 
        if(_id < 16)//摆放红色的棋子 
        {
            //设置棋子的x坐标 
            this->setX(_initPos[_id]._x);
            //设置棋子的y坐标 
            this->setY(_initPos[_id]._y);
        }
        else//摆放黑色的棋子 
        {
            this->setX(8 - _initPos[_id-16]._x);
            //两个类型相同颜色不同的棋子的y坐标相差9 
            //如：红色的车和黑色的车的y坐标相差9 
            this->setY(9 - _initPos[_id-16]._y);
        }
    }
    else//当玩家点黑色的棋子进入游戏场景时 
    {//玩家的棋子的颜色是黑色 

          if(_id < 16)//摆放黑色的棋子 
        {
            this->setX(8 - _initPos[_id]._x);
            this->setY(9 - _initPos[_id]._y);
        }
        else//摆放红色的棋子 
        {
            //两个类型相同颜色不同的棋子的id相差16 
            //如：红色的车和黑色的车的id相差16 
            this->setX(_initPos[_id-16]._x);
            this->setY(_initPos[_id-16]._y);
        }
    }
}

//初始化棋子 
//象棋的一次性初始化 
bool Stone::init(int id, bool red)
{
    _id = id;//获得棋子的id 
    //当棋子的id小于16时,棋子是红色的 
    _red = _id < 16;
    //初始化红色的棋子 
    if(_id < 16)
    {
        _type = _initPos[_id]._type;
    }
    else//初始化黑色的棋子 
    {
        //类型(车、马、炮)相同颜色不同的两个棋子的id相差16 
        _type = _initPos[_id-16]._type;
    }

	const char* stonePic[14] = {
		"rbing.png",  //(红色)兵 
        "rshi.png",   //(红色)士 
        "rxiang.png", //(红色)相 
        "rma.png",    //(红色)马 
        "rpao.png",   //(红色)炮 
		"rche.png",   //(红色)车 
		"rshuai.png", //(红色)帅 

		"bzu.png",    //(黑色)卒 
        "bshi.png",   //(黑色)士 
        "bxiang.png", //(黑色)相 
        "bma.png",    //(黑色)马 
        "bpao.png",   //(黑色)炮 
		"bche.png",   //(黑色)车 
		"bjiang.png"  //(黑色)将 
        };

    //计算图片的下标 
    //如果棋子是红色的 idx = _type 
    //如果棋子是黑色的 idx = 7 + _type 
    //两个棋子的颜色的下标相差 7 
    int idx = (_red ? 0 : 1) * 7 + _type;
    //创建精灵(创建棋子) 
    Sprite::initWithFile(stonePic[idx]);
    //压缩棋子 
    setScale(.8f);
    //摆棋子(设置棋子的位置) 
    reset(red);
	//计算子力分数
	_score = calculate(this->getType());

	return true;
}

//创建一张表 
Stone::InitPos Stone::_initPos[16] =
{
    //车的位置(0,0) 
    {0, 0, Stone::JU},
    //马的位置(1,0) 
    {1, 0, Stone::MA},
    //相的位置(2,0) 
    {2, 0, Stone::XIANG},
    //士的位置(3,0) 
    {3, 0, Stone::SHI},
    //将的位置(4,0) 
    {4, 0, Stone::JIANG},
    //士的位置(5,0) 
    {5, 0, Stone::SHI},
    //相的位置(6,0) 
    {6, 0, Stone::XIANG},
    //马的位置(7,0) 
    {7, 0, Stone::MA},
    //车的位置(8,0) 
    {8, 0, Stone::JU},
    //炮的位置(1,2) 
    {1, 2, Stone::PAO},
    //炮的位置(7,2) 
    {7, 2, Stone::PAO},
    //兵的位置(0,3) 
    {0, 3, Stone::BING},
    //兵的位置(2,3) 
    {2, 3, Stone::BING},
    //兵的位置(4,3) 
    {4, 3, Stone::BING},
    //兵的位置(6,3) 
    {6, 3, Stone::BING},
    //兵的位置(8,3) 
    {8, 3, Stone::BING},
};

int Stone::getScore()
{
	return _score + getLoc();
}

int Stone::calculate(Stone::TYPE type)
{
	int ret = 0;
	if (type == Stone::JIANG)
		ret = 1000;
	else if (type == Stone::BING)
		ret = 2;
	else if (type == Stone::JU)
		ret = 10;
	else if (type == Stone::PAO)
		ret = 5;
	else if (type == Stone::MA)
		ret = 4;
	else if (type == Stone::SHI)
		ret = 2;
	else if (type == Stone::XIANG)
		ret = 2;
	return ret;
}

int Stone::getLoc()
{
	Stone::TYPE type = getType();
	if (type == Stone::JIANG)
	{
		if (_x == 4) return 4;
		else if (_y == 0 || _y == 2 || _y == 9 || _y == 7)
			return 3;
		else
			return 2;
	}
	else if (type == Stone::BING)
	{
		if (_x == 0 || _x == 8) return 1;
		else if (_x >= 3 && _x <= 5) {
			//注意这里！！
			if (_y > 4 && getRed() == true)
				return 3;
			else if (_y <= 4 && getRed() == false)
				return 3;
		}
		else{
			if (_y > 4 && getRed() == true)
				return 2;
			else if (_y <= 4 && getRed() == false)
				return 2;
		}
	}
	else if (type == Stone::JU)
	{
		if (_x == 0 && (_y == 0 || _y == 9))
			return 15;
		else if (_x == 8 && (_y == 0 || _y == 9))
			return 15;
		else
			return 17;
	}
	else if (type == Stone::PAO)
		return 17;
	else if (type == Stone::MA)
	{
		if (_x == 0 || _x == 8)
		{
			if (_y<2 || _y>7)
				return 2;
			else
				return 3;
		}
		else
			return 6;
	}
	else if (type == Stone::SHI)
	{
		if (_x == 4)
			return 2;
		else
			return 1;
	}
	else if (type == Stone::XIANG)
	{
		if (_x == 4)
			return 2;
		else
			return 1;
	}
	return 0;
}