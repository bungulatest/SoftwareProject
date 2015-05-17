#include "ListUtils.h"
#include <stdlib.h>

int fatalerror;

typedef struct List {
	void* data;
	ListRef next;
} List;

ListRef newList(void* headData) {
	ListRef list;
	list = (ListRef)malloc(sizeof(List)); //if malloc fails, list is NULL (not empty list)
	if (list == NULL) {
		fatalerror = 1;
		perror("ERROR: malloc failed\n");
		exit(1):
	}
	list->data = headData;
	list->next = NULL;
	return list;
}

int isEmpty(ListRef list) {
	if (list == NULL || list->data == NULL) { // C supports short-circuit evaluation therefore if list is NULL list->data will not be accessed
		return 1;
	}
	else {
		return 0;
	}
}

void* headData(ListRef list) {
	if (list == NULL) return NULL;
	else return list->data; // if list is empty than data is NULL
}

ListRef tail(ListRef list) {
	if (list == NULL) return NULL;
	return list->next;
}

ListRef append(ListRef list, void* data) {
	if (data == NULL) return NULL;
	if (isEmpty(list)) {
		list->data = data;
		return list;
	}
	else {
		ListRef newnode = newList(data);
		ListRef templist = list;
		while (templist->next != NULL) {
			templist = templist->next;
		}
		templist->next = newnode;
		return newnode; // might be NULL if 
		//failed on newList
	}
}

// recursively freeing the sub-lists and the data.
// assumes the list isn't very long and doesn't cause stack overflow
void destroyList(ListRef list, FreeFunc freeData) {
	if (list != NULL) {
		destroyList(list->next, freeData);
		if (list->data != NULL) freeData(list->data); // deal with edge case of empty list with NULL data
		free(list);
	}
}
