#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "CmdLineArgs.hpp"


CmdLineArgs::CmdLineArgs( void ) :
       debug( 0 ),
       seed( time( 0 ) ),
       loadNetworkFileName( "Network.desc" ),
       saveNetworkFileName( "" ),
       trainFileName( "Network.train" ),
       whitenFileName( "" ),
       whitenParamSaveFileName( "" ),
       whitenParamLoadFileName( "" ),
       validateFileName( "Network.validate" ),
       testFileName( "Network.test" ),
       errorStyleFlag( ESF_RMS ),
       acceptableError( 0.1 ),
       maxEpochs( 10000 ),
       learningRateStyle( 5 ),
       initialLearningRate( 0.6 ),
       hints( 0 ),
       cgd( 0.0 ),
       whitenFlags( 3 ),
       maxRestarts( 3 )

       return( sum ) ;
}




void CmdLineArgs::actualUsage( FILE * fid )
{
       fprintf( fid, "\t-debug %x\n", debug ) ;
       fprintf( fid, "\t-seed %d\n", seed ) ;
       fprintf( fid, "\t-load \"%s\"\n", loadNetworkFileName ) ;
       fprintf( fid, "\t-save \"%s\"\n", saveNetworkFileName ) ;
       fprintf( fid, "\t-train \"%s\"\n", trainFileName ) ;
       fprintf( fid, "\t-whiten \"%s\"\n", whitenFileName ) ;
       fprintf( fid, "\t-validate \"%s\"\n", validateFileName ) ;
       fprintf( fid, "\t-test \"%s\"\n", testFileName ) ;
       fprintf( fid, "\t-whitenFlags %x\n", whitenFlags ) ;
       fprintf( fid, "\t-whitenParamSave \"%s\"\n", whitenParamSaveFileName ) ;
       fprintf( fid, "\t-whitenParamLoad \"%s\"\n", whitenParamLoadFileName ) ;
       fprintf( fid, "\t-errorStyle %d\n", errorStyleFlag ) ;
       fprintf( fid, "\t-acceptableError %.10lf\n", acceptableError ) ;
       fprintf( fid, "\t-maxEpochs %d\n", maxEpochs ) ;
       fprintf( fid, "\t-maxRestarts %d\n", maxRestarts ) ;
       fprintf( fid, "\t-learningRateStyle %d\n", learningRateStyle ) ;
       fprintf( fid, "\t-learningRate %.10lf\n", initialLearningRate ) ;
       fprintf( fid, "\t-annealingRate %.10lf\n", annealingRate ) ;
       fprintf( fid, "\t-momentumRate %.10lf\n", momentumRate ) ;
       fprintf( fid, "\t-reportingRate %d\n", reportingRate ) ;
       fprintf( fid, "\t-confidence %.10lf\n", spamConfidence ) ;
       fprintf( fid, "\t-hints %x\n", hints ) ;
       fprintf( fid, "\t-cgd %.10lf\n", cgd) ;
       fprintf( fid, "\t-dataBreakdown %d\n", dataBreakdown ) ;
       fprintf( fid, "\t-cost %0.10lf\n", costMatrixItem ) ;
       fprintf( fid, "\t-tag \"%s\"\n", tag ) ;
}



void CmdLineArgs::usage( FILE * fid )
{
       fprintf( fid, "\t-usage | -help | -h\n" ) ;
       fprintf( fid, "\t-debug | -debugLevel <hex#>\n" ) ;
       fprintf( fid, "\t-seed <#>\n" ) ;
       fprintf( fid, "\t-load <Network FileName>\n" ) ;
       fprintf( fid, "\t-save <Network FileName>\n" ) ;
       fprintf( fid, "\t-train <Data FileName>\n" ) ;
       fprintf( fid, "\t-whiten <Data FileName>\n" ) ;
       fprintf( fid, "\t-validate <Data FileName>\n" ) ;
       fprintf( fid, "\t-test <Data FileName>\n" ) ;
       fprintf( fid, "\t-whitenFlags <hex#>\n" ) ;
       fprintf( fid, "\t-whitenParamSave <Data FileName>\n" ) ;
       fprintf( fid, "\t-whitenParamLoad <Data FileName>\n" ) ;
       fprintf( fid, "\t-errorStyle <0-2>\n" ) ;
       fprintf( fid, "\t-acceptableRrror <#.#>\n" ) ;
       fprintf( fid, "\t-maxEpochs <#>\n" ) ;
       fprintf( fid, "\t-maxRestarts <#>\n" ) ;
       fprintf( fid, "\t-learningRateStyle <0-4>\n" ) ;
       fprintf( fid, "\t-learningRate #.#\n" ) ;
       fprintf( fid, "\t-annealingRate #.#\n" ) ;
       fprintf( fid, "\t-momentumRate #.#\n" ) ;
       fprintf( fid, "\t-reportingRate #\n" ) ;
       fprintf( fid, "\t-confidence #.#\n" ) ;
       fprintf( fid, "\t-hints <hex#>\n" ) ;
       fprintf( fid, "\t-cgd <0 = off, >0 the delta input (0.01 default)>\n" ) ;
       fprintf( fid, "\t-maxRestarts <#>\n" ) ;
       fprintf( fid, "\t-dataBreakdown\n" ) ;
       fprintf( fid, "\t-cost\n" ) ;
       fprintf( fid, "\t-tag <tag string>\n\n" ) ;
}



void CmdLineArgs::readArgs( int argc, char ** argv )
{
       for( int i = 1 ;   i < argc ;   i++ )
       {
               if  ( !strcasecmp( argv[i], "-usage" )  ||
                         !strcasecmp( argv[i], "-help" )  ||
                         !strcasecmp( argv[i], "-?" )  ||
                         !strcasecmp( argv[i], "-h" ) )
               {
                       usage( stderr ) ;
                       exit( 0 ) ;
               }
               else if  ( !strcasecmp( argv[i], "-debug" ) )
                       debug = DEBUG_ACTUAL_ARGS |
				DEBUG_FINAL_STATS ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-debugLevel" ) )
                       debug = atoh( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-seed" ) )
                       seed = atoi( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-load" ) )
                       loadNetworkFileName = argv[++i] ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-save" ) )
                       saveNetworkFileName = argv[++i] ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-train" ) )
                       trainFileName = argv[++i] ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-whiten" ) )
                       whitenFileName = argv[++i] ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-validate" ) )
                       validateFileName = argv[++i] ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-test" ) )
                       testFileName = argv[++i] ;
               else if  ( !strcasecmp( argv[i], "-whitenFlags" ) )
                       whitenFlags = atoh( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-whitenParamSave" ) )
                       whitenParamSaveFileName = argv[++i] ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-whitenParamLoad" ) )
                       whitenParamLoadFileName = argv[++i] ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-errorStyle" ) )
                       errorStyleFlag = atoi( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-acceptableError" ) )
                       acceptableError = atof( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-maxEpochs" ) )
                       maxEpochs = atoi( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-maxRestarts" ) )
                       maxRestarts = atoi( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-hints" ) )
                       hints = atoh( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-cgd" ) )
			   {
                       cgd = atof( argv[++i] );
                       if (cgd <= 0)
						cgd = 0;
               }
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-learningRateStyle" ) )
                       learningRateStyle = atoi( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-learningRate" ) )
                       initialLearningRate = atof( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-annealingRate" ) )
                       annealingRate = atof( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-momentumRate" ) )
                       momentumRate = atof( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-confidence" ) )
                       spamConfidence = atof( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-reportingRate" ) )
                       reportingRate = atoi( argv[++i] ) ;
               else if  ( !strcasecmp( argv[i], "-dataBreakdown" ) )
			dataBreakdown = 1 ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-cost" ) )
                       costMatrixItem = atof( argv[++i] ) ;
               else if  ( ((i+1) < argc )  &&  !strcasecmp( argv[i], "-tag" ) )
                       tag = argv[++i] ;
               else fprintf( stderr, "Bad argument \"%s\"\n", argv[i] ) ;
       }
}
