#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <ctype.h>
#include <math.h>


#include "Neuron.hpp"


Synapse::Synapse( void ) :
       weight( 0.0 ),
       deltaWeight( 0.0 ),
       learningRate( 1.0 ),
       weightNeedsInit ( 1.0 ), //WK
       neuronNeedsInit ( 0.0 ), //WK
       tempNeuronLevel ( -1 ),  //WK
       tempNeuronNumber ( -1 ), //WK
       srcNeuron( NULL ),
       destNeuron( NULL )
{
}

// Are currently using the default dtor for
// synapse.  Because it does not create the
// neurons to which it attaches, it does not
// need to delete them and so can use the default


Neuron::Neuron( void ) :
       valueInput( 0 ),
       valueOutput( 0 ),
       valueError( 0 ),
       numInputSynapses( 0 ),
       //WK Not used inputSynapses( NULL ),
       numOutputSynapses( 0 ),
       outputSynapses( NULL )
{
}

Neuron::~Neuron( void )
{
       // Design Decision to have the neuron destroy
       // its output synapses and to set the destNeuron
       // of its input synapses to NULL.  Implicit in
       // this decision is that a neural net needs to
       // be destroyed from the output layer back to
       // the input layer (ooh looks like backprop).

       int i ;

       /* WK Unused
       for( i = 0 ;   i < numInputSynapses ;   i++ )
                if  ( inputSynapses[ i ] )
                         inputSynapses[ i ]->destNeuron = NULL ;
       delete [] inputSynapses ;
       */

       for( i = 0 ;   i < numOutputSynapses ;  i++ )
                delete outputSynapses[ i ] ;
       delete [] outputSynapses ;
}

double Neuron::squashingFunction( double value )
{
       return( value ) ;
}

double Neuron::squashingSlope( double /* value */ )
{
       return( 1.0 ) ;
}

int Neuron::acceptsInput( void )
{
       return( 1 ) ;
}

void Neuron::SaveBaseInfo( FILE * fid )
{
    fprintf( fid, "fn: %s %d\n", name, numOutputSynapses ) ;
}

ostream & Neuron::PrintInfo( ostream & os )
{
	return( os ) ;
}


NeuronBias::NeuronBias( void )
{
       valueInput = valueOutput = 1.0 ;
}
NeuronBias::~NeuronBias( void ) {}

double NeuronBias::squashingFunction( double /* value */ )
{
       return( 1.0 ) ;
}

double NeuronBias::squashingSlope( double /* value */ )
{
       return( 1.0 ) ;
}

int NeuronBias::acceptsInput( void )
{
       return( 0 ) ;
}


NeuronThreshold::NeuronThreshold( void ) :
       threshold( 0.5 )
{
}

NeuronThreshold::NeuronThreshold( double thresh )
{
       threshold = thresh ;
}

NeuronThreshold::~NeuronThreshold( void ) {}

double NeuronThreshold::squashingFunction( double value )
{
       // return( value >= threshold ) ;
       if  ( value >= threshold )
                return( 1.0 ) ;
       else return( -1.0 ) ;
}

double NeuronThreshold::squashingSlope( double value )
{
       double expA    = exp( value ) ;
       double expNegA = exp( -value ) ;

       double gOfA = ( (expA - expNegA) / (expA + expNegA) ) ;

       return( 1.0 - gOfA * gOfA ) ;

       // double gOfA = 1.0 / ( 1.0 + exp( -value ) ) ;
       // return( gOfA * ( 1.0 - gOfA ) ) ;
}

void NeuronThreshold::SaveBaseInfo( FILE * fid )
{
    fprintf( fid, "fn: %s %d %.10lf\n", name, numOutputSynapses, threshold ) ;
}

ostream & NeuronThreshold::PrintInfo( ostream & os )
{
	os << "Threshold " << threshold ;
	return( os ) ;
}


NeuronLogistic::NeuronLogistic( void ) :
	scalar( 0.001 )
{
}

NeuronLogistic::NeuronLogistic( double scaleVal ) :
	scalar( scaleVal )
{
}

NeuronLogistic::~NeuronLogistic( void ) {}

double NeuronLogistic::squashingFunction( double value )
{
       return( ( 1.0 / ( 1.0 + exp( -value ) ) ) + scalar * value ) ;
}

double NeuronLogistic::squashingSlope( double value )
{
       double gOfA = squashingFunction( value ) ;
       return( scalar + gOfA * ( 1.0 - gOfA ) ) ;
}

void NeuronLogistic::SaveBaseInfo( FILE * fid )
{
    fprintf( fid, "fn: %s %d %.10lf\n", name, numOutputSynapses, scalar ) ;
}

ostream & NeuronLogistic::PrintInfo( ostream & os )
{
	os << "Logistic " << scalar ;
	return( os ) ;
}


NeuronTanh::NeuronTanh( void )
{
}

NeuronTanh::~NeuronTanh( void ) {}

double NeuronTanh::squashingFunction( double value )
{
       double expA    = exp( value ) ;
       double expNegA = exp( -value ) ;

       return( (expA - expNegA) / (expA + expNegA) ) ;
}

double NeuronTanh::squashingSlope( double value )
{
       double gOfA = squashingFunction( value ) ;

       return( 1.0 - gOfA * gOfA ) ;
}


NeuronFunnyTanh::NeuronFunnyTanh( void ) :
	scalar( 0.001 )
{
}

NeuronFunnyTanh::NeuronFunnyTanh( double scaleVal ) :
	scalar( scaleVal )
{
}

NeuronFunnyTanh::~NeuronFunnyTanh( void ) {}

static const double TWO_THIRDS = 2.0 / 3.0 ;
double NeuronFunnyTanh::squashingFunction( double value )
{
       return( 1.7519 * tanh( TWO_THIRDS * value ) + scalar * value ) ;
}

double NeuronFunnyTanh::squashingSlope( double value )
{
       double pseudoGOfA = tanh( TWO_THIRDS * value ) ;

       return( scalar + 1.7519 * TWO_THIRDS * ( 1.0 - pseudoGOfA * pseudoGOfA ) ) ;
}

void NeuronFunnyTanh::SaveBaseInfo( FILE * fid )
{
    fprintf( fid, "fn: %s %d %.10lf\n", name, numOutputSynapses, scalar ) ;
}

ostream & NeuronFunnyTanh::PrintInfo( ostream & os )
{
	os << "fTanh " << scalar ;
	return( os ) ;
}



NeuronFactory::NeuronFactory( void ) {}
NeuronFactory::~NeuronFactory( void ) {}
Neuron * NeuronFactory::createNeuron( char * fnStr, double tmpDbl )
{
       if  ( !strcasecmp( "logistic", fnStr ) )
	{
		if  ( tmpDbl == 0.0 )
                	return( new NeuronLogistic() ) ;
                else return( new NeuronLogistic( tmpDbl ) ) ;
	}
       else if  ( !strcasecmp( "tanh", fnStr ) )
                return( new NeuronTanh() ) ;
       else if  ( !strcasecmp( "ftanh", fnStr ) )
	{
		if  ( tmpDbl == 0.0 )
                	return( new NeuronFunnyTanh() ) ;
                else return( new NeuronFunnyTanh( tmpDbl ) ) ;
	}
       else if  ( !strcasecmp( "bias", fnStr ) )
                return( new NeuronBias() ) ;
       else if  ( !strcasecmp( "threshold", fnStr ) )
                return( new NeuronThreshold( tmpDbl ) ) ;
       else return( new Neuron() ) ;
}



ostream & operator<<( ostream & os, Synapse & synapse )
{
       os << synapse.destNeuron
                << " <-- "
                << setw(5) << setprecision( 5 ) << synapse.weight
                << " <-- "
                << synapse.srcNeuron ;
       return( os ) ;
}


ostream & operator<<( ostream & os, Neuron & neuron )
{
       os.setf( ios::fixed ) ;
       os << "   " << &neuron << " " ;
	neuron.PrintInfo( os ) << " F( "
                << setw(5) << setprecision( 5 ) << neuron.valueInput
                << " ) --> "
                << setw(5) << setprecision( 5 ) << neuron.valueOutput
                << "   ==> "
                << setw(5) << setprecision( 5 ) << neuron.valueError ;

       for( int i = 0 ;   i < neuron.numOutputSynapses ;   i++ )
                os << "\n\t" << *neuron.outputSynapses[ i ] ;

       return( os ) ;
}


ostream & operator<<( ostream & os, NeuronLogistic & neuron )
{
       os << "Logistic" << ((Neuron &) neuron) ;
       return( os ) ;
}


ostream & operator<<( ostream & os, NeuronTanh & neuron )
{
       os << "Tanh" << ((Neuron &) neuron) ;
       return( os ) ;
}


ostream & operator<<( ostream & os, NeuronFunnyTanh & neuron )
{
       os << "F" << ((NeuronTanh &) neuron) ;
       return( os ) ;
}


