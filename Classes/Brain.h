#ifndef  _BRAIN_H
#define  _BRAIN_H

#include "Stone.h"
#include "cocos2d.h"
#include <string>
#include "HashMap.h"

class CBrain
{
public:
	CBrain(bool red, Stone* s[32]);
	CBrain();
	virtual ~CBrain();
	Move think();
	void disorder();
	void moveStone(Move m);
	void reverseMove(Move m);
	Move makeRevMove(Move m);
	bool isGameOver();
	int  alphaBetaSearch(bool turn, int depth, int alpha, int beta, int a[]);
	void alphaBetaSearch();
	std::string getCurJu();
	unsigned long long getJuMianLL();
	Move int2move(int);
	void setPad(Stone* s[32], bool turn);
	void init();
	int quies(bool turn, int depth, int alpha, int beta);
	int evaluate(bool);
	int evaluate(int);
private:
	Stone* _s[32];
	bool _turn;
	bool _redSide;
	int _xulie[32];
	int _depth;
	Move _bestMove;
	int _searchCount;
	int _hashcnt;
	int _firstScore;
	CHashMap *mp[6];
	Stone before[32];
};


#endif