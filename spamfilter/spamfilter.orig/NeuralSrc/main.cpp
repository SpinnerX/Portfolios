
#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#include "CmdLineArgs.hpp"
#include "CMGroup.hpp"
#include "DataLoader.hpp"
#include "NNTrainer.hpp"


int BBDebug = 0 ;
int mainRestart = 0 ;

CmdLineArgs cmdLineArgs ;


char * basename( char * str )
{
	int posn = strlen( str ) ;
	while( posn >= 0 )
		if  ( str[ posn ] == '/' )
			return( &str[ posn + 1 ] ) ;
		else posn-- ;

	return( str ) ;
}


int runTestPatterns(
       NeuralNet  & network,
       DataLoader & loader,
       char       * filename,
       int          debugInfo,
	double       avgTrainSignalFactor,
	int          dataBreakdown )
{
	int finalResult = 0 ;

	double sumError = 0.0 ;

	FILE * fids[4] ;

	static char * extensions[] = { "tp", "fn", "fp", "tn" } ;

	if  ( dataBreakdown )
	{
		char * tmpstr = new char[ strlen( filename ) + 8 ] ;

		for( int i = 0 ;   i < 4 ;   i++ )
		{
			sprintf( tmpstr, "%s.%s", basename( filename ), extensions[ i ] ) ;
			fids[ i ] = fopen( tmpstr, "w" ) ;
		}

		delete [] tmpstr ;
	}


	CMGroup cmGroup ;
	cmGroup.Allocate( cmdLineArgs.spamConfidence, loader.outputDimensions, loader.outputMiddles ) ;
	// int tp = 0, fp = 0, tn = 0, fn = 0, qtp = 0, qfp = 0, qtn = 0, qfn = 0 ;


       if  ( ( 0 < strlen( filename ) )  &&
               ( 0 == loader.Load( filename ) ) )
       {
               if  ( cmdLineArgs.whitenFlags )
                       loader.Whiten( cmdLineArgs.whitenFlags, 0 ) ;

               int i, j ;
                          double err ;

               for( i = 0 ;   i < loader.numSamples ;   i++ )
               {
                       /*
                       for( j = 0 ;   j < loader.inputDimensions ;   j++ )
                               cerr << "  " << (loader.inputs[ i ])[ j ] ;
                       cerr << endl ;
                       */

                       network.AssignInputs( loader.inputs[ i ] ) ;
                       if  ( ( cmdLineArgs.debug & debugInfo )  &&
                               ( cmdLineArgs.debug & CmdLineArgs::DEBUG_ASSIGN ) )
                       cerr << network << endl ;

                       network.FeedForward() ;
                       if  ( ( cmdLineArgs.debug & debugInfo )  &&
                               ( cmdLineArgs.debug & CmdLineArgs::DEBUG_FEEDFWD ) )
                       cerr << network << endl ;


/*
                       for( j = 0 ;   j < loader.inputDimensions ;   j++ )
                       {
                               cerr << setw( 9 ) << (loader.inputs[ i ])[ j ] << "   " ;
                       }
                       cerr << " --> " ;
                       for( j = 0 ;   j < loader.outputDimensions ;   j++ )
                       {
                               cerr << setw( 9 ) << network.Level( 0 )->neurons[ j ]->valueOutput << "   " ;
                       }
*/

// BBDebug = 1 ;
                       if  ( loader.outputs[ i ] != NULL )
                       {
                               err = network.ComputeOutputError( loader.outputs[ i ] ) ;
							   sumError += err ;
                       if  ( ( cmdLineArgs.debug & debugInfo )  &&
                               ( cmdLineArgs.debug & CmdLineArgs::DEBUG_CALCERR ) )
                       cerr << network << endl ;

/*
                               cerr << " vs " ;
                               for( j = 0 ;   j < loader.outputDimensions ;   j++ )
                               {
                                       cerr << setw( 9 ) << (loader.outputs[ i ])[ j ] << "   " ;
                               }
                                               cerr << " has error " << err ;
*/


double * results = new double[ network.Level( 0 )->numNeurons ] ;
for( int odIndex = 0 ;   odIndex < network.Level( 0 )->numNeurons ;   odIndex++ )
	results[ odIndex ] = network.Level( 0 )->neurons[ odIndex ]->valueOutput ;
int * state = cmGroup.AddResult( loader.outputs[ i ], results ) ;

if  ( dataBreakdown )
{
	for( j = 0 ;   j < loader.inputDimensions ;   j++ )
		fprintf( fids[ state[ 0 ] ], "%.3f ", (loader.inputs[ i ])[ j ] ) ;
	for( j = 0 ;   j < loader.outputDimensions ;   j++ )
		fprintf( fids[ state[ 0 ] ], "%.3f ", (loader.outputs[ i ])[ j ] ) ;
	fprintf( fids[ state[ 0 ] ], "\n" ) ;
}

if  ( results[ 0 ] > cmdLineArgs.spamConfidence )
	finalResult = 1 ;		// spam
else finalResult = 0 ;	// good

delete [] state ;
delete [] results ;
                       }

/*
                       cerr << endl ;
*/
               }

	   cerr << endl << "Test Pattern error: " << (sumError * avgTrainSignalFactor) << endl << endl ;

	cerr << cmGroup << endl ;


       }

if  ( dataBreakdown )
{
	for( int i = 0 ;   i < 4 ;   i++ )
		fclose( fids[ i ] ) ;
}


	return( finalResult ) ;
}


int main( int argc, char ** argv )
{
	int finalResult = 0 ;	// default is to call the mail good

       DataLoader loader ;


       cmdLineArgs.readArgs( argc, argv ) ;

       if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_ACTUAL_ARGS )
                       cmdLineArgs.actualUsage( stderr ) ;

       srand48( cmdLineArgs.seed ) ;

       NeuralNet network ;
       network.Load( cmdLineArgs.loadNetworkFileName ) ;
       if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_INITIAL_NET )
               cerr << network << endl ;

	double avgTrainSignalFactor = 1.0 ;

       if  ( 0 == loader.Load( cmdLineArgs.trainFileName ) )
       {
			if  ( strlen( cmdLineArgs.whitenParamLoadFileName ) )
			{
				loader.LoadWhite(
					cmdLineArgs.whitenFlags,
					1,
					cmdLineArgs.whitenParamLoadFileName ) ;
cerr << "loaded white params" << endl ;
                            loader.Whiten( cmdLineArgs.whitenFlags, 0 ) ;
cerr << "whitened" << endl ;
			}
                     else if  ( cmdLineArgs.whitenFlags )
			{
				loader.Whiten( cmdLineArgs.whitenFlags, 1 ) ;
cerr << "whitened" << endl ;
				if  ( strlen( cmdLineArgs.whitenParamSaveFileName ) )
				{
					loader.SaveWhite(
						cmdLineArgs.whitenFlags,
						cmdLineArgs.whitenParamSaveFileName ) ;
cerr << "saved white params" << endl ;
				}
			}

                       if  ( 0 < strlen( cmdLineArgs.whitenFileName ) )
                               loader.Save( cmdLineArgs.whitenFileName ) ;


               int done = 0 ;
               int restart = -1 ;

			   avgTrainSignalFactor = loader.AverageTrainingSignal(
				CmdLineArgs::ESF_AVG == cmdLineArgs.errorStyleFlag ) ;

               NNTrainer trainer ;

               while( !done  &&  ( restart++ < cmdLineArgs.maxRestarts ) )
               {
				mainRestart = restart ;

/*
cerr << loader.numSamples << ", "
       << loader.inputDimensions << ", "
       << loader.inputs << ", "
       << loader.outputDimensions << ", "
       << loader.outputs << ", "
       << loader.outputMiddles << ", "
       << endl ;
*/

                       trainer.TrainStochasticGroup(
                               network,
                               loader.numSamples,
                               loader.inputDimensions,
                               loader.inputs,
                               loader.outputDimensions,
                               loader.outputs,
                               loader.outputMiddles,
                               cmdLineArgs.acceptableError,
                               cmdLineArgs.maxEpochs,
                               cmdLineArgs.hints,
							   avgTrainSignalFactor ) ;

                       if  ( trainer.Epoch() >= cmdLineArgs.maxEpochs )
                       {
                               // really need a better way to reinit weights
                               // than reloading the network, but for now...
                               network.Load( cmdLineArgs.loadNetworkFileName ) ;
                               if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_INITIAL_NET )
                                       cerr << network << endl ;
                       }
                       else
                       {
                               // Note: this saves only on success
                               if  ( 0 < strlen( cmdLineArgs.saveNetworkFileName ) )
                                       network.Save( cmdLineArgs.saveNetworkFileName ) ;
                               done = 1 ;
                       }
               }

       if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_FINAL_STATS )
               cerr << "Function learned(" << done << ") at"
                               << " restart " << setw( 3 ) << restart
                               << " epoch " << setw( 5 ) << trainer.Epoch() << " >= " << cmdLineArgs.maxEpochs
                               << " or avg error " << trainer.EpochError() << " <= " << cmdLineArgs.acceptableError
                               << endl ;
                               // << " using current learning rate " << network.learningRate << endl ;

       if  ( cmdLineArgs.debug & CmdLineArgs::DEBUG_FINAL_NET )
                       cerr << endl << network << endl ;

       }

               // Perform validation if requested by filename getting specified
               runTestPatterns( network, loader, cmdLineArgs.validateFileName,
                       CmdLineArgs::DEBUG_VALIDATION, avgTrainSignalFactor,
			  cmdLineArgs.dataBreakdown ) ;
               finalResult = runTestPatterns( network, loader, cmdLineArgs.testFileName,
                       CmdLineArgs::DEBUG_TESTING, avgTrainSignalFactor,
			  0 ) ;

	cout << "Final Result: " << finalResult << endl ;

       return( finalResult ) ;
}
