#include "graph.h"
#include <fstream>
#include <sstream>
#include "randomNumberGenerator.h"
#include <list>
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;
/*#ifdef one
void graph::syncOneDegreeNodes() {
	for (unsigned int i = 0; i < size; i ++) {
		if (nodes[i]->degree == 1) {
			cout << "done" << endl;
			nodes[i]->component = nodes[i]->connectedNodes[0]->component;
		}
	}
}
#endif*/

#ifndef CLS
void graph::createBlondleImportFile(string filename) {
	stringstream buffer;
	unsigned int i, j;
	for (i = 0; i < baseSize; i ++) {
		for (j = 0; j < baseNodes[i].baseDegree; j ++) {
			if (baseNodes[i].virtualNumber < baseNodes[i].connectedNodes[j]->virtualNumber) {
#ifdef WE
				buffer << baseNodes[i].virtualNumber << "\t" << baseNodes[i].connectedNodes[j]->virtualNumber << "\t" << baseNodes[i].weight[j] << endl;
#else
				buffer << baseNodes[i].virtualNumber << "\t" << baseNodes[i].connectedNodes[j]->virtualNumber << endl;
#endif
			}
		}
	}
	ofstream fout(filename.c_str());
	fout << buffer.rdbuf() << endl;
	fout.close();
	buffer.clear();
}
#endif

node** graph::sepratedNodes;
unsigned int graph::indexSepratedNodes = 0;

node* graph::baseNodes;
unsigned int graph::baseSize;
unsigned int graph::baseEdgeSize;
#ifdef WE
double graph::baseWeights;
#endif

void graph::resetNodes() {
	for (unsigned int i = 0; i < size; i ++) {
		nodes[i]->reset();
	}
}

graph* graph::initialGraph() {
	graph *nGraph = new graph;
	nGraph->nodes = new node*[baseSize];
	nGraph->size = baseSize;
	nGraph->edgeSize = baseEdgeSize;
	for (unsigned int i = 0; i < baseSize; i ++) {
		baseNodes[i].degree = baseNodes[i].baseDegree;
		nGraph->nodes[i] = baseNodes + i;
	}
	return nGraph;
}
#ifdef WE
void graph::newMajmueTajamoei() {
	for (unsigned int i = 0; i < size; i ++) {
		nodes[i]->majmueTajamoei = new double[nodes[i]->degree];
		nodes[i]->majmueTajamoei[0] = nodes[i]->weight[0];
		for (unsigned int j = 1; j < nodes[i]->degree; j ++) {
			nodes[i]->majmueTajamoei[j] = nodes[i]->weight[j] + nodes[i]->majmueTajamoei[j - 1];
		}
	}
}

void graph::deleteMajmueTajamoei() {
	for (unsigned int i = 0; i < size; i ++) {
		delete[](nodes[i]->majmueTajamoei);
	}
}
#endif
void graph::prone(list<graph*>& retVal, unsigned int limit) {
	unsigned int i, j;
#ifdef newpn
	for (i = 0; i < size; ) {
		if (nodes[i]->degree == 1 && nodes[i]->connectedNodes[0]->degree == 2) {
			nodes[i]->component = -1;
			j = nodes[i]->connectedNodes[0]->virtualNumber;
			if (nodes[i] == nodes[j]->connectedNodes[0]) {
				nodes[j]->connectedNodes[0] = nodes[j]->connectedNodes[1];
#ifdef WE
				nodes[j]->weight[0] = nodes[j]->weight[1];
#endif
			}
			nodes[j]->degree = 1;
			sepratedNodes[indexSepratedNodes ++] = nodes[i];
			size --;
			if (j > i) {
				nodes[i] = nodes[j];
				nodes[i]->virtualNumber = i;
				nodes[j] = nodes[size];
				nodes[j]->virtualNumber = j;
			} else {
				nodes[i] = nodes[size];
				nodes[i]->virtualNumber = i;
				i --;
				node *tmp = nodes[j];
				nodes[j] = nodes[i];
				nodes[i] = tmp;
				nodes[i]->virtualNumber = i;
				nodes[j]->virtualNumber = j;
			}
		} else {
			i ++;
		}
	}
#endif
#ifdef pn
	for (i = 0; i < size; ) {
		if (nodes[i]->degree > 1) {
#ifndef rec
			nodes[i]->group = 3;
#endif
			for (j = 0; j < nodes[i]->degree; ) {
				if (nodes[i]->connectedNodes[j]->degree == 1
#ifndef rec
				&& nodes[i]->connectedNodes[j]->group != 3
#endif
				) {
					nodes[i]->connectedNodes[j]->component = -1;
					sepratedNodes[indexSepratedNodes ++] = nodes[i]->connectedNodes[j];
					nodes[-- size]->virtualNumber = nodes[i]->connectedNodes[j]->virtualNumber;
					nodes[nodes[size]->virtualNumber] = nodes[size];
					nodes[i]->connectedNodes[j] = nodes[i]->connectedNodes[-- nodes[i]->degree];
#ifdef WE
					nodes[i]->weight[j] = nodes[i]->weight[nodes[i]->degree];
#endif
				} else {
					j ++;
				}
			}
			if (nodes[i]->degree == 1) {
				node *tmp;
				if (nodes[i]->connectedNodes[0]->virtualNumber < i) {
					tmp = nodes[i --]->connectedNodes[0];
				} else {
					tmp = nodes[i]->connectedNodes[0];
				}
				nodes[i]->virtualNumber = tmp->virtualNumber;
				nodes[tmp->virtualNumber] = nodes[i];
				nodes[i] = tmp;
				nodes[i]->virtualNumber = i;
			}
			else if (nodes[i]->degree > 1) {
				i ++;
			}
		} else if (nodes[i]->degree == 1 && nodes[i]->connectedNodes[0]->degree != 1) {
			node *tmp = nodes[i]->connectedNodes[0];
			nodes[i]->virtualNumber = tmp->virtualNumber;
			nodes[tmp->virtualNumber] = nodes[i];
			nodes[i] = tmp;
			nodes[i]->virtualNumber = i;
		} else if (nodes[i]->degree == 0) {
			retVal.push_back(createOneNodeGraph(nodes[i]));
			nodes[i] = nodes[-- size];
			nodes[i]->virtualNumber = i;
		} else {
			i ++;
		}
	}
	if (size == 0) {
		delete(this);
		return;
	}
#endif
#ifdef dfs
#ifdef compLimit
	if (limit > 0) {
	unsigned int upToNow = 0;
#endif
	list<node*> newList;
	for (i = 0; i < size; i ++) {
		if (nodes[i]->group == 4) {
			continue;
		}
#ifdef compLimit
		if (limit == 0) {
			upToNow = size - upToNow;
			graph *nlgraph = new graph(upToNow);
			j = 0;
			for ( ; i < size; i ++) {
				if (nodes[i]->group == 4) {
					continue;
				}
				nodes[i]->group = 4;
				nlgraph->nodes[j] = nodes[i];
				nlgraph->nodes[j]->virtualNumber = j ++;
			}
			retVal.push_back(nlgraph);
			break;
		}
		limit --;
#endif
		nodes[i]->group = 4;
		newList.push_back(nodes[i]);
		unsigned int len = 1;
		for (list<node*>::iterator itr = newList.begin(); len > 0; itr ++) {
			len --;
			for (j = 0; j < (*itr)->degree; j ++) {
				if ((*itr)->connectedNodes[j]->group == 4) {
					continue;
				}
				(*itr)->connectedNodes[j]->group = 4;
				newList.push_back((*itr)->connectedNodes[j]);
				len ++;
			}
		}
		graph *nGraph = new graph(newList.size());
		j = 0;
		for (list<node*>::iterator itr = newList.begin(); itr != newList.end(); itr ++, j ++) {
			nGraph->nodes[j] = (*itr);
			nGraph->nodes[j]->virtualNumber = j;
		}
#ifdef compLimit
		upToNow += j;
#endif
		newList.clear();
		retVal.push_back(nGraph);
	}
	delete(this);
#ifdef compLimit
	} else {
		retVal.push_back(this);
	}
#endif
#else
	retVal.push_back(this);
#endif
}

graph* graph::createOneNodeGraph(node* inode) {
	graph* nGraph = new graph();
	nGraph->size = 1;
	nGraph->nodes = new node*[1];
	nGraph->nodes[0] = inode;
	nGraph->edgeSize = 0;
	return nGraph;
}

void graph::updateAllComponents(unsigned int nCompVal) {
	for (unsigned int i = 0; i < size; i ++) {
		nodes[i]->component = nCompVal;
	}
}

void graph::readFromOurFile(string fileName) {
	stringstream buffer;
	ifstream fin(fileName.c_str());
	buffer << fin.rdbuf();
	fin.close();
	buffer >> baseSize >> baseEdgeSize
#ifdef WE
		>> baseWeights
#endif
		;
	baseNodes = new node[baseSize];

	unsigned int i = 0, j, k;
	for (; buffer >> baseNodes[i].realNumber >> baseNodes[i].baseDegree; i ++) {
		baseNodes[i].virtualNumber = i;
#ifdef newM
		baseNodes[i].degree = baseNodes[i].baseDegree;
#endif
		baseNodes[i].component = 0;
		baseNodes[i].connectedNodes = new node*[baseNodes[i].baseDegree];
#ifdef WE
		//baseNodes[i].weight = new unsigned int[baseNodes[i].baseDegree];
		baseNodes[i].weight = new double[baseNodes[i].baseDegree];
		baseNodes[i].baseAllWeights = 0;
#endif
		for (j = 0; j < baseNodes[i].baseDegree; j ++) {
			buffer >> k;
			baseNodes[i].connectedNodes[j] = baseNodes + k;
#ifdef WE
			buffer >> baseNodes[i].weight[j];
			baseNodes[i].baseAllWeights += baseNodes[i].weight[j];
#endif
		}
	}
	buffer.clear();

//	cout << "number of nodes: " << baseSize << endl;
//	cout << "number of edges: " << baseEdgeSize << endl;
#ifdef WE
	cout << "weights: " << baseWeights << endl;
#endif
}
void graph::createGephiImportFile(string fileName) {
	stringstream buffer;
	buffer
		<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\">\n"
		<< "<key attr.name=\"label\" attr.type=\"string\" for=\"node\" id=\"label\"/>\n"
		<< "<key attr.name=\"comp\" attr.type=\"string\" for=\"node\" id=\"comp\"/>\n"
#ifdef injectblondel
		<< "<key attr.name=\"blondel\" attr.type=\"string\" for=\"node\" id=\"blondel\"/>\n"
#endif
#ifdef injectinfohiermap
		<< "<key attr.name=\"infohiermap\" attr.type=\"string\" for=\"node\" id=\"infohiermap\"/>\n"
#endif
#ifdef injectTaval
		<< "<key attr.name=\"othercomp\" attr.type=\"string\" for=\"node\" id=\"othercomp\"/>\n"
#endif
#ifdef WE
		<< "<key attr.name=\"weight\" attr.type=\"double\" for=\"edge\" id=\"weight\"/>\n"
#endif
		<< "<graph edgedefault=\"undirected\">\n";
	unsigned int i = 0;
	for ( ; i < baseSize; i ++) {
		baseNodes[i].virtualNumber = i;
	}
	i = 0;
	for (; i < baseSize; i ++) {
		buffer << "<node id=\"" << baseNodes[i].virtualNumber << "\">\n"
			<< "<data key=\"label\">" << baseNodes[i].realNumber << "</data>\n"
			<< "<data key=\"comp\">" << baseNodes[i].component << "</data>\n"
#ifdef injectblondel
			<< "<data key=\"blondel\">" << baseNodes[i].otherComponents[0] << "</data>\n"
#endif
#ifdef injectinfohiermap
			<< "<data key=\"infohiermap\">" << baseNodes[i].otherComponents[1] << "</data>\n"
#endif
#ifdef injectTaval
			<< "<data key=\"othercomp\">" << baseNodes[i].otherComponents[2] << "</data>\n"
#endif
			<< "</node>\n";
	}
	for (i = 0; i < baseSize; i ++) {
		for (unsigned int j = 0; j < baseNodes[i].baseDegree; j++) {
			buffer
				<< "<edge id=\"" << baseNodes[i].virtualNumber << "_" << baseNodes[i].connectedNodes[j]->virtualNumber << "\" source=\"" << baseNodes[i].virtualNumber << "\" target=\"" << baseNodes[i].connectedNodes[j]->virtualNumber << "\">\n"
#ifdef WE
				<< "\t<data key=\"weight\">" << baseNodes[i].weight[j] << "</data>\n"
#endif				
				<< "</edge>\n";
		}
	}
	buffer
		<< "</graph>\n"
		<< "</graphml>" << endl;
	ofstream fout(fileName.c_str());
	fout << buffer.rdbuf();
	fout.close();
	buffer.clear();
}

void graph::readFromFile1(string fileName, bool createRandomizedGraph) {
	stringstream buffer;
	ifstream fin(fileName.c_str());
	buffer << fin.rdbuf();
	fin.close();
	unsigned int src, dst;

	map<int, int> virtualNumbers;
	map<int, bool> markNodes;
	baseSize = 0;
#ifndef WE
	while (buffer >> src >> dst) {
#else
	double tmp;
	while (buffer >> src >> dst >> tmp) {
#endif
		if (src == dst)
			continue;
		if (markNodes[src]) {
			if (markNodes[dst]) {
				continue;
			}
			markNodes[dst] = true;
			virtualNumbers[dst] = baseSize ++;
			continue;
		}
		markNodes[src] = true;
		virtualNumbers[src] = baseSize ++;
		if (markNodes[dst]) {
			continue;
		}
		markNodes[dst] = true;
		virtualNumbers[dst] = baseSize ++;
	}

	buffer.clear();
	buffer.seekg(0);
	baseNodes = new node[baseSize];
	for (map<int, int>::iterator itr = virtualNumbers.begin(); itr != virtualNumbers.end(); itr ++) {
		baseNodes[(*itr).second].realNumber = (*itr).first;
	}

	map<long long, bool> markEdges1;
	list<unsigned int> *edges1;
	edges1 = new list<unsigned int>[baseSize];
#ifdef WE
	list<double> *weight;
	weight = new list<double>[baseSize];
	while (buffer >> src >> dst >> tmp) {
#else
	while (buffer >> src >> dst) {
#endif
		if (src == dst)
			continue;
		src = virtualNumbers[src];
		dst = virtualNumbers[dst];
		long long index = src < dst ? src * baseSize + dst : dst * baseSize + src;
		if (markEdges1[index]) {
#ifdef WE
			list<double>::iterator iter = weight[src].begin();
			for (list<unsigned int>::iterator itr = edges1[src].begin(); itr != edges1[src].end(); itr ++, iter ++) {
				if ((*itr) == dst) {
					if ((*iter) < tmp) {
						(*iter) = tmp;
						iter = weight[dst].begin();
						for (itr = edges1[dst].begin(); itr != edges1[dst].end(); itr ++, iter ++) {
							if ((*itr) == src) {
								(*iter) = tmp;
								break;
							}
						}
					}
					break;
				}
			}
#endif
			continue;
		}
		markEdges1[index] = true;
		edges1[src].push_back(dst);
		edges1[dst].push_back(src);
#ifdef WE
		weight[src].push_back(tmp);
		weight[dst].push_back(tmp);
#endif
	}
	baseEdgeSize = 0;
#ifdef WE
	baseWeights = 0;
#endif
	unsigned int i = 0, j;
	for ( ; i < baseSize; i ++) {
		baseNodes[i].virtualNumber = i;
		baseNodes[i].component = 0;
		baseNodes[i].baseDegree = (unsigned int)edges1[i].size();
		baseNodes[i].connectedNodes = new node*[baseNodes[i].baseDegree];
#ifdef WE
		baseNodes[i].weight = new double[baseNodes[i].baseDegree];
		list<double>::iterator it = weight[i].begin();
		baseNodes[i].baseAllWeights = 0;
#endif
		baseEdgeSize += baseNodes[i].baseDegree;
		j = 0;
		for (list<unsigned int>::iterator itr = edges1[i].begin(); itr != edges1[i].end(); itr++, j ++) {
			baseNodes[i].connectedNodes[j] = &baseNodes[(*itr)];
#ifndef WE
		}
	}
#else
			baseNodes[i].weight[j] = (*it);
			baseNodes[i].baseAllWeights += baseNodes[i].weight[j];
			it ++;
		}
		baseWeights += baseNodes[i].baseAllWeights;
	}
	baseWeights = baseWeights / 2;
	delete[](weight);
#endif
	delete[](edges1);
	baseEdgeSize >>= 1;
	buffer.clear();

	stringstream outputBuffer;
	outputBuffer << baseSize << "\t" << baseEdgeSize
#ifdef WE
		<< "\t" << baseWeights
#endif
		;
	for (i = 0; i < baseSize; i ++) {
		outputBuffer << "\n" << baseNodes[i].realNumber << "\t" << baseNodes[i].baseDegree << "\n";
		for (j = 0; j < baseNodes[i].baseDegree; j ++) {
#ifndef WE
			outputBuffer << baseNodes[i].connectedNodes[j]->virtualNumber << "\t";
#else
			outputBuffer << baseNodes[i].connectedNodes[j]->virtualNumber << "\t" << baseNodes[i].weight[j] << "\t";
#endif
		}
	}
	ofstream fout((fileName.substr(0, fileName.size() - 4) + ".smg").c_str());
	fout << outputBuffer.rdbuf();
	fout.close();
	outputBuffer.clear();

	cout << "Number of nodes: " << baseSize << endl;
	cout << "Number of edges: " << baseEdgeSize << endl;
#ifdef WE
	cout << "weights: " << baseWeights << endl;
#endif
}
#ifndef CLS
void graph::createPajekImportFile(string fileName) {
	stringstream buffer;
	buffer
	<< "*Vertices " << baseSize << "\n";

	unsigned int i = 0;
	for (; i < baseSize; i ++) {
		buffer << (i + 1) << " " << "\"" << (i + 1) << "\"" << "\n";
	}
	buffer << "*Edges " << baseEdgeSize << "\n";
	for (i = 0; i < baseSize; i ++) {
		for (unsigned int j = 0; j < baseNodes[i].baseDegree; j++) {
			if (baseNodes[i].virtualNumber <= baseNodes[i].connectedNodes[j]->virtualNumber) {
				buffer
					<< baseNodes[i].virtualNumber + 1 << " " << baseNodes[i].connectedNodes[j]->virtualNumber + 1 <<
#ifndef WE
					" 1\n";
#else
					" " << baseNodes[i].weight[j] << "\n";
#endif
			}
		}
	}
	ofstream fout(fileName.c_str());
	fout << buffer.rdbuf();
	fout.close();
	buffer.clear();
}

void graph::injectBlondleClusters(string filename, int num) {
	stringstream buffer;
	ifstream fin(filename.c_str());
	unsigned int virtualNumber;
#ifdef DD
	unsigned int component;
#else
	string component;
#endif
	if (fin.is_open()) {
		buffer << fin.rdbuf();
		fin.close();
		while (buffer >> virtualNumber >> component) {
#ifdef DD
			baseNodes[virtualNumber].component = component;
#else
			baseNodes[virtualNumber].otherComponents[num] = component;
#endif
		}
	}
	buffer.clear();
}

void graph::injectClusters(string filename, int num) {
#ifdef DD
	map<string, unsigned int> vcomp;
#endif
	stringstream buffer;
	ifstream fin(filename.c_str());
	if (fin.is_open()) {
		buffer << fin.rdbuf();
		fin.close();
		char tmp[100];
		buffer.getline(tmp, 100);
		while (true) {
			int node;
			char t = '\0';
			double d;
			string s;
			buffer.getline(tmp, 100);
			if (buffer.eof())
				break;
			string str(tmp);
			stringstream buf(str);
			buf >> s;
			s.erase(s.find_last_of(':'));
#ifdef DD
			if (vcomp.find(s) == vcomp.end()) {
				vcomp[s] = vcomp.size() - 1;
			}
#endif
			buf >> d >> t >> node >> t;
#ifdef DD
			baseNodes[node - 1].component = vcomp[s];
#else
			baseNodes[node - 1].otherComponents[num] = s;
#endif
		}
	}
}
#endif

void graph::reload(string fileName) {
	stringstream buffer;
	ifstream fin(fileName.c_str());
	buffer << fin.rdbuf();
	fin.close();
	buffer >> baseSize >> baseEdgeSize
#ifdef WE
		>> baseWeights
#endif
		;
	unsigned int i = 0, j, k;
	for (; buffer >> baseNodes[i].realNumber >> baseNodes[i].baseDegree; i ++) {
		baseNodes[i].degree = baseNodes[i].baseDegree;
		baseNodes[i].virtualNumber = i;

		for (j = 0; j < baseNodes[i].baseDegree; j ++) {
			buffer >> k;
			baseNodes[i].connectedNodes[j] = baseNodes + k;
#ifdef WE
			buffer >> baseNodes[i].weight[j];
#endif
		}
	}
	buffer.clear();
}

#ifndef CLS
double graph::calculateModularity(int type, int num) {
	double mod = 0;
	unsigned int i, j;
	for (i = 0; i < baseSize; i++) {
		for (j = 0; j < baseSize; j++) {
			node *n1 = &baseNodes[i];
			node *n2 = &baseNodes[j];
			if (
				(type == 0 && n1 -> component == n2 -> component)
				||
				(type == 1 && n1 -> otherComponents[num] == n2 -> otherComponents[num])
				) {
#ifndef WE
					mod -= ((double) n1 -> baseDegree * n2 -> baseDegree) / (2 * baseEdgeSize);
#else
					mod -= ((double) n1 -> baseAllWeights * n2 -> baseAllWeights) / (2 * baseWeights);
#endif
			}
		}
	}
	for (i = 0; i < baseSize; i++) {
		node *n1 = &baseNodes[i];
		for (j = 0; j < baseNodes[i].baseDegree; j++) {
			node *n2 = baseNodes[i].connectedNodes[j];
			if (
				(type == 0 && n1 -> component == n2 -> component)
				||
				(type == 1 && n1 -> otherComponents[num] == n2 -> otherComponents[num])
				) {
#ifndef WE
				mod += 1;
#else
				mod += n1 -> weight[j];
#endif
			}
		}
	}
#ifndef WE
	return mod / (2 * baseEdgeSize);
#else
	return mod / (2 * baseWeights);
#endif
}
#endif
graph::graph(unsigned int isize) {
	size = isize;
	nodes = new node*[isize];
}

graph::graph(void)
{
}

graph::~graph(void)
{
	delete[](nodes);
}
