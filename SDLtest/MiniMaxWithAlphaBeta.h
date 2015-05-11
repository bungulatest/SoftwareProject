#ifndef MiniMaxWithAlphaBeta_h__
#define MiniMaxWithAlphaBeta_h__
#include "MiniMax.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct MiniMaxResult getBestChildWithAlphaBeta(
	void* state,
	unsigned int maxDepth,
	ListRef(*getChildren) (void* state),
	FreeFunc freeState,
	int(*evaluate) (void* state),
	int isMaxPlayer,
	int alphal,
	int beta);

#endif // MiniMaxWithAlphaBeta_h__
