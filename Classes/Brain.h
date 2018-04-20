#ifndef  _BRAIN_H
#define  _BRAIN_H

#include "Stone.h"
#include "cocos2d.h"
#include <string>
#include "Board.h"
#include "HashMap.h"

class CBrain
{
public:
	CBrain(bool red, Stone* s[32]);
	CBrain(CBoard);
	CBrain();
	virtual ~CBrain();
	//返回一个结果
	Move think();
	bool findKillerMove(Move & m, bool);
	Move alphaBetaSearchByTime(int second);
	//打乱序列顺序
	void disorder();
	//在搜索过程中移动棋子
	void moveStone(Move m);
	//在搜索过程中撤销上一步移动棋子
	void reverseMove(Move m);
	//主要搜索程序
	int  alphaBetaSearch(bool turn, int depth, int alpha, int beta);
	//着法生成通过SL模型生成，beta阶段，效果不理想，还是主要神经网络模型效果太差
	Move alphaBetaSearchWithSL(int depth);
	int alphaBetaSearchWithSL(bool turn, int depth, int alpha, int beta);
	//可外部调用的AlphaBeta算法
	Move alphaBetaSearch(int);
	void setPad(Stone* s[32], bool turn);
	void init();
	//获得神经网络的推荐着法NN即神经网络
	Move getNNMove();
	//静态搜索算法，用于解决搜索地平线效应
	int quies(bool turn, int depth, int alpha, int beta);
private:
	bool _turn;
	int _xulie[32];
	int _depth;
	Move _bestMove;
	int _searchCount;
	int _hashcnt;
	int _firstScore;
	int _alpha, _beta;
	CBoard* _board;
};


#endif