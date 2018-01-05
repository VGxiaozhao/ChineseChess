#include "SceneGame.h"
#include "SRule.h"
SceneGame::SceneGame()
{
}

Scene* SceneGame::createScene(bool red)
{
    auto scene = Scene::create();
    auto layer = SceneGame::create(red);
    scene->addChild(layer);

    return scene;
}

//�Զ���create���� 
SceneGame* SceneGame::create(bool red)
{
    SceneGame* pRet = new SceneGame();
    if(pRet)
    {
        if(pRet->init(red))
        {
            pRet->autorelease();
        }
        else
        {
            pRet->release();
            return NULL;
        }
    }
    else
    {
        return NULL;
    }

    return pRet;
}

bool SceneGame::init(bool red)
{
    //���ø���Layer 
    if ( !Layer::init() )
    {
        return false;
    }
    auto winSize = Director::getInstance()->getWinSize();
    //�������̵�ƫ��ֵ 
    this->_plateOffset = ccp(20,10);
    //�������ӵ�ƫ��ֵ 
    this->_stoneOffset = ccp(60, 33);
    //�������ӵ�ֱ��Ϊ46 
	this->_d = 46;
    //��ʼ��ʱ��û��ѡ������ 
    _selectid = -1;
    //����ʱ���л����ӵ���ɫ 
    _redTurn = true;
    //��������Ұں��� 
    _redSide = red;
    //�������� 
    auto desk = Sprite::create("floor.jpg");
    this->addChild(desk);
    //�������ӵ�λ�� 
    desk->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    //ѹ������ 
    desk->setScaleX(winSize.width / desk->getContentSize().width);
    desk->setScaleY(winSize.height / desk->getContentSize().height);
    //�������� 
    auto plate = Sprite::create("background.png");
    this->addChild(plate);
    //�������Ϊ(0,0) 
    plate->setAnchorPoint(Vec2::ZERO);
    //�������̵�λ�� 
    plate->setPosition(_plateOffset);
    //ѹ�����̣�(���ڵĸ߶� - ƫ�Ƶ�y���� * 2) / ͼƬ�ĸ߶� 
    plate->setScale((winSize.height -_plateOffset.y *2)/ plate->getContentSize().height);
    //������ 
    for(int i=0; i<32; i++)
    {
        //�������� 
        _s[i] = Stone::create(i, red);
        addChild(_s[i]);
        //�������ӵĳ�ʼλ��Ϊ���λ�� 
        _s[i]->setPosition(ccp(CCRANDOM_0_1() * winSize.width, CCRANDOM_0_1() * winSize.height));
        //�������� 
        _s[i]->setVisible(false);
    }
    //����һ��ѡ��� 
    //��ѡ��ĳ�����ӵ�ʱ��,ѡ��������ѡ�õ������� 
    _selectSprite = Sprite::create("selected.png");
    addChild(_selectSprite);
    //����ѡ��� 
    _selectSprite->setVisible(false);
    //����ѡ�������ȼ� 
    _selectSprite->setZOrder(1000);
    //ѹ��ѡ��� 
    _selectSprite->setScale(.8f);
    //����Menu 
    auto menu = Menu::create();
    this->addChild(menu);
    //������ʼ��ť 
    auto itemStart = MenuItemImage::create("start.jpg", "start.jpg", this, menu_selector(SceneGame::Start));
    menu->addChild(itemStart);
    itemStart->setPositionX(190);
    itemStart->setPositionY(120);
    //�����¾ְ�ť 
    auto itemNew = MenuItemImage::create("new.jpg", "new.jpg", this, menu_selector(SceneGame::New));
    menu->addChild(itemNew);
    itemNew->setPositionX(itemStart->getPositionX());
    itemNew->setPositionY(itemStart->getPositionY() + 60);
    //�������尴ť 
    auto item = MenuItemImage::create("regret.jpg", "regret.jpg", this, menu_selector(SceneGame::Back));
    menu->addChild(item);
    item->setPositionX(itemStart->getPositionX());
    item->setPositionY(itemStart->getPositionY() - 60);
    //������ͣ��ť 
    auto itemPause = MenuItemImage::create("pause.jpg", "pause.jpg", this, menu_selector(SceneGame::Pause));
    menu->addChild(itemPause);
    itemPause->setPositionX(itemStart->getPositionX());
    itemPause->setPositionY(itemStart->getPositionY() - 60 - 60);
    //�����ѶȰ�ť 
    auto itemDifficulty = MenuItemImage::create("difficulty.jpg", "difficulty.jpg", this, menu_selector(SceneGame::Difficulty));
    menu->addChild(itemDifficulty);
    itemDifficulty->setPositionX(itemStart->getPositionX());
    itemDifficulty->setPositionY(itemStart->getPositionY() - 60 - 60 - 60);
    //�������ű������ְ�ť 
    auto itemVoice = MenuItemImage::create("openVolice.png", "closeVolice.png", this, menu_selector(SceneGame::Voice));
    menu->addChild(itemVoice);
    itemVoice->setPositionX(itemStart->getPositionX());
    itemVoice->setPositionY(itemStart->getPositionY() - 60 - 60 - 60 - 60);

    //CCLog("x=%lf", itemStart->getPositionX());
    //CCLog("y=%lf", itemStart->getPositionY() - 240);

    //����һ������,�����Դ�Ϊ���������  
    auto label = LabelTTF::create("Voice", "Arial", 25);  
    addChild(label);
    //�������ֵ�λ��  
    label->setPosition(ccp(winSize.width/2 + 120, winSize.height/2 - 120));
    //�������ֵ���ɫ 
    label->setColor(ccc3(0, 0, 0));
    //�������� 
    _steps = Array::create();
    _steps->retain();
    //����������ʾ��Ϸ��� 
    sprite = Sprite::create("yingjiemian.png");
    addChild(sprite);
    sprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    //���ؽ�� 
    sprite->setVisible(false);
    visible = false;

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch *t,Event *e){
        Object* obj = (Object*)t;
        //��ȡ������Ĵ������� 
        Point ptInWin = t->getLocation();
        if(sprite->boundingBox().containsPoint(ptInWin) && visible == true)  
        {  
            //������Ϸ��� 
            sprite->setVisible(false);
            visible = false;
            New(obj);
        }
        int x, y;//���津������������� 
        //ͨ��������Ĵ��������ȡ���̵�x�����y���� 
        if(!getClickPos(ptInWin, x, y))
        {
            return false;
        }
        //ͨ���������������е������ȡѡ�е����ӵ�id 
        int clickid = getStone(x, y);
        //���������λ���������ӵ�ʱ��,clickidΪѡ�е����ӵ�id,��ʾ�����ѡ�� 
        //���������λ����û�����ӵ�ʱ��,clickidΪ-1,��ʾ��������� 

        //-1 == _selectid��ʾ��ǰû��ѡ������ 
        if(-1 == _selectid)
        {
            setSelectId(clickid);
        }
        else
        {
            //�ƶ����� 
            //��һ���������ƶ������ӵ�id 
            //�ڶ���������ͨ���������λ���жϴ��������Ƿ������� 
            //�������������������x���� 
            //���ĸ��������������y���� 
            //moveStoneִ������������ѡ������� 
            //ѡ���ӣ���_selectid == clickidʱ����ʾѡ����idΪ_selectid������ 
            //�����ӣ���selectid != clickidʱ�� ��ʾ��idΪ_selectid�������ƶ���(x,y)���ڵ�λ���� 
            moveStone(_selectid, clickid, x, y);
        }

        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    return true;
}

//�õ�������������ϵ������ 
//���������λ���������ⷵ��false 
//ͨ�������������������� 
bool SceneGame::getClickPos(Point ptInWin, int &x, int &y)
{
    for(x=0; x<=8; x++)
    {
        for(y=0; y<=9; y++)
        {
            //���������ϵĸ����ڴ����ϵ�λ�� 
            Point ptInPlate = getStonePos(x, y);
            // CCLog("ptInPlate.x=%lf   ptInPlate.y=%lf", ptInPlate.x,  ptInPlate.y);
            //Ѱ�����������λ�þ���С�����ӵİ뾶�ĸ��� 
            //����ҵ���,return true,���򷵻� return false 
            if(ptInWin.getDistance(ptInPlate) < _d / 2)
            {
                return true;
            }
        }
    }

    return false;
}

//ͨ��������±��ȡ���ӵ�ID 
//���������û������,����-1 
int SceneGame::getStone(int x, int y)
{
    Stone* s;
    //����32������ 
    for(int i=0; i<32; i++)
    {
        s = _s[i];
        if(s->getX() == x && s->getY() == y && !s->getDead())
        {
            //�õ����ӵ�ID 
            return s->getID();
        }
    }

	return -1;
}

//����ѡ��� 
void SceneGame::setSelectId(int id)
{
    if(-1 == id)
    {
        return;
    }
    //���û��ѡ�к��� 
    if(_s[id]->getRed() != _redTurn)
    {
        return;
    }
    //_selectidΪѡ�е����ӵ�id 
    _selectid = id;
    //��ʾѡ��� 
    _selectSprite->setVisible(true);
    //ѡ����ڱ�ѡ�е���������ʾ 
    _selectSprite->setPosition(_s[_selectid]->getPosition());
}

//�ƶ����� 
//��һ���������ƶ������ӵ�id 
//�ڶ���������ͨ���������λ���жϴ��������Ƿ������� 
//�������������������x���� 
//���ĸ��������������y���� 
void SceneGame::moveStone(int moveId, int killId, int x, int y)
{
    //killId != -1��ʾ�������λ������һ������ 
    //_s[moveId]->getRed() == _s[killId]->getRed()��ʾ�������� 
    //�����Ӻ���������ӵ���ɫ��ͬ 
    if(killId != -1 && _s[moveId]->getRed() == _s[killId]->getRed())
    {
        //����ѡ��� 
        setSelectId(killId);
        return;
    }
    //������� 
    bool bCanMove =  SRule::canMove(moveId, killId, x, y, _s);
    //���bCanMoveΪfalse 
    //�������� 
    if(false == bCanMove)
    {
        return;
    }
    //����֮ǰ��¼���ӵ���Ϣ 
    //��һ����������Ҫ�ƶ������ӵ�id 
    //�ڶ���������ͨ���������λ���жϴ��������Ƿ������� 
    //���������������ӵ�ǰ��λ�õ�x���� 
    //���ĸ����������ӵ�ǰ��λ�õ�y���� 
    //����������������ƶ����λ�õ�x���� 
    //�����������������ƶ����λ�õ�y���� 
    Step* step = Step::create(moveId, killId, _s[moveId]->getX(), _s[moveId]->getY(), x, y);
    //�����ӵ���Ϣ���ӵ������� 
    _steps->addObject(step);
    //�������ӵ�����(�ƶ�����) 
    _s[moveId]->setX(x);
    _s[moveId]->setY(y);
    //_s[moveId]->setPosition(getStonePos(x,y));
    //SetRealPos(_s[moveId]);
    //�����ƶ�����ʱ�Ķ��� 
    auto move = MoveTo::create(.5f, getStonePos(x, y));
    //�����ص� 
    auto call = CCCallFuncND::create(this, callfuncND_selector(SceneGame::moveComplete), (void*)(intptr_t)killId);
    //���ö�����ִ��˳��(���ƶ�����,����ûص�����) 
    auto seq = Sequence::create(move, call, NULL);
    //�����ƶ������ӵ����ȼ� 
    _s[moveId]->setZOrder(_s[moveId]->getZOrder() + 1);
    //ִ�������ƶ��Ķ��� 
    _s[moveId]->runAction(seq);
}

//����������ת���ɴ������� 
CCPoint SceneGame::getStonePos(int x, int y)
{
    int xx = x * _d;
    int yy = y * _d;

    return ccp(xx, yy) + _stoneOffset;
}

//ʵ�ֻ��� 
void SceneGame::Back(Object*)
{
    //�������е�Ԫ�ظ���Ϊ0ʱ 
    //û���� 
    if( 0 == _steps->count() )
    {
        return;
    }
    //��ȡ�����е����һ��Ԫ�� 
    //��ȡ����ʱ�����һ�����ӵ���Ϣ 
    Step* step = (Step*)_steps->lastObject();
    //�ָ����ӵ���Ϣ 
    //������������ǰ��λ��x���� 
    _s[step->_moveid]->setX(step->_xFrom);
    //������������ǰ��λ��y���� 
    _s[step->_moveid]->setY(step->_yFrom);
    _s[step->_moveid]->setPosition(getStonePos(step->_xFrom, step->_yFrom));
    //�ָ��Ե������� 
    if(step->_killid != -1)
    {
        //��ʾ�Ե������� 
        _s[step->_killid]->setVisible(true);

        //����Ե������� 
        _s[step->_killid]->setDead(false);
    }
    //�ƶ���һ����� 
    //�л��ƶ������ӵ���ɫ 
    _redTurn = ! _redTurn;
    //ɾ�������е����һ��Ԫ�� 
    //ɾ������ʱ���һ�����ӵ���Ϣ 
    _steps->removeLastObject();
}

 //ʵ�ֿ�ʼ 
void SceneGame::Start(Object*)
{
    //������ 
    for(int i=0; i<32; i++)
    {
        //��ʾ���� 
        _s[i]->setVisible(true);
        //�������ƶ���������ָ����λ�� 
        auto move = MoveTo::create(1, this->getStonePos(_s[i]->getX(), _s[i]->getY()));
        _s[i]->runAction(move);
    }
}

//ʵ���¾� 
void SceneGame::New(Object* obj)
{
    //�õ����ڵĴ�С 
    auto winSize = Director::getInstance()->getWinSize();
    //���˶��ٲ���ͻڶ��ٲ����� 
    for(int i = _steps->count(); i>0; i--)
    {
        Back(obj);
    }
    for(int i=0; i<32; i++)
    {
        //�������ӵĳ�ʼλ��Ϊ���λ�� 
        _s[i]->setPosition(ccp(CCRANDOM_0_1() * winSize.width, CCRANDOM_0_1() * winSize.height));
        //�������� 
        _s[i]->setVisible(false); 
    }
}

//ʵ����ͣ 
void SceneGame::Pause(Object*)
{
}

//ʵ���Ѷ� 
void SceneGame::Difficulty(Object*)
{
}

//���ű������� 
void SceneGame::Voice(Object*)
{
    static int i = 0;
    if(0 == i)
    {
        //���ű������� 
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("floor.wav",true);
        i++;
    }
    else
    {
        //ֹͣ���ű������� 
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        i--;
    }
}

void SceneGame::moveComplete(Node* movestone, void* _killid)
{
    //�õ����ڵĴ�С 
    auto winSize = Director::getInstance()->getWinSize();
    //�������ȼ� 
    movestone->setZOrder(movestone->getZOrder() - 1);
    int killid =  (int)(intptr_t)_killid;
    //����������������� 
    if(-1 != killid)
    {
        //ɱ���������ϵ����� 
        _s[killid]->setDead(true);
        //����ɱ�������� 
        _s[killid]->setVisible(false);
        //��ɱ������ʱ��,���¿�ʼ 
        if(Stone::JIANG  == _s[killid]->getType())
        {
            //��ʾ��Ϸ��� 
            sprite->setVisible(true);
            //�������ȼ� 
            sprite->setZOrder(1000);
            visible = true;
        }
    }
    //û��ѡ������ 
    _selectid = -1;
    //����ѡ��� 
    _selectSprite->setVisible(false);
    //�ƶ���һ����� 
    //�л��ƶ������ӵ���ɫ 
    _redTurn = ! _redTurn;
	if (!_redTurn)
		callAI();
}

void SceneGame::moveStone(Move m)
{
	return moveStone(m.moveid, m.killid, m.x, m.y);
}

void SceneGame::callAI()
{
	/*int mid, x, y, kid;
	int kred, mred;
	do{
		int end = 16, start = 31;
		mid = (int)(CCRANDOM_0_1()*(end - start + 1) + start);
		x = (int)(CCRANDOM_0_1() * 8);
		y = (int)(CCRANDOM_0_1() * 9);
		kid = getStone(x, y);
		mred = _s[mid]->getRed();
		if (kid != -1)
			kred = _s[kid]->getRed();
		else
			kred = !mred;
	} while (!SRule::canMove(mid, kid, x, y, _s, _redSide));
	moveStone(mid, kid, x, y);*/
	CBrain b(_redTurn, _s);
	moveStone(b.think());
}
