#include "nodeStarLinkList.h"

void nodeStarLinkList::insert(node* nval) {
	linkList *nNode = new linkList;
	nNode->val = nval;
	nNode->next = head.next;
	head.next = nNode;
	len ++;
}
