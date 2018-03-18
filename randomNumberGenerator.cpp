#include "randomNumberGenerator.h"
#include <time.h>

#ifdef R1
#ifndef R3
unsigned int randomNumberGenerator::nextInt() {
	int hi = seed / qqq;
	int lo = seed % qqq;
	int test = aaa * lo - rrr * hi;
	if(test > 0)
		seed = test;
	else
		seed = test + mmm;
	return seed - 1;
}

int randomNumberGenerator::seed = SEED_DEFAULT_VALUE;
#else
#include <iostream>
unsigned int randomNumberGenerator::nextInt() {
	static unsigned int x=123456789, y=362436069, z=521288629;

	unsigned int t;

	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;
	//std::cout << x << " " << y << " " << z << std::endl;

	return z;
	//return rand() * 0x7fff + rand();
	//return rand();
	//return time(NULL);
}
#endif

double randomNumberGenerator::nextReal() {
	return randomNumberGenerator::nextInt() / (double)rand_max_val;
}
#endif

#ifdef R2
#include <limits>
using namespace std;
mt19937 randomNumberGenerator::rng(3u);
uniform_int_distribution<> randomNumberGenerator::uInt32dist(0, numeric_limits<int>::max());
variate_generator<mt19937&, uniform_int_distribution<>> randomNumberGenerator::nextInt(rng, uInt32dist);
uniform_01<mt19937> randomNumberGenerator::nextReal(rng);
#endif
