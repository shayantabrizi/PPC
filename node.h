#pragma once
#include <list>
#include "fib.h"
#include <string>
using namespace std;

#define one 1
#define newpn 1
//#define HA 1
#define PAPER 1
#define dfs 1
//#define CLS
//#define WE 1

class node
{
public:
	class nodeFrequency {
	public:
#ifndef PAPER
		node *inode;
#endif
		double frequency;
	};
	class linkList {
	public:
		node *inode1;

		nodeFrequency inodeFrequency;
#ifdef PAPER
		linkList *next;
#else
		linkList *previous, *next;
#endif

		inline void erase();
		inline linkList* insert(nodeFrequency&);
	};
#ifndef PAPER
	list<linkList*> jumps1; 
#endif
	linkList *walkedNodes1;

	void reset();

	void eraseFromWalkedNode(linkList*);
	linkList* insertWalkingNodeToWalkedNode();
#ifndef PAPER
	void updateWalkedNodes();
#endif
	int realNumber;
	int virtualNumber;
	int component;
#ifdef hier
	string hi;
#endif
#ifndef CLS
	string otherComponents[3];
#endif
	node **connectedNodes;
#ifdef WE
	//unsigned int *weight;
	//unsigned int baseAllWeights;
	double *weight;
	double baseAllWeights;
#endif
	unsigned int inIncident;
	unsigned int baseDegree;
	unsigned int degree;
	bool seed;
	int group;
	double score;
	fibheap_el *fibNode;
#ifdef WE
	//unsigned int *majmueTajamoei;
	double *majmueTajamoei;
#endif

	nodeFrequency walkingNode;

	node(void);
	~node(void);
};
