
#ifndef ConfusionMatrix_hpp
#define ConfusionMatrix_hpp

#include <iostream>
using namespace std;

class ConfusionMatrix
{

public:

	ConfusionMatrix( void ) ;
	virtual ~ConfusionMatrix( void ) ;

	inline int truePositive( void )		{ return( tp ) ; }
	inline int falsePositive( void )	{ return( fp ) ; }
	inline int trueNegative( void )		{ return( tn ) ; }
	inline int falseNegative( void )	{ return( fn ) ; }

	inline void truePositive( int value )	{ tp = value ; }
	inline void falsePositive( int value )	{ fp = value ; }
	inline void trueNegative( int value )	{ tn = value ; }
	inline void falseNegative( int value )	{ fn = value ; }

	inline void incTruePositive( void )		{ tp++ ; }
	inline void incFalsePositive( void )	{ fp++ ; }
	inline void incTrueNegative( void )		{ tn++ ; }
	inline void incFalseNegative( void )	{ fn++ ; }

	double precision( void ) ;
	double recall( void ) ;
	double accuracy( void ) ;

	inline void refreshPosProbability( void )	{ pp = double( tp + fp ) / double( tp + fp + tn + fn ) ; }
	inline void refreshNegProbability( void )	{ np = double( tn + fn ) / double( tp + fp + tn + fn ) ; }
	inline double posProbability( void )		{ return( pp ) ; }
	inline double negProbability( void )		{ return( np ) ; }


	inline void reset( void ) { tp = fp = tn = fn = 0 ;  pp = np = 0.0 ; }

	friend ostream & operator<<( ostream & os, ConfusionMatrix & cm ) ;

protected:

	int tp, fp, tn, fn ;

	double pp, np ;

public:

	static double positiveScale ;
	static double negativeScale ;

} ;


ostream & operator<<( ostream & os, ConfusionMatrix & cm ) ;


#endif
