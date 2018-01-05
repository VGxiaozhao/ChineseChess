#ifndef  _BRAIN_H
#define  _BRAIN_H

#include "Stone.h"
#include "cocos2d.h"

class CBrain
{
public:
	CBrain(bool red, Stone* s[32]);
	virtual ~CBrain();
	Move think();
	void disorder();
	int  getScore(int id);
	void moveStone(Move m);
	void reverseMove(Move m);
	Move makeRevMove(Move m);
	bool isGameOver();
	void evaluate(int& myturn,int& yourturn);
	int  alphaBetaSearch(bool turn, int depth, int alpha, int beta);
private:
	Stone* _s[32];
	bool _turn;
	bool _redSide;
	int _xulie[32];
	int _myturn, _yourturn;
	int _depth;
	Move _bestMove;
};


#endif