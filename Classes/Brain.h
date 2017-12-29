#ifndef  _BRAIN_H
#define  _BRAIN_H

#include "Stone.h"
#include "cocos2d.h"

class CBrain
{
public:
	CBrain(bool red, Stone* s[32], bool redSide);
	virtual ~CBrain();
	Move think();
	void disorder();
	Move testAI();
	void dfs(bool turn, int sum, int depth, bool& out, int& best, int& worst);
	Move Alpah();
	int  getScore(int id);
	void moveStone(Move m);
	void reverseMove(Move m);
	Move makeRevMove(Move m);
	int alphaBetaSearch(int turn, int depth, int alpha, int beta);
	Move fuck();
	bool isGameOver(bool turn);
	void evaluate(int& myturn,int& yourturn);
private:
	Stone* _s[32];
	bool _turn;
	bool _redSide;
	int _xulie[32];
	int _best, _worst, _depth;
	Move _bestMove;
	int _myturn, _yourturn;
};


#endif