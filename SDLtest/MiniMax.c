#include "MiniMax.h"

#include "MiniMax.h"
#include <stdlib.h>
#include "MiniMaxWithAlphaBeta.h"

struct MiniMaxResult getBestChild(
	void* state,
	unsigned int maxDepth,
	ListRef(*getChildren) (void* state),
	FreeFunc freeState,
	int(*evaluate) (void* state),
	int isMaxPlayer) {
	 
	int alpha = MIN_EVALUATION - 1;
	int beta = MIN_EVALUATION + 1;
	return  getBestChildWithAlphaBeta(state, maxDepth, getChildren, freeState, evaluate, isMaxPlayer, alpha, beta);

}

struct MiniMaxResult getBestChildWithAlphaBeta(
	void* state,
	unsigned int maxDepth,
	ListRef(*getChildren) (void* state),
	FreeFunc freeState,
	int(*evaluate) (void* state),
	int isMaxPlayer,
	int alpha,
	int beta) {

	struct MiniMaxResult result;
	ListRef children = getChildren(state);
	result.index = 0; // should be changed by calling function, assigned here to be on the safe side.
	if (maxDepth == 0 || isEmpty(children)) { // list is leaf
		result.value = evaluate(state);
	}
	else { // list is internal node
		ListRef templist = children;
		int maxvalue = MIN_EVALUATION;
		int maxindex = 0;
		int minvalue = MAX_EVALUATION;
		int minindex = 0;
		int childindex = 0;
		while (templist != NULL) { // calling recursive function on all child lists and finding min or max
			//printf("depth is %d, board is %c", maxDepth, )
			struct MiniMaxResult childresult = getBestChildWithAlphaBeta(headData(templist), maxDepth - 1, getChildren, freeState, evaluate, !isMaxPlayer,alpha,beta);
			childresult.index = childindex;
			if (childresult.value > maxvalue) {
				maxvalue = childresult.value;
				maxindex = childresult.index;
				alpha = MAX(alpha, childresult.value);
				if (beta <= alpha) { //(* beta cut-off *) - prune subTree
					break;
				}
			}
			if (childresult.value < minvalue) {
				minvalue = childresult.value;
				minindex = childresult.index;
				beta = MIN(beta, childresult.value);
				if (beta <= alpha) { //(* alpha cut-off *) - prune subTree
					break;
				}
			}
			++childindex;
			templist = tail(templist);
		}
		if (isMaxPlayer == 0) {
			result.value = beta;
			result.index = minindex;
		}
		else {
			result.value = alpha;
			result.index = maxindex;
		}
	}

	destroyList(children, freeState);
	return result;
}