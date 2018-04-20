#include "SceneStart.h"
#include "SceneGame.h"

Scene* SceneStart::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneStart::create();
    scene->addChild(layer);

    return scene;
}

bool SceneStart::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto winSize = Director::getInstance()->getVisibleSize();
    auto bkr = Sprite::create("bkg2.png");
    addChild(bkr);
    auto bkb = CCSprite::create("bkg1.png");
    addChild(bkb);
    bkr->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2));
    bkb->setPosition(ccp(winSize.width / 2 + 100, winSize.height / 2));
    _red = bkr;
    _black = bkb;
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch *t,Event *e){
        auto winSize = Director::getInstance()->getVisibleSize();
        //��ô������λ��(����) 
        auto ptClick = t->getLocation();
        //�����ж��Ƿ���������� 
        bool bClickStone = false;
        //�����к�ɫ�����ӵ�ʱ��(�������λ���ں�ɫ���������ڵķ�Χ��) 
        if(_red->boundingBox().containsPoint(ptClick))
        {
            //�����˺�ɫ������ 
            this->_selected = true;
            //���������� 
            bClickStone = true;
        }
        //�����к�ɫ���ӵ�ʱ��(�������λ���ں�ɫ�������ڵķ�Χ��) 
        else if(_black->boundingBox().containsPoint(ptClick))
        {
            //û���к�ɫ���� 
            this->_selected = false;
            //���������� 
            bClickStone = true;
        }
        //�����������ӵ�ʱ�� 
        if(bClickStone)
        {
            //�ƶ����� 
            auto moveTo1 = MoveTo::create(1, ccp(winSize.width / 2, winSize.height / 2));
            auto moveTo2 = MoveTo::create(1, ccp(winSize.width / 2, winSize.height / 2));
            //��ת���� 
            auto rotate1 =  RotateBy::create(1, 360);
            auto rotate2 =  RotateBy::create(1, -360);
            //��ת���ƶ�ͬʱִ�� 
            auto spawn1 = Spawn::create(moveTo1, rotate1, NULL);
            auto spawn2 = Spawn::create(moveTo2, rotate2, NULL);
            //ִ���ж��� 
            _red->runAction(spawn1);
            _black->runAction(spawn2);
            //������ʱ�� 
            scheduleUpdate();
        }
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,_red);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(),_black);
   
    return true;
}

void SceneStart::update(float)
{
    //��ȡ�������ӵ�x����
    float x1 = _red->getPositionX();
    float x2 = _black->getPositionX();
    //����ɫ�����Ӻͺ�ɫ��������ײ�� 
    //�������ӵľ���С�����ӵ�ֱ�� 
    //getContentSize().width������ӵĿ��(���ӵ�ֱ��) 
    if(abs(x1 - x2) <= _red->getContentSize().width)
    {
        //������Ϸ 
        Director::getInstance()->replaceScene(SceneGame::createScene(this->_selected));
    }
}