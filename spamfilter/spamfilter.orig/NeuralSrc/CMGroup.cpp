
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>

#include "CMGroup.hpp"


CMGroup::CMGroup( void ) :
	numCMs( -1 ),
	numDims( -1 ),
	cms( NULL ),
	rate( 1.0 ),
	middles( NULL )
{
}

CMGroup::~CMGroup( void )
{
	Release() ;
}

void CMGroup::Release( void )
{
	for( int i = 0 ;   i <= numCMs ;   i++ )
		delete [] cms[ i ] ;
	delete [] cms ;
}

void CMGroup::Allocate( double highRate, int dimCnt, double * dataMiddles )
{
	Release() ;

	numDims = dimCnt ;
	middles = dataMiddles ;

	numCMs = 1 ;
	rate = 1.0 - highRate ;
	if  ( ( rate <= 0.0 )  ||  ( rate > 1.0 ) )
		rate = 1.0 ;
	if  ( ( highRate < 1 )  &&  ( highRate >= 0.01 ) )
	{
		numCMs = 1.0 / (1.0 - highRate) ;
		if  ( highRate * numCMs < 1.0 )
			numCMs += 1 ;
	}

	cms = new ConfusionMatrix * [ numCMs + 1 ] ;
	for( int i = 0 ;   i <= numCMs ;   i++ )
		cms[ i ] = new ConfusionMatrix[ numDims ] ;
}

void CMGroup::Reset( void )
{
	for( int i = 0 ;   i <= numCMs ;   i++ )
		for( int j = 0 ;   j < numDims ;   j++ )
			(cms[ i ])[ j ].reset() ;

	cmsCum.reset() ;
}


int * CMGroup::AddResult( double * expected, double * actuals )
{
	int * state = new int[ numDims ] ;
	memset( state, 0, numDims * sizeof( int ) ) ;

	for( int odIndex = 0 ;   odIndex < numDims ;   odIndex++ )
	{
		double resultActual = actuals[ odIndex ] ;
		double resultExpect = expected[ odIndex ] ;
		double resultMiddle = middles[ odIndex ] ;
		// double resultMiddle = 0.0 ;

// cerr << "exp: " << resultExpect << "   act: " << resultActual << "   mid: " << resultMiddle << endl ;

		if  ( resultExpect > resultMiddle )
		{
			int cnt = 0 ;
			double offset = (resultExpect - resultMiddle) * rate ;
			while( ( cnt < numCMs )  &&
				   ( ( resultActual < ( resultExpect - (cnt + 1) * offset ) )  &&
				 	 ( resultActual > ( 2 * resultMiddle - resultExpect + (cnt + 1) * offset ) ) ) )
			{
// cerr << "up cnt: " << cnt << "  " << ( resultExpect - (cnt + 1) * offset ) << ", " << ( 2 * resultMiddle - resultExpect + (cnt + 1) * offset ) << endl ;
				cnt++ ;
			}

			if  ( resultActual >= resultMiddle )
			{
				(cms[ cnt ])[ odIndex ].incTrueNegative() ;
				cmsCum.incTrueNegative() ;
				state[ odIndex ] = 0 ;
			}
			else
			{
				(cms[ cnt ])[ odIndex ].incFalsePositive() ;
				cmsCum.incFalsePositive() ;
				state[ odIndex ] = 2 ;
			}
		}
		else
		{
			int cnt = 0 ;
			double offset = (resultMiddle - resultExpect) * rate ;
			while( ( cnt < numCMs )  &&
				   ( ( resultActual > ( resultExpect + (cnt + 1) * offset ) )  &&
				 	 ( resultActual < ( 2 * resultMiddle - resultExpect - (cnt + 1) * offset ) ) ) )
			{
// cerr << "down cnt: " << cnt << ( resultExpect + (cnt + 1) * offset ) << ", " << ( 2 * resultMiddle - resultExpect - (cnt + 1) * offset ) << endl ;
				cnt++ ;
			}

			if  ( resultActual <= resultMiddle )
			{
				(cms[ cnt ])[ odIndex ].incTruePositive() ;
				cmsCum.incTruePositive() ;
				state[ odIndex ] = 3 ;
			}
			else
			{
				(cms[ cnt ])[ odIndex ].incFalseNegative() ;
				cmsCum.incFalseNegative() ;
				state[ odIndex ] = 1 ;
			}
		}
	}

	return( state ) ;
}


ostream & operator<<( ostream & os, CMGroup & cmgrp )
{
	for( int odIndex = 0 ;   odIndex < cmgrp.numDims ;   odIndex++ )
	{
		os << endl << "Output Dimension " << odIndex << endl ;
		for( int i = 0 ;   i < cmgrp.numCMs ;   i++ )
			os << (1.0 - (i+1) * cmgrp.rate) << "  " << (cmgrp.cms[ i ])[ odIndex ] << endl ;
		os << "Slop: " << (cmgrp.cms[ i ])[ odIndex ] << endl ;
	}
	os << "Cumulative Results:" << endl << cmgrp.cmsCum << endl ;

	return( os ) ;
}
