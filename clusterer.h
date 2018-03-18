#pragma once
#include "network.h"
#include <vector>
using namespace std;

class clusterer
{
	vector<network*> results;
	string dataset;

public:
#ifndef CLS
	void connectedness1(double&, unsigned int&);
	void connectedness(double&);
	double fitnessAvg(double);
	void ghotr(unsigned int);
	//void calculateModularity();
#endif
	unsigned int compNum;
	node **nodes;
	void calc();
	double calculateModularity(unsigned int, unsigned int);
	void run();
	clusterer(string dataset);
	~clusterer(void);
};
