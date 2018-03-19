#pragma once
#include "node.h"
#include <string>
#include <list>
using namespace std;

class graph
{
public:
	double maxModul;
	node *maxNode;

	double deltaInternal();
	double deltaInterCluster();

	static node *baseNodes;
	static unsigned int baseSize;
	static unsigned int baseEdgeSize;
#ifdef WE
	static double baseWeights;
#endif

	static unsigned int indexSepratedNodes;
	static node** sepratedNodes;

	node **nodes;
	unsigned int size;
	unsigned int edgeSize;

	static graph* initialGraph();
	void prune(list<graph*>&, unsigned int, bool swap);
	static graph* createOneNodeGraph(node*);

	void resetNodes();
	void updateAllComponents(unsigned int);

	static void readFromOurFile(string);
	static void readFromFile1(string, bool);
	static void createGephiImportFile(string);
#ifndef CLS
	static void createPajekImportFile(string fileName);
	static void createBlondleImportFile(string);
	static void injectBlondleClusters(string, int);
	static void injectClusters(string, int);
	static double calculateModularity(int, int);
	void createPajekClusters(string in, string name, int num);
#endif
	static void reload(string);
#ifdef WE
	void newMajmueTajamoei();
	void deleteMajmueTajamoei();
#endif
#ifdef one
	void syncOneDegreeNodes();
#endif
	graph(void);
	~graph(void);
private:
	inline graph(unsigned int);
};
