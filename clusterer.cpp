#include "clusterer.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "fib/fibpriv.h"
#include <sys/stat.h>
#include <stdlib.h>
using namespace std;

void clusterer::run() {
	double maxModularity = 0;
	time_t startTime = time(NULL);
	int i = 0;
	graph::sepratedNodes = new node*[graph::baseSize];
	list<graph*> gs;
	graph *initGraph = graph::initialGraph();
#ifdef compLimit
	initGraph->prune(gs, compLimit - 1, true);
	compNum = gs.size();
#else
	initGraph->prune(gs, 0, true);
#endif
	i = 0;
	for (list<graph*>::iterator itr = gs.begin(); itr != gs.end(); itr ++) {
		(*itr)->updateAllComponents(i ++);
		results.push_back(new network((*itr)));
	}
	gs.clear();
#ifdef hier
	stringstream ss;
	for (i = 0; i < graph::baseSize; i ++) {
		ss << graph::baseNodes[i].component;
		graph::baseNodes[i].hi = ss.str();
		ss.str("");
	}
#endif
#ifdef compLimit
	if (compNum < compLimit) {
#endif
	while (true) {
		vector<network*>::iterator maxModularNetwork;
		double maxMod = -1000;
		for (vector<network*>::iterator itr = results.begin(); itr != results.end(); itr++) {
			if ((*itr)->mainGraph->size==1) {
				(*itr)->modularity=-1;
				(*itr)->isClustered=true;
				continue;
			}

			if (!(*itr)->isClustered) {
#ifdef WE
				(*itr)->mainGraph->newMajmueTajamoei();
#endif
				(*itr)->partition2(false);
#ifdef WE
				(*itr)->mainGraph->deleteMajmueTajamoei();
#endif
			}

			if ((*itr)->modularity > maxMod) {
				maxMod = (*itr)->modularity;
				maxModularNetwork = itr;
			}
		}
#ifndef exactClustersCount
		if (maxMod > 0) {
#endif
			maxModularity += maxMod;
			(*maxModularNetwork)->updateComponents((int)results.size());
			graph *graph1 = (*maxModularNetwork)->getFirstSubGraph();
			graph *graph2 = (*maxModularNetwork)->getSecondSubGraph();
#ifdef hier
			i = (*maxModularNetwork)->lvl;
#endif
			delete((*maxModularNetwork)->mainGraph);
			delete(*maxModularNetwork);
#ifdef compLimit
			graph1->prune(gs, compLimit - compNum - 1, false);
			graph2->prune(gs, compLimit - compNum - gs.size(), true);
			compNum += gs.size() - 1;
#else
			graph1->prune(gs, 0, false);
			graph2->prune(gs, 0, true);
#endif
			graph2 = gs.back();
			gs.pop_back();
			network *ng2 = new network(graph2);
#ifdef hier
			ng2->lvl = i + 1;
#endif
			(*maxModularNetwork) = ng2;
			for (list<graph*>::iterator itr = gs.begin(); itr != gs.end(); itr ++) {
				(*itr)->updateAllComponents((unsigned int) results.size());
				network* net = new network(*itr);
				results.push_back(net);
#ifdef hier
				net->lvl=i + 1;
				for (int t = 0; t < (*itr)->size; t ++) {
					ss << ":" << (*itr)->nodes[t]->component;
					(*itr)->nodes[t]->hi += ss.str();
					ss.str("");
				}
#endif
			}
			gs.clear();
#ifdef compLimit
			if(compLimit == compNum) {
				break;
			}
#endif
		}
#ifndef exactClustersCount
		else {
			break;
		}
	}
#endif
#ifdef compLimit
	} else {
		compNum ++;
	}
#endif
	for (i = graph::indexSepratedNodes - 1; i >= 0; i --) {
		graph::sepratedNodes[i]->component = graph::sepratedNodes[i]->connectedNodes[0]->component;
#ifdef hier
		graph::sepratedNodes[i]->hi = graph::sepratedNodes[i]->connectedNodes[0]->hi;
#endif
	}
	compNum = results.size();
	for (vector<network*>::iterator itr = results.begin(); itr != results.end(); itr ++) {
		delete((*itr)->mainGraph);
		delete(*itr);
	}
	results.clear();
#ifdef one
        for (i = 0; i < graph::baseSize; i ++) {
                if (graph::baseNodes[i].baseDegree == 1) {
                        graph::baseNodes[i].component = graph::baseNodes[i].connectedNodes[0]->component;
#ifdef hier
						graph::baseNodes[i].component = graph::baseNodes[i].connectedNodes[0]->component;
#endif
                }
        }
#endif
        cout << compNum << " components in " << time(NULL) - startTime << "seconds" << endl;
        graph::reload("Datasets/" + dataset + "/" + dataset + ".smg");
#ifdef creategraphml
	graph::createGephiImportFile("Datasets/" + dataset + "/" + dataset + ".graphml");
#endif
//#ifdef HA
	std::stringstream buffer, buffer1;
	for (i = 0; i < graph::baseSize; i ++) {
		//buffer1 << graph::baseNodes[i].virtualNumber << " " << graph::baseNodes[i].ha << "\n";
#ifdef HAE
		buffer1 << graph::baseNodes[i].virtualNumber << " " << graph::baseNodes[i].component << "\n";
#endif
#ifdef HA
		buffer << graph::baseNodes[i].realNumber << " " << graph::baseNodes[i].component << "\n";
#endif
		//buffer << graph::baseNodes[i].realNumber << " " << graph::baseNodes[i].ha << "\n";
#ifdef hier
		ss << graph::baseNodes[i].realNumber << "\t" << graph::baseNodes[i].hi << "\n";
#endif
	}
#ifdef hier
	ofstream hfout(("Datasets/" + dataset + "/hier_" + dataset + ".tree").c_str());
	hfout << ss.rdbuf();
	hfout.close();
	ss.str("");
#endif
#ifdef HA
	ofstream fout(("Datasets/" + dataset + "/" + dataset + ".tree").c_str());
	fout << buffer.rdbuf();
	fout.close();
	buffer.clear();
#endif
#ifdef HAE
	ofstream fout1(("Datasets/" + dataset + "/" + dataset + ".tav").c_str());
	fout1 << buffer1.rdbuf();
	fout1.close();
	buffer1.clear();
#endif
//#endif
#ifdef DD
	ghotr(compNum);
#else
	//calc();
#endif
}
#include <stdlib.h>
clusterer::clusterer(string dataset)
{
	this->dataset = dataset;
	struct stat buffer;
	if (stat(("Datasets/" + dataset + "/" + dataset + ".smg").c_str(), &buffer) != 0) {
		graph::readFromFile1("Datasets/" + dataset + "/" + dataset + ".txt", false);
	} else {
		graph::readFromOurFile("Datasets/" + dataset + "/" + dataset + ".smg");
	}
#ifndef CLS
#ifdef blondelimportfile
	graph::createBlondleImportFile("Datasets/" + dataset + "/blondel" + dataset + ".txt");
#endif
#ifdef pajekimportfile
	graph::createPajekImportFile("Datasets/" + dataset + "/infohiermap" + dataset + ".net");
#endif
#ifdef injectblondel
	graph::injectBlondleClusters("Datasets/" + dataset + "/blondel" + dataset + ".tree", 0);
#ifdef DD
	unsigned int i, compNum = 0;
	for (i = 0; i < graph::baseSize; i ++) {
		graph::baseNodes[i].degree = graph::baseNodes[i].baseDegree;
		if (compNum < graph::baseNodes[i].component) {
			compNum = graph::baseNodes[i].component;
		}
	}
	ghotr(compNum + 1);
#endif
#endif
#ifdef injectinfohiermap
	graph::injectClusters("Datasets/" + dataset + "/infohiermap" + dataset + ".tree", 1);
#ifdef DD
	unsigned int i, compNum = 0;
	for (i = 0; i < graph::baseSize; i ++) {
		graph::baseNodes[i].degree = graph::baseNodes[i].baseDegree;
		if (compNum < graph::baseNodes[i].component) {
			compNum = graph::baseNodes[i].component;
		}
	}
	ghotr(compNum + 1);
#endif
#endif
#ifdef injecttaval
	graph::injectBlondleClusters("Datasets/" + dataset + "/" + dataset + ".tav", 2);
	cout << "taval injected" << endl;
#endif
#ifdef blondelmodularity
	cout << "blondel= " << graph::calculateModularity(1, 0) << endl;
#endif
#ifdef infohiermapmodularity
	cout << "infohiermap= " << graph::calculateModularity(1, 1) << endl;
#endif
#ifdef tavalmodularity
	cout << "taval= " << graph::calculateModularity(1, 2) << endl;
#endif
#ifdef nrun
	exit(1);
#endif
#endif
}

clusterer::~clusterer(void)
{
}
#ifndef CLS
void clusterer::ghotr(unsigned int compNum) {
	cout << "\n";
	unsigned int *index = new unsigned int[compNum];
	node ***arrayOfNodes = new node**[compNum];
	unsigned int i;
	for (i = 0; i < compNum; i ++) {
		index[i] = 0;
	}
	for (i = 0; i < graph::baseSize; i ++) {
		index[graph::baseNodes[i].component] ++;
	}
	for (i = 0; i < compNum; i ++) {
		arrayOfNodes[i] = new node*[index[i]];
		index[i] = 0;
	}
	for (i = 0; i < graph::baseSize; i ++) {
		arrayOfNodes[graph::baseNodes[i].component][index[graph::baseNodes[i].component] ++] = graph::baseNodes + i;
	}
	unsigned int j;
	node *v1;
	for (i = 0; i < graph::baseSize; i ++) {
		v1 = graph::baseNodes + i;
		for (j = 0; j < v1->degree; ) {
			if (v1->component != v1->connectedNodes[j]->component) {
				v1->connectedNodes[j] = v1->connectedNodes[-- v1->degree];
#ifdef WE
				v1->weight[j] = v1->weight[v1->degree];
#endif
			} else {
				j ++;
			}
		}
	}
	struct fibheap *fh = fh_makekeyheap();
	unsigned int k, l;
	double dist, maxDist, maxMaxDist = 0;
	for (k = 0; k < compNum; k ++) {
		maxDist = 0;
		for (i = 0; i < index[k]; i ++) {
			dist = 0;
			for (j = 0; j < index[k]; j ++) {
				if (i != j) {
					arrayOfNodes[k][j]->seed = false;
					arrayOfNodes[k][j]->score = 0x7FFFFFFF;
					arrayOfNodes[k][j]->fibNode = 0;
				} else {
					arrayOfNodes[k][j]->score = 0;
				}
			}
			v1 = arrayOfNodes[k][i];
			l = index[k] - 1;
			while (l > 0) {
				v1->seed = true;
				for (j = 0; j < v1->degree; j ++) {
					if (v1->connectedNodes[j]->seed) {
						continue;
					}
#ifdef WE
					if (v1->connectedNodes[j]->score > v1->score + v1->weight[j]) {
						v1->connectedNodes[j]->score = v1->score + v1->weight[j];
#else
					if (v1->connectedNodes[j]->score > v1->score + 1) {
						v1->connectedNodes[j]->score = v1->score + 1;
#endif
						if (v1->connectedNodes[j]->fibNode == 0) {
							v1->connectedNodes[j]->fibNode = fh_insertkey(fh, v1->connectedNodes[j]->score, (void*) v1->connectedNodes[j]);
						} else {
							fh_replacekey(fh, v1->connectedNodes[j]->fibNode, v1->connectedNodes[j]->score);
						}
					}
				}
				if (fh->fh_n == 0) {
					break;
				} else if (l == 1) {
					v1 = (node*) fh_extractmin(fh);
					dist = v1->score;
				} else {
					v1 = (node*) fh_extractmin(fh);
					l --;
					continue;
				}
				break;
			}
			if (maxDist < dist) {
				maxDist = dist;
			}
		}
		cout << "component " << k << ": " << maxDist << "\n";
		if (maxMaxDist < maxDist) {
			maxMaxDist = maxDist;
		}
	}
	cout << "\nmax: " << maxMaxDist << "\n"  << endl;
	fh_deleteheap(fh);
	for (i = 0; i < compNum; i ++) {
		delete[](arrayOfNodes[i]);
	}
	delete[](arrayOfNodes);
	delete[](index);
}
#endif

void clusterer::calc() {
	unsigned int i;
	unsigned int *count = new unsigned int[compNum];
	for (i = 0; i < compNum; i ++) {
		count[i] = 0;
	}
	for (i = 0; i < graph::baseSize; i ++) {
		graph::baseNodes[i].group = 2;
		count[graph::baseNodes[i].component] ++;
	}
	for (i = 1; i < compNum; i ++) {
		count[i] += count[i - 1];
	}
	nodes = new node*[graph::baseSize];
	for (i = 0; i < graph::baseSize; i ++) {
		nodes[-- count[graph::baseNodes[i].component]] = graph::baseNodes + i;
	}
	for (i = 0; i < graph::baseSize; i ++) {
		cout << nodes[i]->realNumber << endl;
	}
	cout <<	"modularity: " << calculateModularity(0, graph::baseSize) << endl;
	delete[](count);
	delete[](nodes);
}

double clusterer::calculateModularity(unsigned int row, unsigned int limitRow) {
	if (nodes[row]->component == nodes[limitRow - 1]->component) {
		return 0;
	}
	nodes[row]->group = 1;
	unsigned int inDegree = nodes[row]->baseDegree, inDegree2 = 0, degree, inIncident, inIncident2, j, i, k;
	for (i = row + 1; i < limitRow; i ++) {
		inDegree2 += nodes[i]->baseDegree;
	}
	double modularity = -(((double) nodes[row]->degree) / graph::baseEdgeSize - ((double) (inDegree * inDegree2)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize));
	for (i = row + 1; nodes[row]->component == nodes[i]->component; i ++) {
		nodes[i]->group = 1;
		degree = nodes[i]->baseDegree;
		inIncident = 0;
		inIncident2 = 0;
		for (j = 0; j < nodes[i]->degree; j ++) {
			if (nodes[i]->connectedNodes[j]->group == 1) {
				inIncident ++;
			} else {
				inIncident2 ++;
			}
		}
		inDegree2 -= degree;
		modularity += (((double) inIncident) / graph::baseEdgeSize - ((double) (degree * inDegree)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize));
		modularity -= (((double) inIncident2) / graph::baseEdgeSize - ((double) (degree * inDegree2)) / (2.0 * graph::baseEdgeSize * graph::baseEdgeSize));
		inDegree += degree;
	}
	k = i;
	for (i = row; i < k; i ++) {
		nodes[i]->group = 2;
		for (j = 0; j < nodes[i]->degree; ) {
			if (nodes[row]->component != nodes[i]->connectedNodes[j]->component) {
				nodes[i]->connectedNodes[j] = nodes[i]->connectedNodes[-- nodes[i]->degree];
			} else {
				j ++;
			}
		}
	}
	for ( ; i < limitRow; i ++) {
		for (j = 0; j < nodes[i]->degree; ) {
			if (nodes[row]->component == nodes[i]->connectedNodes[j]->component) {
				nodes[i]->connectedNodes[j] = nodes[i]->connectedNodes[-- nodes[i]->degree];
			} else {
				j ++;
			}
		}
	}
	return modularity + calculateModularity(k, limitRow);
}
