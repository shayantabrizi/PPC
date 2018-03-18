#include "clusterer.h"
#include "randomNumberGenerator.h"
#include <limits>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
using namespace std;
#include <stdlib.h>

int main(int argc, char*argv[]) {
	unsigned int seed = 0;
	if (argc == 3) {
		seed = atoi(argv[2]);
	}
	for (unsigned int i = 0; i < seed; i ++) {
		randomNumberGenerator::nextInt();
	}
	string dataset;
	string command(
		"usage:\n"
		"\t- ./clusterer dataset [seed]\n"
		"\n"
		"relative directory for graph input file: Datasets\\dataset\\dataset.txt\n"
		"\t- a smg file can be used too. clusterer looks for a smg file, if there wasn't such file, it reads graph input file and generates Datasets\\dataset\\dataset.smg for future use.");
	if (argc > 1) {
		dataset = argv[1];
		struct stat buffer;
		if (stat(("Datasets/" + dataset + "/" + dataset + ".smg").c_str(), &buffer) != 0
			&&
			stat(("Datasets/" + dataset + "/" + dataset + ".txt").c_str(), &buffer) != 0) {
				cout << command << endl;
				return -1;
		}

		clusterer pageRankClusterer(argv[1]);
		pageRankClusterer.run();
	} else {
		cout << command << endl;
	}
	return 0;
}
