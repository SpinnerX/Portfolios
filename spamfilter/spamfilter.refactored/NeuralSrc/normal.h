#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <random>
// normalRandom() - generates random number along a normal distribution
// sigma - is a scaling factor between 0.0001 to 0.9

static const int iterations = 12;

double normalRandom(double sigma){
	double sum = 0.0;
	for(size_t i = 0; i < iterations; i++) sum += -1 + 2 * drand48();

	return (sigma * sum / std::sqrt(double(iterations) / 3.0));
}
