
#include <stdlib.h>
#include <math.h>

#include "normal.hpp"

static const int iterations = 12 ;

double normalRandom( double sigma )
{
       double sum = 0.0 ;
       for( int i = 0 ;   i < iterations ;   i++ )
               sum += -1 + 2 * drand48() ;
       return( sigma * sum / sqrt( double( iterations ) / 3.0 ) ) ;
}
