all:
	g++ -DHA -Dcreategraphml -O3 -o clusterer *.cpp fib/fib.cpp
	g++ -DHA -Dcreategraphml -DWE -O3 -o weclusterer *.cpp fib/fib.cpp
	g++ -DHA -Dcreategraphml -DWE -DintWeight -O3 -o weintclusterer *.cpp fib/fib.cpp
clean:
	rm clusterer
	rm weclusterer
	rm weintclusterer
