
#include <iostream.h>
#include <iomanip.h>
#include <math.h>

#include "ConfMatrix.hpp"


double ConfusionMatrix::positiveScale = 1.0 ;
double ConfusionMatrix::negativeScale = 1.0 ;

ConfusionMatrix::ConfusionMatrix( void ) :
	tp( 0 ),
	fp( 0 ),
	tn( 0 ),
	fn( 0 ),
	pp( 0.0 ),
	np( 0.0 )
{
}


ConfusionMatrix::~ConfusionMatrix( void )
{
}

double ConfusionMatrix::precision( void )
{
	double tp1 = positiveScale * tp ;
	double fp1 = negativeScale * fp ;

	return( tp1 / ( tp1 + fp1 ) ) ;
}


double ConfusionMatrix::recall( void )
{
	double tp1 = positiveScale * tp ;
	double fn1 = positiveScale * fn ;

	return( tp1 / ( tp1 + fn1 ) ) ;
}


double ConfusionMatrix::accuracy( void )
{
	double tp1 = positiveScale * tp ;
	double fp1 = negativeScale * fp ;
	double fn1 = positiveScale * fn ;
	double tn1 = negativeScale * tn ;

	return( ( tp1 + tn1 ) / ( tp1 + fp1 + fn1 + tn1 ) ) ;
}



ostream & operator<<( ostream & os, ConfusionMatrix & cm )
{
	double tp1 = ConfusionMatrix::positiveScale * cm.tp ;
	double fp1 = ConfusionMatrix::negativeScale * cm.fp ;
	double fn1 = ConfusionMatrix::positiveScale * cm.fn ;
	double tn1 = ConfusionMatrix::negativeScale * cm.tn ;

	os
			<< "    Train:  Pos      Neg   \n"
			   "   NN:  +-------------------\n"
			   "    Pos |  "
			<< setw( 7 ) << setprecision( 5 ) << tp1 << "  " 
			<< setw( 7 ) << setprecision( 5 ) << fp1 << "  "
			<< setw( 7 ) << setprecision( 5 ) << (tp1 + fp1) << "  "
			<< setw( 6 ) << setprecision( 5 ) << cm.precision()
			<< endl

			<< "    Neg |  " 
			<< setw( 7 ) << setprecision( 7 ) << fn1 << "  "
			<< setw( 7 ) << setprecision( 7 ) << tn1 << "  " 
			<< setw( 7 ) << setprecision( 5 ) << (fn1 + tn1) << "  "
			<< setw( 6 ) << setprecision( 5 ) << (fn1/(tn1+fn1))
			<< endl

			<< "    Sum |  " 
			<< setw( 7 ) << setprecision( 7 ) << (tp1 + fn1) << "  "
			<< setw( 7 ) << setprecision( 7 ) << (fp1 + tn1) << "  " 
			<< setw( 7 ) << setprecision( 5 ) << (tp1 + fn1 + fp1 + tn1) // << "  "
			// << setw( 6 ) << setprecision( 5 ) << (double(tp1 + tn1)/double(tp1+fp1+tn1+fn1))
			<< endl

			<< "           "
			<< setw( 6 ) << setprecision( 5 ) << cm.recall() << "  "
			<< setw( 6 ) << setprecision( 5 ) << (fp1/(tn1+fp1)) << "    " << "        " 
			<< setw( 6 ) << setprecision( 5 ) << cm.accuracy()
		<< endl ;

	return( os ) ;
}


