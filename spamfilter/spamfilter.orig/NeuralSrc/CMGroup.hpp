#ifndef CM_GROUP_HPP
#define CM_GROUP_HPP

#include <iostream>

#include "ConfMatrix.hpp"
using namespace std;

class CMGroup
{
public:

	CMGroup( void ) ;
	~CMGroup( void ) ;

	void Release( void ) ;
	void Allocate( double highRate, int numOutputDims, double * dataMiddles ) ;

	void Reset( void ) ;

	// output is cell index, row major order { tp=0, fn=1, fp=2, tn=3 }
	int * AddResult( double * expected, double * actual ) ;

	friend ostream & operator<<( ostream & os, CMGroup & cmgrp ) ;

// protected:

	int numCMs ;
	int numDims ;

	double rate ;
	double * middles ;

	ConfusionMatrix ** cms ;
	ConfusionMatrix cmsCum ;


};

#endif
