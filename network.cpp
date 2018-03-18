#include "network.h"
#include "randomNumberGenerator.h"
#include "fibpriv.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include "nodeStarLinkList.h"
using namespace std;

int network::trialNum;

node* network::candidateNode() {
	return mainGraph->nodes[randomNumberGenerator::nextInt() % (mainGraph->size)];
}

void network::partition2(bool testing) {
	struct fibheap *fh = fh_makekeyheap();
	node* lastSeed = candidateNode();
	lastSeed->seed = true;
	node* start = lastSeed;
	int i;
	node **order1 = new node*[mainGraph->size];
	order1[0] = lastSeed;
	int orderIndex = 1;	

	unsigned int generalIndex = 0;

	if (testing) {
		for (i = (*mainGraph).size - 1; i >= 0; i --) {
			order1[i] = mainGraph->nodes[i];
		}
		start = order1[0];
	}
#ifdef PAPER
	list<node::linkList*> jumpsList;
#endif
	for (i = (*mainGraph).size - 1; i >= 0; i--) {
		if (mainGraph->nodes[i]->seed || mainGraph->nodes[i]->degree == 0) {
			continue;
		}
		node *inode = mainGraph->nodes[i];
		node *saveNode = inode;
		trialNum = TRIAL_DEFUALT_MULT * inode->degree > TRIAL_DEFAULT ? TRIAL_DEFUALT_MULT * inode->degree : TRIAL_DEFAULT;
#ifndef PAPER
		jumps = 0;
		inode->walkingNode.inode = saveNode;
#else
		jumps = trialNum;
		inode->walkingNode.frequency = jumps;
#endif
#ifndef IGNORE_SELF_WALKS
		//inode->walkingNode.frequency = jumps;
#ifdef PAPER
		jumpsList.push_back(inode->insertWalkingNodeToWalkedNode());
#else
		saveNode->jumps1.push_back(inode->insertWalkingNodeToWalkedNode());
#endif
#endif
		for (int j = 0; j < trialNum; j ++) {
			inode = saveNode;
			while (randomNumberGenerator::nextReal() < pageRankAlpha) {
#ifndef WE
				inode = inode->connectedNodes[randomNumberGenerator::nextInt() % (inode->degree)];
#else
				inode = inode->connectedNodes[getPosition(inode)];
#endif
#ifndef PAPER
				if (inode->walkingNode.inode != saveNode) {
					inode->walkingNode.inode = saveNode;
					saveNode->jumps1.push_back(inode->insertWalkingNodeToWalkedNode());
				}
#else
				if (inode->walkingNode.frequency == 0) {
					jumpsList.push_back(inode->insertWalkingNodeToWalkedNode());
				}
#endif
				
				inode->walkingNode.frequency++;
				jumps++;
			}
		}
#ifdef IGNORE_SELF_WALKS
		jumps -= (int)saveNode->walkingNode.frequency;
#endif
#ifdef PAPER
		for (list<node::linkList*>::iterator itr = jumpsList.begin(); itr != jumpsList.end(); itr ++) {
#else
		for (list<node::linkList*>::iterator itr = saveNode->jumps1.begin(); itr != saveNode->jumps1.end(); itr++) {
#endif
			(*itr)->inodeFrequency.frequency = (*itr)->inode1->walkingNode.frequency / jumps;
			(*itr)->inode1->walkingNode.frequency = 0;
#ifndef PAPER
			(*itr)->inode1->walkingNode.inode = NULL;
#else
			(*itr)->inode1 = saveNode;
#endif
		}
		saveNode->walkingNode.frequency = 0;
#ifdef PAPER
		jumpsList.clear();
#endif
	}
	//cout << mainGraph->size << endl;
	if (!testing) {
	for (i = mainGraph->size - 2; i >= 0; i --) {
		node::linkList *link = lastSeed->walkedNodes1->next;
#ifdef PAPER
		node::linkList *next;
#endif
		while (link != NULL) {
#ifdef PAPER
			if (!link->inode1->seed) {
				link->inode1->score += link->inodeFrequency.frequency;
				if (link->inode1->fibNode == NULL) {
					link->inode1->fibNode = fh_insertkey(fh, -link->inode1->score, (void*) link->inode1);
				} else {
					fh_replacekey(fh, link->inode1->fibNode, -link->inode1->score);
				}
			}
#else
			link->inodeFrequency.inode->score += link->inodeFrequency.frequency;
			if (link->inodeFrequency.inode->fibNode == NULL) {
				link->inodeFrequency.inode->fibNode = fh_insertkey(fh, -link->inodeFrequency.inode->score, (void*) link->inodeFrequency.inode);
			} else {
				fh_replacekey(fh, link->inodeFrequency.inode->fibNode, -link->inodeFrequency.inode->score);
			}
#endif
#ifdef PAPER
			next = link->next;
			delete(link);
			link = next;
		}
		lastSeed->walkedNodes1->next = NULL;
#else
			link = link->next;
		}
#endif
		if (fh->fh_n > 0) {
			lastSeed = (node*) fh_extractmin(fh);
			lastSeed->fibNode = NULL;
		} else {
			while (generalIndex != mainGraph->size) {
				lastSeed = mainGraph->nodes[generalIndex ++];
				if (!lastSeed->seed) {
					break;
				}
			}
		}
		lastSeed->seed = true;
#ifndef PAPER
		for (list<node::linkList*>::iterator itr = lastSeed->jumps1.begin(); itr != lastSeed->jumps1.end(); itr++) {
			(*itr)->inode1->eraseFromWalkedNode(*itr);
		}
		lastSeed->jumps1.clear();
#endif
		order1[orderIndex ++] = lastSeed;
	}
#ifdef PAPER
	node::linkList *link = lastSeed->walkedNodes1->next;
	node::linkList *next;
	while (link != NULL) {
		next = link->next;
		delete(link);
		link = next;
	}
	lastSeed->walkedNodes1->next = NULL;
	}
#endif
	modularityInit(start);
	for (int j = 1; j < orderIndex; j ++) {
		updateModularity(order1[j]);
	}
	bool check = false;
	if (maxNode == NULL) {
		for (i = 0; i < orderIndex; i ++) {
			p1.push_back(order1[i]);
		}
	} else {
	for (i = 0; i < orderIndex; i ++) {
		if (check == false) {
#ifdef oneex
			if (order1[i]->degree == 1 && order1[i]->connectedNodes[0]->virtualNumber > maxNode->virtualNumber) {
				p2.push_back(order1[i]);
			} else {
				p1.push_back(order1[i]);
			}
#else
			p1.push_back(order1[i]);
#endif
			if (order1[i] == maxNode) {
				check = true;
			}
		} else {
			order1[i]->seed = false;
#ifdef oneex
			if (order1[i]->degree == 1 && order1[i]->connectedNodes[0]->virtualNumber <= maxNode->virtualNumber) {
				p1.push_back(order1[i]);
			} else {
				p2.push_back(order1[i]);
			}
#else
			p2.push_back(order1[i]);
#endif
		}
	}
	}
	if (!p1.empty() && !p2.empty()) {
		double temp = modularity;
		modularity = maxModul;
		if (testing) {
		maxModul = temp;
}
	} else {
		modularity = -1;
	}
	isClustered = true;

	delete[](order1);
	fh_deleteheap(fh);
}
#ifdef WE
int network::getPosition(node* inode){
	double max = inode->majmueTajamoei[inode->degree-1];
#ifdef intWeight
	if (inode->degree == max)
		return randomNumberGenerator::nextInt() % (unsigned int)max;
	double randomPosition = (randomNumberGenerator::nextInt() % (unsigned int)max);
#else
	double randomPosition = (randomNumberGenerator::nextReal() * max);
#endif
	int i = 0;
	int j = inode->degree-1;
	while (true) {
		if (i == j) {
			return i;
		} else if (i == j + 1) {
			if (inode->majmueTajamoei[i] <= randomPosition) {
				return j;
			}
			return i;
		}
		
		int k = (i + j) / 2;
		if (inode->majmueTajamoei[k] <= randomPosition) {
			i = k + 1;
		} else {
			j = k;
		}
	}
}
#endif

void network::modularityInit(node* inode) {
	inode->group = 1;

	maxModul = 0;
	maxNode = NULL;
#ifndef WE
	inDegree = inode->baseDegree;
	inDegree2 = 0;
	for (int i = mainGraph->size - 1; i >= 0; i--) {
		inDegree2 += mainGraph->nodes[i]->baseDegree;
	}

	inDegree2 -= inDegree;

	modularity = -(((double) inode->degree) / graph::baseEdgeSize - ((double) (inDegree * inDegree2)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize));
#else
	inDegree = inode->baseAllWeights;
	inDegree2 = 0;
	for (int i = mainGraph->size - 1; i >= 0; i--) {
		inDegree2 += mainGraph->nodes[i]->baseAllWeights;
	}

	inDegree2 -= inDegree;
	int i, j = 0;
	for (i = 0; i < inode->degree; i ++) {
		j += inode->weight[i];
	}
	modularity = -(((double) j) / graph::baseWeights - ((double) (inDegree * inDegree2)) / (2.0 * graph::baseWeights * graph::baseWeights));
#endif
}

inline double network::updateModularity(node* inode) {
	inode->group = 1;
#ifndef WE
	unsigned int degree = inode->baseDegree;
#else
	double degree = inode->baseAllWeights;
#endif
	double inIncident = 0;
	double inIncident2 = 0;
	for (int i = inode->degree - 1; i >= 0; i --) {
		if (inode->connectedNodes[i]->group == 1) {
#ifndef WE
			inIncident++;
		} else {
			inIncident2++;
#else
			inIncident += inode->weight[i];
		} else {
			inIncident2 += inode->weight[i];
#endif
		}
	}
	inDegree2 -= degree;
#ifndef WE
	double daghoo = (((double) inIncident) / graph::baseEdgeSize - ((double) (degree * inDegree)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize)) - (((double) inIncident2) / graph::baseEdgeSize - ((double) (degree * inDegree2)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize));
	modularity += (((double) inIncident) / graph::baseEdgeSize - ((double) (degree * inDegree)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize));
	modularity -= (((double) inIncident2) / graph::baseEdgeSize - ((double) (degree * inDegree2)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize));
#else
	double daghoo = (((double) inIncident) / graph::baseWeights - ((double) (degree * inDegree)) / (2.0 * graph::baseWeights * graph::baseWeights)) - (((double) inIncident2) / graph::baseWeights - ((double) (degree * inDegree2)) / (2.0 * graph::baseWeights * graph::baseWeights));
	modularity += (((double) inIncident) / graph::baseWeights - ((double) (degree * inDegree)) / (2.0 * graph::baseWeights * graph::baseWeights));
	modularity -= (((double) inIncident2) / graph::baseWeights - ((double) (degree * inDegree2)) / (2.0 * graph::baseWeights * graph::baseWeights));
#endif
	if (modularity > maxModul) {
		maxModul = modularity;
		maxNode = inode;
	}
	inDegree += degree;
	return daghoo;
}

void network::updateComponents(int componentValue) {
	for (std::vector<node*>::iterator itr = p1.begin(); itr != p1.end(); itr++) {
		(*itr)->component = componentValue;
	}
}

graph* network::createSubGraph(std::vector<node*>& p) {
	graph *nGraph = new graph;
	nGraph->size = (unsigned int) p.size();
	nGraph->edgeSize = 0;
	nGraph->nodes = new node*[nGraph->size];
	unsigned int i = 0, j;
	for (std::vector<node*>::iterator itr = p.begin(); itr != p.end(); itr ++) {
		nGraph->nodes[i] = (*itr);
		for (j = 0; j < nGraph->nodes[i]->degree; ) {
			if (nGraph->nodes[i]->component != nGraph->nodes[i]->connectedNodes[j]->component) {
				nGraph->nodes[i]->connectedNodes[j] = nGraph->nodes[i]->connectedNodes[-- nGraph->nodes[i]->degree];
#ifdef WE
				nGraph->nodes[i]->weight[j] = nGraph->nodes[i]->weight[nGraph->nodes[i]->degree];
#endif
			} else {
				j ++;
			}
		}
		nGraph->edgeSize += nGraph->nodes[i]->degree;
		nGraph->nodes[i]->virtualNumber = i ++;
	}
	nGraph->edgeSize /= 2;
	nGraph->resetNodes();
	return nGraph;
}

graph* network::getFirstSubGraph() {
	return createSubGraph(p1);
}

graph* network::getSecondSubGraph() {
	return createSubGraph(p2);
}

network::network(graph* mainGraphRef)
{
#ifdef hier
	lvl = 0;
#endif
	mainGraph = mainGraphRef;
	if (mainGraph->size == 1) {
		isClustered = true;
		modularityInit(mainGraph->nodes[0]);
	} else {
		isClustered = false;
		modularity = -1;
	}
}

network::~network(void)
{
}
