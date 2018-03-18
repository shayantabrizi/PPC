-----------------------------------------------------------------------------

PPC: A scalable graph clustering algorithm

Based on the article "Personalized PageRank Clustering: A Graph Clustering Algorithm Based on Random Walks"
Authors: S.A. Tabrizi, A. Shakery, M. Asadpour, M. Abbasi, M.A. Tavallaie
Version: 1.2

-----------------------------------------------------------------------------

Code Author	: 	M. Abbasi
Email		: 	ma.abbasi@ut.ac.ir
University	:	University of Tehran, Iran

-----------------------------------------------------------------------------

Disclaimer:
We really appreciate bug reports and any suggestions for improving the algorithm. Please contact us via email at s.tabrizi@ut.ac.ir.

usage:
	The second argument is optional and changes the random number generator seed.

1) For simple undirected graphs:
	./clusterer test 0

clusterer reads Datasets/test/test.txt
test.txt consists of the network edges in the format of "src dst"

2)	For weighted undirected graphs:
	./weclusterer test 0
	./weintclusterer test 0

weintclusterer is optimized for integer edge weights.
weclusterer (weintclusterer) reads Datasets/test/test.txt
test.txt consists of network edges in the format of "src dst weight"
In case of conflicts, e.g. "2 5 2.5", "2 5 3.5" and "5 2 3", they consider "2 5 3.5", in which the weight is the maximum of the input weights.

Program automatically generates Datasets/test/test.smg for future use and it might take more time in the first run. In the future releases, this part should be more optimized.
    if you have a .smg file, you don't need the .txt file anymore.

if you like to compile the program manually, here is what you need:
-DcompLimit=40				you can set a limit for the number of clusters
-DexactClustersCount			using it in conjunction with -DcompLimit produces exactly compLimit clusters (Useful for when the number of clusters is known) (compLimit must be not higher than the number of vertices)
-DpageRankAlpha=0.7			to change the PageRank alpha value
-DTRIAL_DEFAULT=50			minimum number of random jumps starting from a vertex
-DTRIAL_DEFUALT_MULT=10		number of random jumps starting from a vertex = max{TRIAL_DEFUALT_MULT * node's degree, DTRIAL_DEFAULT}
-DHA 					creates .tree output file
-Dhier					creates .tree output file: with hierarchy -> Datasets/football/hier_football.tree
-Dcreategraphml 		creates .graphml output file
-DWE					for weighted graphs with float weights
-DWE -DintWeight		for weighted graphs with integer weights

-----------------------------------------------------------------------------

Change Log:
Release 1.2:
	Bug Fixes:
		Fixed a bug in reading big graph files (more than about 60000 vertices).
Release 1.1:
	New Features:
		Added support for determining the exact number of clusters.
	Bug Fixes:
		Fixed some issues.
Release 1.0
	

