#pragma once
#include "graph.h"
#include "node.h"
#include <vector>
using namespace std;
#ifndef TRIAL_DEFAULT
#define TRIAL_DEFAULT 50
#endif
#ifndef TRIAL_DEFUALT_MULT
#define TRIAL_DEFUALT_MULT 10
#endif
#define NUM_REMOVE_FROM_HEAP 100
#ifndef pageRankAlpha
#define pageRankAlpha 0.7
#endif
#define IGNORE_SELF_WALKS 1

class network
{
public:
	static int trialNum;

	graph *mainGraph;
	bool isClustered;
	double modularity;
	vector<node*> p1, p2;
	int jumps;
	double *score, *estimatePageRank;
	list<node*> alterNodes1;

#ifdef hier
	unsigned int lvl;
#endif
#ifndef WE	
	int inDegree, inDegree2;
#else
	double inDegree, inDegree2;
#endif
	double maxModul;
	node *maxNode;
	void modularityInit(node*);
	inline double updateModularity(node*);
	double aaa(node*);
#ifndef CLS
	double relativeMutualInfo();
	double nodeOverlaps();
	void connectedness(double&);
	double fitness(double);
	node* randomWalk(node*);
	void prw(node*);
	void partition1();
	void partition3();
#endif
#ifdef WE
	int getPosition(node*);
#endif
	node* candidateNode();
	void updateComponents(int);
	void partition2(bool);

	graph* createSubGraph(vector<node*>&);
	graph* getFirstSubGraph();
	graph* getSecondSubGraph();
	network(graph*);
	~network(void);
};
