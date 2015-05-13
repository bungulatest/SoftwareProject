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
	int beta = MAX_EVALUATION + 1;
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
		

		if (isMaxPlayer) {
			result.value = MIN_EVALUATION - 1;
		}
		else {
			result.value = MAX_EVALUATION + 1;
		}

		int childindex = 0;
		while (templist != NULL) { // calling recursive function on all child lists and finding min or max
			//printf("depth is %d, board is %c", maxDepth, )
			struct MiniMaxResult childresult = getBestChildWithAlphaBeta(headData(templist), maxDepth - 1, getChildren, freeState, evaluate, !isMaxPlayer,alpha,beta);
			
			if ((isMaxPlayer && childresult.value > result.value) || (!isMaxPlayer && childresult.value < result.value)) {
				result.value = childresult.value;
				result.index = childindex;
			}

			if (isMaxPlayer) {
				alpha = MAX(alpha, childresult.value);
				if (alpha >= beta) {
					break;
				}
			}
			else {
				beta = MIN(beta, childresult.value);
				if (alpha >= beta) {
					break;
				}
			}

			++childindex;
			templist = tail(templist);
		}
	}

	destroyList(children, freeState);
	return result;
}