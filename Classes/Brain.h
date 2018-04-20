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
	//����һ�����
	Move think();
	bool findKillerMove(Move & m, bool);
	Move alphaBetaSearchByTime(int second);
	//��������˳��
	void disorder();
	//�������������ƶ�����
	void moveStone(Move m);
	//�����������г�����һ���ƶ�����
	void reverseMove(Move m);
	//��Ҫ��������
	int  alphaBetaSearch(bool turn, int depth, int alpha, int beta);
	//�ŷ�����ͨ��SLģ�����ɣ�beta�׶Σ�Ч�������룬������Ҫ������ģ��Ч��̫��
	Move alphaBetaSearchWithSL(int depth);
	int alphaBetaSearchWithSL(bool turn, int depth, int alpha, int beta);
	//���ⲿ���õ�AlphaBeta�㷨
	Move alphaBetaSearch(int);
	void setPad(Stone* s[32], bool turn);
	void init();
	//�����������Ƽ��ŷ�NN��������
	Move getNNMove();
	//��̬�����㷨�����ڽ��������ƽ��ЧӦ
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