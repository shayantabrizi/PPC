#include "node.h"
using namespace std;

void node::reset() {
	seed = false;
	group = 2;
	score = 0;
	walkingNode.frequency = 0;
	//fibNode = NULL;
}

inline void node::linkList::erase() {
#ifndef PAPER
	previous->next = next;
	if (next != NULL)
		next->previous = previous;
	delete(this);
#endif
}

inline node::linkList* node::linkList::insert(nodeFrequency& newNode) {
	linkList *ilinkList = new linkList;
	ilinkList->inodeFrequency = newNode;
	ilinkList->next = next;
#ifndef PAPER
	ilinkList->previous = this;
	if (next != NULL)
		next->previous = ilinkList;
#endif
	next = ilinkList;
	return ilinkList;
}

void node::eraseFromWalkedNode(node::linkList* target) {
	target->erase();
	//walkedNodesLenght --;
}

node::linkList* node::insertWalkingNodeToWalkedNode() {
	//walkedNodesLenght ++;

	linkList *insertedLinkList = walkedNodes1->insert(walkingNode);
	insertedLinkList->inode1 = this;
	return insertedLinkList;
}
#ifndef PAPER
void node::updateWalkedNodes() {
	linkList *link = walkedNodes1;
	while (link->next != NULL) {
		link->next->inodeFrequency.inode->score += link->next->inodeFrequency.frequency;
		link = link->next;
	}
}
#endif
node::node(void)
{
	seed = false;
	fibNode = NULL;
	group = 2;
	score = 0;
	walkingNode.frequency = 0;
	walkedNodes1 = new linkList;
	walkedNodes1->next = NULL;
}

node::~node(void)
{
	delete[](connectedNodes);
#ifdef WE
	delete[](weight);
#endif
	delete(walkedNodes1);
}
