#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>


#include "CmdLineArgs.hpp"
#include "NeuralNet.hpp"
#include "normal.hpp"


extern int BBDebug ;

extern CmdLineArgs cmdLineArgs ;



NeuralNetLevel::NeuralNetLevel( char * p_name, int p_num ) :
       name( strdup( p_name ) ),
       numNeurons( p_num )
{
       neurons = new Neuron * [ numNeurons ] ;
}


NeuralNetLevel::~NeuralNetLevel( void )
{
       for( int i = 0 ;   i < numNeurons ;   i++ )
               delete neurons[ i ] ;
       delete [] neurons ;
}



NeuralNet::NeuralNet( void ) :
       _numLevels( 0 ),
       _levels( NULL )
{
}


NeuralNet::~NeuralNet( void )
{
}


int NeuralNet::NumLevels( void )
{
       return( _numLevels ) ;
}


NeuralNetLevel * NeuralNet::Level( int level )
{
       return( _levels[ level ] ) ;
}

//WK This whole function
void NeuralNet::Save( char * filename ) {
       FILE * fid = fopen( filename, "w" ) ;
       if (fid != NULL) {
               fprintf(fid,"Levels: %i\n",_numLevels);
               for (int i = 0; i < _numLevels; i++) {
                       char lname[80];
                       if (i == 0)
                               strncpy(lname,"Output",sizeof(lname));
                       else if (i != _numLevels - 1)
                               strncpy(lname,"Hidden",sizeof(lname));
                       else
                               strncpy(lname,"Input",sizeof(lname));
                       fprintf(fid,"Level: %s %i\n",lname, Level(i)->numNeurons);
                       for (int j = 0; j < Level(i)->numNeurons; j++) {
                               // BB fprintf(fid,"  fn: %s",Level(i)->neurons[j]->name);
                               // BB new fprintf( fid, "  fn: " ) ;
                               Level(i)->neurons[j]->SaveBaseInfo( fid ) ;

                               // BB new if (i == 0) fprintf(fid,"\n");
                               if (i > 0)
							   {
                                       // BB new fprintf(fid," %i\n",Level(i)->neurons[j]->numOutputSynapses);
                                       for (int k = 0; k < Level(i)->neurons[j]->numOutputSynapses; k++) {
                                               Synapse *syn = Level(i)->neurons[j]->outputSynapses[k];
                                               fprintf(fid,"    syn: %i s %lf 0.0 %i %i\n",k,syn->weight,syn->tempNeuronLevel,syn->tempNeuronNumber);
                                       }
                               }
                       }
               }
		fclose( fid ) ;
       }
}

void NeuralNet::Load( char * filename )
{
       FILE * fid = fopen( filename, "r" ) ;
       NeuronFactory factory ;

       if  ( fid != NULL )
       {
               char * str ;
               char sstr[ 256 ] ;
               char tmpStr[ 256 ] ;
               int  tmpInt, tmpInt2, tmpInt3 ;
               double tmpDbl = 0 ;
               int  openLevel = -1 ;
               int  neuronId ;
               int  i ;
			   int  tmpIntBB ;
               Neuron * newNeuron = NULL ;

               while( !feof( fid )  &&  ( fgets( sstr, 256, fid ) != NULL ) )
               {
                       str = sstr ;
                       while( isspace( str[0] ) )
                               str++ ;

                       if  ( '#' != str[0] )
                       {
                               if  ( sscanf( str, "Levels:%d", &_numLevels ) )
                               {
                                       _levels = new NeuralNetLevel * [ _numLevels ] ;
                               }
                               else if  ( sscanf( str, "Level:%s%d", tmpStr, &tmpInt ) )
                               {
                                       openLevel++ ;
                                       // cerr << "Reading level: " << openLevel << ": " << tmpStr << ": " << tmpInt << endl ;

                                       _levels[ openLevel ] = new NeuralNetLevel( tmpStr, tmpInt ) ;
                                       neuronId = -1 ;
                               }
                               else if  ( tmpIntBB = sscanf( str, "fn:%s%d%lf", tmpStr, &tmpInt, &tmpDbl ) )
                               {
									   if  ( tmpIntBB < 3 )
										tmpDbl = 0.0 ;
                                       // cerr << "Read fn = \"" << tmpStr << "\"" << endl ;
                                       newNeuron = factory.createNeuron( tmpStr, tmpDbl ) ;
                                       strncpy(newNeuron->name,tmpStr,sizeof(newNeuron->name));
                                       _levels[ openLevel ]->neurons[ ++neuronId ] = newNeuron ;
                                       if  ( openLevel > 0 )
                                       {
                                               // newNeuron->numOutputSynapses = _levels[ openLevel - 1 ]->numNeurons ;
                                               newNeuron->numOutputSynapses = tmpInt ;
                                               newNeuron->outputSynapses = new Synapse * [ newNeuron->numOutputSynapses ] ;
                                               for( i = 0 ;   i < newNeuron->numOutputSynapses ;   i++ )
                                               {
                                                       newNeuron->outputSynapses[ i ] = new Synapse() ;
                                                       newNeuron->outputSynapses[ i ]->weight = normalRandom( 1.0 / sqrt( 2.0 ) ) ;
                                                       newNeuron->outputSynapses[ i ]->tempNeuronLevel = openLevel - 1;
                                                       newNeuron->outputSynapses[ i ]->tempNeuronNumber = i;

                                                       // newNeuron->synapses[ i ].weight = drand48() ;
                                                       // newNeuron->outputSynapses[ i ]->weight = -1 + 2 * drand48() ;
                                                       newNeuron->outputSynapses[ i ]->srcNeuron = newNeuron ;
                                                       newNeuron->outputSynapses[ i ]->destNeuron =
                                                               _levels[ openLevel - 1 ]->neurons[ i ] ;
                                               }
                                       }
                               }
                               else
                               {
                                       //WK Most of this block is modified
                                       char tmpChar;
                                       double tmpDbl2;
                                       int cnt = sscanf( str, "syn:%d %c%lf%lf%d%d", &tmpInt, &tmpChar, &tmpDbl, &tmpDbl2, &tmpInt2, &tmpInt3 ) ;
                                       if  ( ( cnt >= 2 )  && ( tmpInt < newNeuron->numOutputSynapses ) )
                                       {
 //cerr << "Setting up synapse " << tmpInt << " with weight " << tmpDbl << endl ;
 //cerr << "Setting up synapse " << tmpInt << " with type " << tmpChar << " with weights " << tmpDbl << " " << tmpDbl2 << endl ;
                                               switch (tmpChar) {
                                                       case 'f': //Fanin
                                                               //TmpDbl == not used, TmpDbl2 == not used
                                                               newNeuron->outputSynapses[ tmpInt ]->weight = 0 ;
                                                               break;
                                                       case 'g': //Gaussian
                                                               //TmpDbl == mean, TmpDbl2 == stddev
                                                               newNeuron->outputSynapses[ tmpInt ]->weight =
                                                               tmpDbl + normalRandom( tmpDbl2 ) ;
                                                               newNeuron->outputSynapses[ tmpInt ]->weightNeedsInit = 0; //WK
                                                               break;
                                                       case 'l': //Linear range
                                                               //TmpDbl == lower bound, TmpDbl2 == upper bound
                                                               newNeuron->outputSynapses[ tmpInt ]->weight =
                                                               (tmpDbl2-tmpDbl)*drand48() + tmpDbl;
                                                               newNeuron->outputSynapses[ tmpInt ]->weightNeedsInit = 0; //WK
                                                               break;
                                                       case 's': //Specified
                                                               //TmpDbl == value for weight, TmpDbl2 == not used
                                                               newNeuron->outputSynapses[ tmpInt ]->weight = tmpDbl;
                                                               newNeuron->outputSynapses[ tmpInt ]->weightNeedsInit = 0; //WK
                                                               break;
                                                       default:
                                                               cerr << "No synapse type specified! Defaulting to fanin\n";
                                                               newNeuron->outputSynapses[ tmpInt ]->weight = 0 ;
                                               }

                                               if  ( ( cnt == 6 )  &&
                                                        ( tmpInt2 < openLevel )  &&
                                                        ( tmpInt3 < _levels[ tmpInt2 ]->numNeurons ) )
                                               {
 //cerr << "Reconnecting synapse from " << newNeuron->outputSynapses[ tmpInt ]->destNeuron ;
                                                       newNeuron->outputSynapses[ tmpInt ]->destNeuron =
                                                               _levels[ tmpInt2 ]->neurons[ tmpInt3 ] ;
 //cerr << " to " << newNeuron->outputSynapses[ tmpInt ]->destNeuron << endl ;
                                               }
                                               //WK Here and below
                                               if  ( ( cnt == 6 )  &&
                                                        !(( tmpInt2 < openLevel )  &&
                                                        ( tmpInt3 < _levels[ tmpInt2 ]->numNeurons )) )
                                               {
 cerr << "WK Forward declaration of synapse " << newNeuron->outputSynapses[ tmpInt ]->destNeuron ;
                                                       //newNeuron->outputSynapses[ tmpInt ]->destNeuron =
                                                       //      _levels[ tmpInt2 ]->neurons[ tmpInt3 ] ;
                                                       newNeuron->outputSynapses[ tmpInt ]->tempNeuronLevel  = tmpInt2;
                                                       newNeuron->outputSynapses[ tmpInt ]->tempNeuronNumber = tmpInt3;
                                                       newNeuron->outputSynapses[ tmpInt ]->neuronNeedsInit  = 1;
 cerr << " to " << tmpInt2 << "," << tmpInt3 << endl ;
                                               } //END WK
                                       }
                                       else cerr << "Warning: can't parse string \"" << str << "\"" << endl ;
                               }

                       }
                       // else comment
               }
               //WK Here and below
    //WK When we're done, clean up all the forward declarations
               //WK Calculate fanin at each point, hook up predeclared synapses
    for (i = 0; i < _numLevels; i++) {
      for (int j = 0; j < _levels[i]->numNeurons; j++) {
       for (int k = 0; k < _levels[i]->neurons[j]->numOutputSynapses; k++) {
         if (_levels[i]->neurons[j]->outputSynapses[k]->neuronNeedsInit) { //Case where we specifcally said which neurals we'd connect to
                                               Synapse *foo = _levels[i]->neurons[j]->outputSynapses[k];
                                               int l = foo->tempNeuronLevel;
                                               int m = foo->tempNeuronNumber;
                                               if (l >= _numLevels || m >= _levels[i]->numNeurons)
                                                       cerr << "Out of bounds number too high in forward declaration of synapse!\n";
                                               else
                                                       foo->destNeuron = _levels[l]->neurons[m];
         }
                                       //Adding to fanin
                                       if (i > 0)
                                               _levels[i]->neurons[j]->outputSynapses[k]->destNeuron->numInputSynapses++;
       }
      }
               }
               //WK Then set weights based on the fanin we calculated in the last pass
    for (i = 1; i < _numLevels; i++) {
      for (int j = 0; j < _levels[i]->numNeurons; j++) {
       for (int k = 0; k < _levels[i]->neurons[j]->numOutputSynapses; k++) {
                                               //cerr << "WK Loop2: " << i << " " << j << " " << k << endl;
                                       if (_levels[i]->neurons[j]->outputSynapses[k]->weightNeedsInit) {
                                               double nIS = _levels[i]->neurons[j]->outputSynapses[k]->destNeuron->numInputSynapses;
                                               //cerr << "WK Setting random weight with fanin of " << nIS << endl;
                                               _levels[i]->neurons[j]->outputSynapses[k]->weight = normalRandom( 1.0 / sqrt( nIS ) ) ;
               }
       }
      }
               } //END WK
		fclose( fid ) ;
       }
}


void NeuralNet::AssignInputs( double input[] )
{
       int i, j, k ;
       int inputLevel = _numLevels - 1 ;

       // Assign the new given inputs
       for( i = 0, k = 0 ;   i < _levels[ inputLevel ]->numNeurons ;   i++ )
               if  ( _levels[ inputLevel ]->neurons[ i ]->acceptsInput() )
			   {
                       _levels[ inputLevel ]->neurons[ i ]->valueInput = input[ k++ ] ;
// cerr << "  " << _levels[ inputLevel ]->neurons[ i ]->valueInput ;
			   }
// cerr << endl ;

       // Clear all other levels for future data propagation
       for( j = 0 ;   j < _numLevels - 1 ;   j++ )
               for( i = 0 ;   i < _levels[ j ]->numNeurons ;   i++ )
                       if  ( _levels[ j ]->neurons[ i ]->acceptsInput() )
                               _levels[ j ]->neurons[ i ]->valueInput = 0 ;
}


void NeuralNet::FeedForward( void )
{
       int i, j, k ;

       // For the input and all hidden layers
       for( i = _numLevels - 1 ;   i > 0 ;   i-- )
       {
               NeuralNetLevel * level = _levels[ i ] ;

               // Traverse each of the neurons on this level
               for( j = 0 ;   j < level->numNeurons ;   j++ )
               {
                       Neuron * neuron = level->neurons[ j ] ;

                       neuron->valueOutput = neuron->squashingFunction( neuron->valueInput ) ;

                       // Traverse each of the synapses from the current neuron
                       for( k = 0 ;   k < neuron->numOutputSynapses ;   k++ )
                               neuron->outputSynapses[ k ]->destNeuron->valueInput +=
                                       neuron->valueOutput * neuron->outputSynapses[ k ]->weight ;
               }
       }

       NeuralNetLevel * level = _levels[ 0 ] ;
       for( j = 0 ;   j < level->numNeurons ;   j++ )
       {
               Neuron * neuron = level->neurons[ j ] ;
               neuron->valueOutput = neuron->squashingFunction( neuron->valueInput ) ;
// cerr << "squash( " << neuron->valueInput << ") ==> " << neuron->valueOutput << endl ;
       }
}


double NeuralNet::ComputeOutputError( double * trainingValues )
{
	double trainingError ;
	double CETrainingError = 0.0 ;
	double sumTrainingError = 0.0 ;

       NeuralNetLevel * level = _levels[ 0 ] ;

       // Compute out the output errors first.
       for( int j = 0 ;   j < level->numNeurons ;   j++ )
       {
               trainingError = level->neurons[ j ]->valueOutput -  trainingValues[ j ] ;
// cerr << "TE = " << level->neurons[ j ]->valueOutput << " - " << trainingValues[ j ] << " ==> " << trainingError << endl ;
// cerr << *this << endl ;


		 // Give a cost matrix scaling to good data so they count more
		 if  ( trainingValues[ j ] < 0.125 )
		 	trainingError *= cmdLineArgs.costMatrixItem ;

               if  ( CmdLineArgs::ESF_AVG == cmdLineArgs.errorStyleFlag )
		 {
                     sumTrainingError += trainingError * trainingError ;
               	level->neurons[ j ]->valueError = trainingError *
                       level->neurons[ j ]->squashingSlope( level->neurons[ j ]->valueInput ) ;
		 }
		 else if  ( CmdLineArgs::ESF_CROSS_ENTROPY_BB == cmdLineArgs.errorStyleFlag )
		 {
			// Implement eqn 6.120 from pg 231 of Bishop
			// messed up by Bob
/*
			const double epsilon = 0.00000001 ;
			if  ( ( -epsilon < trainingError )  &&  ( trainingError < epsilon ) )
				CETrainingError = 0.0 ;
			else if  ( trainingValues[ j ] <= 0.125 )
				CETrainingError = -log( 1.0 - level->neurons[ j ]->valueOutput ) ;
			else if  ( trainingValues[ j ] >= 0.875 )
				CETrainingError = -log( level->neurons[ j ]->valueOutput ) ;
			else CETrainingError =
				-( trainingValues[ j ] * log( level->neurons[ j ]->valueOutput ) +
				   (1.0 - trainingValues[ j ]) * log( (1.0 - level->neurons[ j ]->valueOutput) ) ) ;
*/
               	level->neurons[ j ]->valueError = trainingError
					* level->neurons[ j ]->squashingSlope( level->neurons[ j ]->valueInput )
					;

			sumTrainingError += fabs( trainingError ) ;

if  ( ( trainingError < -5 )  ||  ( 5 < trainingError ) )
{
  cerr << "slope( " << level->neurons[ j ]->valueInput << " ) --> " << level->neurons[ j ]->squashingSlope( level->neurons[ j ]->valueInput ) << endl ;
  cerr << "Sum += " << trainingError << ": " << level->neurons[ j ]->valueError << " --> " << sumTrainingError << endl ;
  cerr << *this << endl << endl ;
  sleep( 5 ) ;
}
		 }
		 else if  ( CmdLineArgs::ESF_CROSS_ENTROPY_BOOK == cmdLineArgs.errorStyleFlag )
		 {
			// Implement eqn 6.120 from pg 231 of Bishop
			level->neurons[ j ]->valueError = trainingError ;
			sumTrainingError += fabs( trainingError ) ;
		 }
		 else if  ( CmdLineArgs::ESF_RMS == cmdLineArgs.errorStyleFlag )
		 {
               	level->neurons[ j ]->valueError = trainingError * trainingError *
                       level->neurons[ j ]->squashingSlope( level->neurons[ j ]->valueInput ) ;
                     sumTrainingError += fabs( level->neurons[ j ]->valueError ) ;
		 }
		 else
		 {
			fprintf( stderr, "Failure identifying error style flag %d\n",
				cmdLineArgs.errorStyleFlag ) ;
			assert( 0 ) ;
		 }


if  ( BBDebug )
fprintf( stderr, "Observed: %.10lf   Expected: %.05lf   Error: %8.5lf, %8.5lf, %8.5lf, %8.5lf;   %8.5lf, %8.5lf %8.5lf\n",
	level->neurons[ j ]->valueOutput,
	trainingValues[ j ],
	trainingError,
	trainingError * trainingError,
	CETrainingError,
	sumTrainingError,
	level->neurons[ j ]->valueInput,
	level->neurons[ j ]->squashingSlope( level->neurons[ j ]->valueInput ),
	level->neurons[ j ]->valueError ) ;

       }

       return( sumTrainingError ) ;
}


void NeuralNet::BackPropagate( double learningRate )
{
       int i, j, k ;

       // Figure out the hidden level errors.
       for( i = 1 ;   i < ( _numLevels - 1 ) ;   i++ )
       {
               NeuralNetLevel * level = _levels[ i ] ;

               for( j = 0 ;   j < level->numNeurons ;   j++ )
               {
                       Neuron * neuron = level->neurons[ j ] ;

                       neuron->valueError = 0 ;

                       // Traverse each of the synapses from the current neuron
                       for( k = 0 ;   k < neuron->numOutputSynapses ;   k++ )
                       {
                               neuron->valueError +=
                                       neuron->outputSynapses[ k ]->destNeuron->valueError *
                                       neuron->outputSynapses[ k ]->weight ;
                       }

                       neuron->valueError *= neuron->squashingSlope( neuron->valueInput ) ;
               }
       }

       // Correct the weights based on the hidden and output level errors.
       for( i = 1 ;   i < _numLevels ;   i++ )
       {
               NeuralNetLevel * level = _levels[ i ] ;

               for( j = 0 ;   j < level->numNeurons ;   j++ )
               {
                       Neuron * neuron = level->neurons[ j ] ;

                       // Update each synapses weight from the current neuron
                       for( k = 0 ;   k < neuron->numOutputSynapses ;   k++ )
                       {
                               neuron->outputSynapses[ k ]->deltaWeight *= cmdLineArgs.momentumRate ;
                               neuron->outputSynapses[ k ]->deltaWeight +=
                                       learningRate * neuron->valueOutput *
                                       neuron->outputSynapses[ k ]->destNeuron->valueError ;
                               neuron->outputSynapses[ k ]->weight -=
                                       neuron->outputSynapses[ k ]->deltaWeight ;
                       }
               }
       }
}



ostream & operator<<( ostream & os, NeuralNetLevel & netLevel )
{
       os << netLevel.name << ": " << endl ;
       for( int j = 0 ;   j < netLevel.numNeurons ;   j++ )
               os << *netLevel.neurons[ j ] << endl ;

       return( os ) ;
}


ostream & operator<<( ostream & os, NeuralNet & net )
{
       for( int i = 0 ;   i < net._numLevels ;   i++ )
               os << *net._levels[ i ] ;

       return( os ) ;
}


