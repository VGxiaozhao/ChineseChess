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
	//��������˳��
	void disorder();
	//�������������ƶ�����
	void moveStone(Move m);
	//�����������г�����һ���ƶ�����
	void reverseMove(Move m);

	int  alphaBetaSearch(bool turn, int depth, int alpha, int beta);
	Move alphaBetaSearchWithSL(int depth);
	int alphaBetaSearchWithSL(bool turn, int depth, int alpha, int beta);
	Move alphaBetaSearch(int);
	std::string getCurJu();
	unsigned long long getJuMianLL();
	Move int2move(int);
	void setPad(Stone* s[32], bool turn);
	void init();
	Move getNNMove();
	int quies(bool turn, int depth, int alpha, int beta);
	int evaluate(bool);
	int evaluate(int);
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