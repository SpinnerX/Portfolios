#ifndef NEURAL_NET_HPP
#define NEURAL_NET_HPP

#include <iostream>

#include "Neuron.hpp"
using namespace std;

class NeuralNetLevel
{
public:

       NeuralNetLevel( char * p_name, int p_num ) ;
       ~NeuralNetLevel( void ) ;

       char *    name ;
       int     numNeurons ;
       Neuron ** neurons ;

} ;


class NeuralNet
{
public:

       NeuralNet( void ) ;
       ~NeuralNet( void ) ;

       void Load( char * filename ) ;
// BB awaiting work from WK -- WK Done
       void Save( char * filename ) ;

       void AssignInputs( double input[] ) ;
       void FeedForward( void ) ;
       double ComputeOutputError( double trainingValues[] ) ;
       void BackPropagate( double learningRate ) ; //WK Returns bits of direction

       int             NumLevels( void ) ;
       NeuralNetLevel * Level( int level ) ;


private:

       int              _numLevels ;
       NeuralNetLevel ** _levels ;

       friend ostream & operator<<( ostream & os, NeuralNet & net ) ;

} ;


ostream & operator<<( ostream & os, NeuralNetLevel & netLevel ) ;
ostream & operator<<( ostream & os, NeuralNet & net ) ;

#endif
