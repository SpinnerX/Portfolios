#ifndef NEURON_HPP
#define NEURON_HPP


#include <iostream>
#include <stdio.h>
using namespace std;

class Neuron ;


class Synapse
{
public:

       Synapse( void ) ;
       // Use default dtor

       double weight ;
       double deltaWeight ;    // BB for momentum
       double learningRate ;
       Neuron * srcNeuron ;
       Neuron * destNeuron ;
       int weightNeedsInit;  //WK If weight needs to be set to sqrt(fanin)
       int neuronNeedsInit;  //WK If neuron hasn't been seen yet
       int tempNeuronLevel;  //WK Level destNeuron will be in
       int tempNeuronNumber; //WK Number destNeuon will be

} ;


class Neuron
{
public:

       Neuron( void ) ;
       virtual ~Neuron( void ) ;

	virtual void SaveBaseInfo( FILE * fid ) ;

       char name[80]; //WK Holds "ftanh" or bias or whatever
       double valueInput ;
       double valueOutput ;
       double valueError ;

       int numInputSynapses ; //WK Used only for fanin
       //WK Unused Synapse ** inputSynapses ;

       int numOutputSynapses ;
       Synapse ** outputSynapses ;

       virtual double squashingFunction( double value ) ;
       virtual double squashingSlope( double value ) ;
       virtual int    acceptsInput( void ) ;

	virtual ostream & PrintInfo( ostream & os ) ;

} ;


class NeuronBias : public Neuron
{
public:

       NeuronBias( void ) ;
       virtual ~NeuronBias( void ) ;

       virtual double squashingFunction( double value ) ;
       virtual double squashingSlope( double value ) ;
       virtual int    acceptsInput( void ) ;

} ;


class NeuronThreshold : public Neuron
{
public:

       NeuronThreshold( void ) ;
       NeuronThreshold( double thresh ) ;
       virtual ~NeuronThreshold( void ) ;

       virtual double squashingFunction( double value ) ;
       virtual double squashingSlope( double value ) ;

	virtual void SaveBaseInfo( FILE * fid ) ;

	virtual ostream & PrintInfo( ostream & os ) ;

       double threshold ;
} ;


class NeuronLogistic : public Neuron
{
public:

       NeuronLogistic( void ) ;
	NeuronLogistic( double scaleVal ) ;
       virtual ~NeuronLogistic( void ) ;

       virtual double squashingFunction( double value ) ;
       virtual double squashingSlope( double value ) ;

	virtual void SaveBaseInfo( FILE * fid ) ;

	virtual ostream & PrintInfo( ostream & os ) ;

	double scalar ;
} ;


class NeuronTanh : public Neuron
{
public:

       NeuronTanh( void ) ;
       virtual ~NeuronTanh( void ) ;

       virtual double squashingFunction( double value ) ;
       virtual double squashingSlope( double value ) ;

} ;


class NeuronFunnyTanh : public NeuronTanh
{
public:

       NeuronFunnyTanh( void ) ;
	NeuronFunnyTanh( double scaleVal ) ;
       virtual ~NeuronFunnyTanh( void ) ;

       virtual double squashingFunction( double value ) ;
       virtual double squashingSlope( double value ) ;

	virtual void SaveBaseInfo( FILE * fid ) ;

	virtual ostream & PrintInfo( ostream & os ) ;

	double scalar ;
} ;


class NeuronFactory
{
public:

       NeuronFactory( void ) ;
       ~NeuronFactory( void ) ;

       Neuron * createNeuron( char * fnStr, double tmpDbl ) ;

} ;


ostream & operator<<( ostream & os, Synapse & synapse ) ;
ostream & operator<<( ostream & os, Neuron & neuron ) ;
ostream & operator<<( ostream & os, NeuronLogistic & neuron ) ;
ostream & operator<<( ostream & os, NeuronTanh & neuron ) ;
ostream & operator<<( ostream & os, NeuronFunnyTanh & neuron ) ;


#endif
