all:
	g++ -DHA -Dcreategraphml -O3 -o clusterer *.cpp
	g++ -DHA -Dcreategraphml -DWE -O3 -o weclusterer *.cpp
	g++ -DHA -Dcreategraphml -DWE -DintWeight -O3 -o weintclusterer *.cpp
clean:
	rm clusterer
	rm weclusterer
	rm weintclusterer
