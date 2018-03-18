#pragma once

#define R3 1

#ifdef R3
#define R1 1
#endif

#ifndef R3
#ifdef R1
#define SEED_DEFAULT_VALUE ((1 << 2) - 1)

#define aaa 16807
#define mmm 2147483647
#define qqq (mmm / aaa)
#define rrr (mmm % aaa)
#define rand_max_val 4294967296
#endif
#endif

#ifdef R2
#include "boost/random/uniform_int.hpp"
#include "boost/random/uniform_01.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/variate_generator.hpp"
using namespace boost::random;
#endif

#ifdef R3
//#define rand_max_val 1073676288
#define rand_max_val 0x100000000
#endif

class randomNumberGenerator {
#ifdef R2
private:
	static mt19937 rng;
	static uniform_int_distribution<> uInt32dist;
public:
	static variate_generator<mt19937&, uniform_int_distribution<>> nextInt;
	static uniform_01<mt19937> nextReal;
#elif R1
private:
	static int seed;
public:
	static double nextReal();
	static unsigned int nextInt();
#endif
};
