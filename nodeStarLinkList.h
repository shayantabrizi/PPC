#pragma once
#include "node.h"

class nodeStarLinkList
{
public:
	struct linkList {
		node *val;
		linkList *next;
	};

	linkList head;
	int len;

	void insert(node*);
};

