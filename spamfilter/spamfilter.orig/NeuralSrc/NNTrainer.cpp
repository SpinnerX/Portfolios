//NeuralNet Trainer by Bob Boyer & William Kerney
//Nov 8, 2000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "CmdLineArgs.hpp"
#include "CMGroup.hpp"
#include "NeuralNet.hpp"
#include "NNTrainer.hpp"

#include "DataLoader.hpp"

extern CmdLineArgs cmdLineArgs ;

extern int BBDebug ;
extern int mainRestart ;


void ValidateData(
	NeuralNet  & network,
	DataLoader & loader,
	CMGroup    & cmGroup,
	double       avgValidationSignalFactor )
{
	double sumError = 0.0 ;

	int i ;
	double err ;

	for( i = 0 ;   i < loader.numSamples ;   i++ )
	{
		network.AssignInputs( loader.inputs[ i ] ) ;
		network.FeedForward() ;

		if  ( loader.outputs[ i ] != NULL )
		{
			err = network.ComputeOutputError( loader.outputs[ i ] ) ;
			sumError += err ;

			double * results = new double[ loader.outputDimensions ] ;
			for( int odIndex = 0 ;   odIndex < loader.outputDimensions ;   odIndex++ )
			{
				results[ odIndex ] = network.Level( 0 )->neurons[ odIndex ]->valueOutput ;
			}

			int * state = cmGroup.AddResult( loader.outputs[ i ], results ) ;

			delete [] state ;
			delete [] results ;
		}

	}

	cerr
		<< " VErr " << setw( 10 ) << setprecision( 7 ) << (sumError * avgValidationSignalFactor)
		<< " VgPg " << setw( 5 ) << cmGroup.cmsCum.truePositive()
		<< " VgPs " << setw( 5 ) << cmGroup.cmsCum.falseNegative()
		<< " VsPg " << setw( 5 ) << cmGroup.cmsCum.falsePositive()
		<< " VsPs " << setw( 5 ) << cmGroup.cmsCum.trueNegative() ;
}




//WK -- BITVECTOR MACROS
#define BITSET(i,base) (base[i>>3]|=(1<<(i%8)))
#define BITCLR(i,base) (base[i>>3]&= ~(1<<(i%8)))
#define BITTST(i,base) (base[i>>3]&(1<<(i%8)))

IndexGroup::IndexGroup( void ) :
          numAllocated( 0 ),
          indexes( NULL ),
          numIndexes( 0 ),
          indexPosn( 0 )
{
}

IndexGroup::~IndexGroup( void )
{
          if  ( NULL != indexes )
                          delete [] indexes ;
}

void IndexGroup::Allocate( int qty, int assign )
{
          numAllocated = qty ;
          indexes = new int[ numAllocated ] ;
          if  ( assign )
          {
                          numIndexes = numAllocated ;
                          for( int i = 0 ;   i < numIndexes ;   i++ )
                                          indexes[ i ] = i ;
          }
          // else numIndexes is still 0 and will be set elsewhere
          // (e.g., by the NNTrainer's AllocateIndexes()
}

void IndexGroup::Shuffle( void )
{
          int i, j, tmp ;

          // Handle special case of only two indexes by only swapping once
          // Because we may have only two groups each with two indexes,
          // make the choice to do the special swap random
          if  ( ( 2 == numIndexes )  &&  ( 0.5 < drand48() ) )
          {
                          tmp = indexes[ 0 ] ;
                          indexes[ 0 ] = indexes[ 1 ] ;
                          indexes[ 1 ] = tmp ;
          }
          else for( i = 0 ;   i < numIndexes ;   i++ )
          {
                          j = int( i + 1 + (numIndexes - 1) * drand48() ) % numIndexes ;
                          tmp = indexes[i] ;
                          indexes[ i ] = indexes[ j ] ;
                          indexes[ j ] = tmp ;
          }

          indexPosn = 0 ;
}

int IndexGroup::NextIndex( void )
{
          if  ( indexPosn >= numIndexes )
                          return( -1 ) ;
          else return( indexes[ indexPosn++ ] ) ;
}

int IndexGroup::AvailableIndexes( void )
{
          if  ( indexPosn >= numIndexes )
                          return( 0 ) ;
          else return( numIndexes - indexPosn ) ;
}

//WK For conjugate gradient descent
void incLoop(char *currd, int numInputDimensions) {
       for (int i = 0; i < numInputDimensions; i++) {
               if (BITTST(i,currd) == 0) {
                       BITSET(i,currd);
                       return;
               }
               BITCLR(i,currd);
       }
}

//WK For conjugate gradient descent
int testFinished(char *currd, int numInputDimensions) {
       for (int i = 0; i < numInputDimensions; i++)
               if (BITTST(i,currd) == 0) return 0;
       return 1;
}

NNTrainer::NNTrainer( void ) :
          groups( NULL ),
          numGroups( 0 ),
          groupPosn( 0 ),
          groupAmount( 0 ),
          groupRatio( 0 ),
          groupLarger( 0 ),
          epoch( 0 ),
          epochError( 0.0 ),
          errorsUp ( 1 ), //WK
          lastError ( 100000 ) //WK Should be maxint
{
}


NNTrainer::~NNTrainer( void )
{
          ReleaseOldIndexes() ;
}


void NNTrainer::AllocateIndexes( int numIndexes )
{
          numGroups = 1 ;
          groups = new IndexGroup[ 1 ] ;
          groups[ 0 ].Allocate( numIndexes, 1 ) ;
}


void NNTrainer::AllocateIndexes(
          int     numSamples,
          int     numDimensions,
          double ** samples,
          int     numClassifications )
{
          if  ( ( numDimensions == 1 )  &&  ( numClassifications == 2 ) )
          {
                          // the samples will be booleans
                          numGroups = 2 ;
                          groups = new IndexGroup[ 2 ] ;
                          groups[ 0 ].Allocate( numSamples, 0 ) ;
                          groups[ 1 ].Allocate( numSamples, 0 ) ;

                          // now need to examine the samples (assumes +/- 1)
                          for( int i = 0 ;   i < numSamples ;   i++ )
                          {
                                          if  ( (samples[i])[0] > 0 )
                                                          groups[1].indexes[ groups[1].numIndexes++ ] = i ;
                                          else groups[0].indexes[ groups[0].numIndexes++ ] = i ;
                          }

#if 0
// temp debug & validation code
for( i = 0 ;   i < numClassifications ;   i++ )
{
          cerr << "Output group " << i << endl ;
          for( int j = 0 ;   j < groups[ i ].numIndexes ;   j++ )
                          cerr << j << " sample " << groups[ i ].indexes[ j ] << endl ;
}
#endif
          }
          else if  ( numDimensions == numClassifications )
          {
                          int i, j ;

                          // the samples are exclusive classifications
                          numGroups = numClassifications ;
                          groups = new IndexGroup[ numClassifications ] ;
                          for( i = 0 ;   i < numClassifications ;   i++ )
                                          groups[ 0 ].Allocate( numSamples, 0 ) ;

                          // now need to examine the samples
                          for( i = 0 ;   i < numSamples ;   i++ )
                                          for( j = 0 ;   j < numClassifications ;   j++ )
                                                          if  ( (samples[i])[j] > 0 )
                                                                          groups[j].indexes[ groups[j].numIndexes++ ] = i ;

// temp debug & validation code
for( i = 0 ;   i < numClassifications ;   i++ )
{
          cerr << "Output group " << i << endl ;
          for( j = 0 ;   j < groups[ i ].numIndexes ;   j++ )
                          cerr << j << " sample " << groups[ i ].indexes[ j ] << endl ;
}

          }
          else
          {
                          // we really should have used the other allocation
                          // because we don't know how to break down these
                          // samples.

                          AllocateIndexes( numSamples ) ;
          }
}


void NNTrainer::ReleaseOldIndexes( void )
{
          delete [] groups ;
          numGroups = 0 ;
          groups = NULL ;
}


void NNTrainer::Shuffle( void )
{
          int i ;
          int cnt = 0 ;

          for( i = 0 ;   i < numGroups ;   i++ )
          {
                          groups[ i ].Shuffle() ;
                          cnt += groups[ i ].numIndexes ;
          }

          cnt = int( cnt * drand48() ) ;
          groupPosn = 0 ;
          while( ( groupPosn < ( numGroups - 1 ) )  &&
                                 ( cnt > groups[ groupPosn ].numIndexes ) )
          {
                          cnt -= groups[ groupPosn ].numIndexes ;
                          groupPosn++ ;
          }

          groupAmount = 0 ;
          for( i = 0 ;   i < numGroups ;   i++ )
          {
                          if  ( i != groupPosn )
                                          groupAmount += groups[ i ].numIndexes ;
          }

          if  ( numGroups == 2 )
          {
                          if  ( groups[ 0 ].numIndexes >= groups[ 1 ].numIndexes )
                                          groupLarger = 0 ;
                          else groupLarger = 1 ;

                          groupRatio =
                                          double( groups[ groupLarger ].numIndexes ) /
                                          double( groups[ (1 - groupLarger) ].numIndexes ) ;
          }
}


int NNTrainer::NextIndex()
{
          int retval ;

          if  ( numGroups > 2 )
          {
                          int tmp = int( groupAmount * drand48() ) ;
                          int oldPosn = groupPosn ;
                          groupPosn = ( (groupPosn+1) % numGroups ) ;

                          int done = 0 ;
                          while( !done )
                          {
                                          if  ( groups[ groupPosn ].AvailableIndexes() > 0.5 * groupAmount )
                                                          done = 1 ;
                                          else if  ( tmp >= groups[ groupPosn ].AvailableIndexes() )
                                          {
                                                          tmp -= groups[ groupPosn ].AvailableIndexes() ;
                                                          groupPosn = ( (groupPosn+1) % numGroups ) ;
                                          }
                                          else done = 1 ;
                          }
                          groupAmount += groups[ oldPosn ].AvailableIndexes() ;
                          groupAmount -= groups[ groupPosn ].AvailableIndexes() ;

                          retval = groups[ groupPosn ].NextIndex() ;
          }
          else if  ( numGroups > 1 )
          {
                          if  ( groups[ (1 - groupLarger) ].AvailableIndexes() < 1 )
                                          retval = groups[ groupLarger ].NextIndex() ;
                          else
                          {
                                          double currRatio =
                                                          double( groups[ groupLarger ].AvailableIndexes() ) /
                                                          double( groups[ (1 - groupLarger) ].AvailableIndexes() ) ;
                                          if  ( currRatio >= groupRatio )
                                                          retval = groups[ groupLarger ].NextIndex() ;
                                          else retval = groups[ (1 - groupLarger) ].NextIndex() ;
                          }
          }
          else
          {
                          retval = groups[ 0 ].NextIndex() ;
          }

// cerr << "Next index returning " << retval << endl ;

          return( retval ) ;
}



int NNTrainer::Epoch( void )
{
          return( epoch ) ;
}


void NNTrainer::StartEpoch( void )
{
          // re-shuffle input order
          Shuffle() ;

          // reset error counters
          epochError = 0.0 ;

	  tp = fp = tn = fn = 0 ;
	  qtp = qfp = qtn = qfn = 0 ;

	  // gettimeofday( &epochStartTime, NULL ) ;
}


double NNTrainer::EpochError( void )
{
          return( epochError ) ;
}




void NNTrainer::TrainStochasticGroup(
          NeuralNet & network,
          int           numTrainingPatterns,
          int           numInputDimensions,
          double         * trainingInputs[],
          int           numOutputDimensions,
          double         * trainingOutputs[],
          double         trainingOutputMiddles[],
          double         acceptableAvgError,
          int           maxTrainingEpochs,
          int           hints,
		  double        avgTrainSignalFactor )
{
          if  ( hints & NN_HINT_CLASSIFIER )
          {
                          AllocateIndexes(
                                          numTrainingPatterns,
                                          numOutputDimensions,
                                          trainingOutputs,
                                          numOutputDimensions == 1 ?
                                                          2 : numOutputDimensions ) ;
          }
          else
          {
                          AllocateIndexes( numTrainingPatterns ) ;
          }



          int i, j ;

	CMGroup cmGroup ;
	cmGroup.Allocate( cmdLineArgs.spamConfidence, numOutputDimensions, trainingOutputMiddles ) ;


          double learningRate = cmdLineArgs.initialLearningRate ;
	double errorDivisor = avgTrainSignalFactor ;

                               //WK
                               errorsUp = 1;
                               lastError = 100000;


gettimeofday( &epochStartTime, NULL ) ;



          epoch = 0;
          epochError = acceptableAvgError + 2 * numOutputDimensions ;
          lastError = epochError ;

       while(
               ( epoch++ < maxTrainingEpochs )  &&
               ( epochError > acceptableAvgError ) )
       {
		cmGroup.Reset() ;
       switch( cmdLineArgs.learningRateStyle )
       {
               case CmdLineArgs::LRS_EPOCH_ERR_A :             // Bob's because it was working
                       if  ( epochError > 0.75 )
                               learningRate = 0.45 ;
                       else if  ( epochError > 0.45 )
                               learningRate = 0.75 * epochError ;
                       else if  ( epochError > 0.2 )
                               learningRate = 0.65 * epochError ;
                       else if  ( epochError > 0.1 )
                               learningRate = 0.60 * epochError ;
                       else if  ( epochError > 0.01 )
                               learningRate = 0.50 * epochError ;
                       // else learningRate = 1.0 / ( 1.0 + log( epoch ) ) ;
                       else learningRate = 1.0 / double( epoch ) ;
                       break ;
               case CmdLineArgs::LRS_EPOCH_ERR_B :             // Bob's attempt at a formula
                       if  ( epochError > 1.0 )
                               learningRate = 0.5 ;
                       else learningRate = 0.5 * epochError / ( 1.0 + log( 1.0 / double( epochError ) ) ) ;
                       break ;
               case CmdLineArgs::LRS_DECAY_LOG_EPOCH :         // Bob's slower than 'standard'
					   if  ( epoch > 1 )
                       	learningRate *= ( 1.0 + log( epoch - 1 ) ) / ( 1.0 + log( epoch ) ) ;
                       break ;
               case CmdLineArgs::LRS_DECAY_INV_EPOCH :    // 'standard'
                       learningRate = 1.0 / double( epoch ) ;
                       break ;
               case CmdLineArgs::LRS_ANNEALING :  //Bill's hacked attempt at annealing
                       if  ( epoch > 1 )
					   {
                       		learningRate *= ( 1.0 + log( epoch - 1 ) ) / ( 1.0 + log( epoch ) ) ;
					   		if  ( lastError < epochError )
							{
                       			// cerr << "\t\tlE: " << lastError << " eE: " << epochError << endl;
                       			learningRate *= cmdLineArgs.annealingRate ;
							}
					   }
                       lastError = epochError;
                       // learningRate = 1.0 / ( 1.0 + log( epoch ) ) ;  // use slow stuff as basis
                       // learningRate *= pow( cmdLineArgs.annealingRate, errorsUp ) ;
                       //cerr << "LR: " << learningRate << " eU: " << errorsUp << endl;
                       break ;
               case CmdLineArgs::LRS_FIXED :      // fixed, never change the learning rate
               default:
                       break ;
       }


       StartEpoch() ;

       i = 0 ;
       double tmpErr ;
       while( 0 <= ( i = NextIndex() ) )
       {
               if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_PATTERN )
               {
          cerr << "Data set " << i << endl ;

          int k ;
          for( k = 0 ;   k < numInputDimensions ;   k++ )
                          cerr << "  " << (trainingInputs[ i ])[ k ] ;
          cerr << " --->" ;
          for( k = 0 ;   k < numOutputDimensions ;   k++ )
                          cerr << "  " << (trainingOutputs[ i ])[ k ] ;
          cerr << endl ;
               }

               //WK Conjugate Gradient Descent Testing
               //XXXX
               if (cmdLineArgs.cgd) {
                       int chars_used = 1+numInputDimensions/8;
                       double bestError = 1000000;
                       char *bestd = new char[chars_used];
                       char *currd = new char[chars_used];
                       if (sizeof(char) != 1) cerr << "Charsize error!\n";
                       memset(bestd,0xFF,chars_used); //Start loop at -1, since it incs at start
                       memset(currd,0xFF,chars_used); //Start loop at -1, since it incs at start

                       int inputLevel = network.NumLevels() - 1 ;
                       NeuralNetLevel *il = network.Level(inputLevel);
                       int finished = 0;
                       int count = 0;
                       do {
                               incLoop(currd,numInputDimensions);
                               // Assign the new given inputs
                               network.AssignInputs( trainingInputs[ i ] ) ;

                               int count = numInputDimensions;
                               for (int bit = 0; bit < count; bit++) {
                                       if (!il->neurons[bit]->acceptsInput()) {
                                               count++;
                                               continue;
                                       }
                                       if (BITTST(bit,currd) == 0)
                                               il->neurons[ bit ]->valueInput -= cmdLineArgs.cgd;
                                       else
                                               il->neurons[ bit ]->valueInput += cmdLineArgs.cgd;
                               }
                               network.FeedForward();
                               double cgdError = network.ComputeOutputError( trainingOutputs[ i ] ) ;
                               if (cgdError < bestError) {
                                       bestError = cgdError;
                                       for (int j = 0; j < sizeof(currd); j++)
                                               bestd[j] = currd[j];
                               }
                               finished = testFinished(currd,numInputDimensions);
                       } while (!finished);
                       //cerr << "BE: " << bestError << endl;
                       /*
                       for (int f = 0; f < numInputDimensions; f++) {
                               if (BITTST(f,bestd))
                                       cout << '1';
                               else
                                       cout << '0';
                       }
                       cout << endl;
                       */
               }
               //END WK


               network.AssignInputs( trainingInputs[ i ] ) ;

               if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_ASSIGN )
          cerr << "Assigned: " << network << endl ;

               network.FeedForward() ;

               if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_FEEDFWD )
          cerr << "FeedFwd: " << network << endl ;

// BBDebug = 1 ;

               tmpErr = network.ComputeOutputError( trainingOutputs[ i ] ) ;

// cerr << "\t\t\t\ttemp error = " << tmpErr << " given expected = " << (trainingOutputs[ i ])[0] << endl ;

		// spam detector...
		double * outputs = new double[ numOutputDimensions ] ;
		for( int odIndex = 0 ;   odIndex < numOutputDimensions ;   odIndex++ )
			outputs[ odIndex ] = network.Level( 0 )->neurons[ odIndex ]->valueOutput ;
		cmGroup.AddResult( trainingOutputs[ i ], outputs ) ;

               if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_CALCERR )
               {
                       int k ;
                       for( k = 0 ;   k < numInputDimensions ;   k++ )
                               cerr << "  " << (trainingInputs[ i ])[ k ] ;
                       cerr << " --->" ;
                       for( k = 0 ;   k < numOutputDimensions ;   k++ )
                               cerr << "  " << (trainingOutputs[ i ])[ k ] ;
                       cerr << " vs " ;
                       for( k = 0 ;   k < numOutputDimensions ;   k++ )
                               cerr << "  " << network.Level( 0 )->neurons[ k ]->valueOutput ;
          cerr << " has error " << tmpErr << endl ;
               }

// cerr << "Epoch error = " << epochError << " += tmpErr = " << tmpErr << endl ;

               epochError += tmpErr ;

               network.BackPropagate( learningRate ) ;
               /*
               while (direction) {
                       if (direction % 2) cout << "1";
                       else cout << "0";
                       direction = direction>>1;
               }
               cout << " - Bill BP result done\n";
               */

               if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_BACKPROP )
                       cerr << "BackProp: " << network << endl ;

               if  ( ( cmdLineArgs.debug & CmdLineArgs::DEBUG_EPOCH_NET ) &&  !(epoch % 1000) )
               {
                       int k ;
                       cerr << endl << "   Epoch is " << epoch
                               << "  error is " << epochError
                               << "  learning rate is " << learningRate << endl ;
                       for( k = 0 ;   k < numInputDimensions ;   k++ )
                               cerr << "  " << (trainingInputs[ i ])[ k ] ;
                       cerr << " --->" ;
                       for( k = 0 ;   k < numOutputDimensions ;   k++ )
                               cerr << "  " << (trainingOutputs[ i ])[ k ] ;
                       cerr << endl << network << endl ;
               }
       }

// cerr << "Epoch error = " << epochError << " * divisor = " << errorDivisor << endl ;

       epochError *= errorDivisor ;

// BB temporary....
// cerr << network << endl ;


if  ( !(epoch % 100) )
{
		struct timeval epochEndTime ;
		gettimeofday( &epochEndTime, NULL ) ;

		int sec = epochEndTime.tv_sec - epochStartTime.tv_sec ;
		int usec = epochEndTime.tv_usec - epochStartTime.tv_usec ;
		if  ( usec < 0 )
		{
			sec-- ;
			usec += 1000000 ;
		}

		cerr << endl << "Epoch time cost was: " << sec << ":" << setw( 6 ) << setfill( '0' ) << usec << setfill( ' ' ) << endl ;

		cerr << cmGroup << endl ;

		gettimeofday( &epochStartTime, NULL ) ;
}

       if  ( ( cmdLineArgs.debug & CmdLineArgs::DEBUG_INTERVAL ) &&  !(epoch % 100) )
               cerr << "Epoch: " << epoch << " has error " << epochError
               << " with learning rate " << learningRate << endl ;

		if  ( ( cmdLineArgs.reportingRate > 0 )  &&
			  ( strlen( cmdLineArgs.saveNetworkFileName ) > 0 )  &&
			  !( epoch % cmdLineArgs.reportingRate ) )
		{
			char * nameStr = new char[ strlen( cmdLineArgs.saveNetworkFileName ) + 4 + 12 ] ;
			sprintf( nameStr, "%s.%d.%d", cmdLineArgs.saveNetworkFileName, mainRestart, epoch ) ;
			network.Save( nameStr ) ;
			delete [] nameStr ;
		}


	   if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_EPOCH_STATS )
	   {
               cerr
			<< "Epoch " << setw( 5 ) << epoch
                     << " error " << setw( 10 ) << setprecision( 7 ) << epochError
                     << " learning rate " << setw( 10 ) << setprecision( 7 ) << learningRate
			<< " gPg " << setw( 5 ) << cmGroup.cmsCum.truePositive()
			<< " gPs " << setw( 5 ) << cmGroup.cmsCum.falseNegative()
			<< " sPg " << setw( 5 ) << cmGroup.cmsCum.falsePositive()
			<< " sPs " << setw( 5 ) << cmGroup.cmsCum.trueNegative() ;

		 static int        validationPrepReqd = 1 ;
		 static int        validationOk = 0 ;
		 static double     avgValidationSignalFactor = 0.0 ;
		 static DataLoader validationLoader ;
		 static CMGroup    validationCMGroup ;

		 if  ( validationPrepReqd )
		 {
			validationPrepReqd = 0 ;
			if  ( ( 0 < strlen( cmdLineArgs.validateFileName ) )  &&
		               ( 0 == validationLoader.Load( cmdLineArgs.validateFileName ) ) )
			{
/*
cerr << validationLoader.numSamples << ", "
       << validationLoader.inputDimensions << ", "
       << validationLoader.inputs << ", "
       << validationLoader.outputDimensions << ", "
       << validationLoader.outputs << ", "
       << validationLoader.outputMiddles << ", "
       << endl ;
*/

				if  ( validationLoader.outputDimensions > 0 )
				{
				  validationLoader.Whiten( cmdLineArgs.whitenFlags, 1 ) ;

				  avgValidationSignalFactor = validationLoader.AverageTrainingSignal(
					CmdLineArgs::ESF_AVG == cmdLineArgs.errorStyleFlag ) ;

				  validationCMGroup.Allocate(
					cmdLineArgs.spamConfidence,
					validationLoader.outputDimensions,
					validationLoader.outputMiddles ) ;

				  validationOk = 1 ;
				}
				// else validationOk still 0
			}
			// else validationOk still 0
		 }

		 if  ( validationOk )
		 {
			validationCMGroup.Reset() ;
		 	ValidateData( network, validationLoader, validationCMGroup, avgValidationSignalFactor ) ;
		 }

               cerr
			<< endl ;
	   }

       }
       epoch--; //Account for while loop increment
}
