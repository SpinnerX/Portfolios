#include<iostream>
#include<iomanip.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include<math.h>

#include"DataLoader.hpp"


DataLoader::DataLoader( void ) :
       numSamples( 0 ),
       inputDimensions( -1 ),
       inputs( NULL ),
       outputDimensions( -1 ),
       outputs( NULL ),
       outputMiddles( NULL ),
       inputSums( NULL ),
       inputSum2s( NULL ),
       outputSums( NULL ),
       outputSum2s( NULL )
{
}


DataLoader::~DataLoader( void )
{
       int i  ;

       if  ( inputs != NULL )
               for( i = 0 ;   i < numSamples ;   i++ )
                       delete [] inputs[i] ;
               delete [] inputs ;
       if  ( inputSums )
               delete [] inputSums ;
       if  ( inputSum2s )
               delete [] inputSum2s ;

       if ( outputs != NULL )
               for( i = 0 ;   i < numSamples ;   i++ )
                       delete [] outputs[i] ;
               delete [] outputs ;
       if  ( outputSums )
               delete [] outputSums ;
       if  ( outputSum2s )
               delete [] outputSum2s ;
       if  ( outputMiddles )
               delete [] outputMiddles ;
}


int DataLoader::Load( char * filename )
{
       int i, j, error = 0 ;
       FILE * fid = fopen( filename, "r" ) ;

       if  ( NULL != fid )
       {
               if  ( fscanf( fid, "%d%d%d",
                               &numSamples,
                               &inputDimensions,
                               &outputDimensions ) == 3 )
               {
/*
cerr << "Expect data " << filename << " "
       << numSamples << ", "
       << inputDimensions << ", "
       << outputDimensions << endl ;
*/
                       inputs = new double * [ numSamples ] ;
                       outputs = new double * [ numSamples ] ;

                       for( i = 0 ;   !error  &&  ( i < numSamples ) ;   i++ )
                       {
                               j = 0 ;
                               inputs[ i ] = new double[ inputDimensions ] ;
                               while( !feof( fid )  &&  ( j < inputDimensions ) )
                               {
                                       if  ( !fscanf( fid, "%lf", &((inputs[ i ])[ j ]) ) )
                                               error = 3 ;
// else cerr << "input[ " << i << " ][ " << j << " ] = " << (inputs[ i ])[ j ] << endl ;
                                       j++ ;
                               }


                               if  ( outputDimensions > 0 )
                               {
                                       j = 0 ;
                                       outputs[ i ] = new double[ outputDimensions ] ;
                                       while( !feof( fid )  &&  ( j < outputDimensions ) )
                                       {
                                       if  ( !fscanf( fid, "%lf", &((outputs[ i ])[ j ]) ) )
                                               error = 3 ;
// else cerr << "    output[ " << i << " ][ " << j << " ] = " << (outputs[ i ])[ j ] << endl ;
                                       j++ ;
                                       }
                               }
                       }
                       if  ( error )
                       {
                               numSamples = i - 1 ;
                               if  ( numSamples > 0 )
                                       error = 0 ;
                       }
               }
               else error = 2 ;

               fclose( fid ) ;
       }
       else error = 1 ;

       return( error ) ;
}


int DataLoader::Save( char * filename )
{
       FILE * fid = fopen( filename, "w" ) ;

       if  ( NULL != fid )
       {
               fprintf( fid, "%d\n%d\n%d\n",
                       numSamples, inputDimensions, outputDimensions ) ;
               for( int i = 0 ;   i < numSamples ;   i++ )
               {
                       int j ;
                       for( j = 0 ;   j < inputDimensions ;   j++ )
                               fprintf( fid, " %.10f", (inputs[ i ])[ j ] ) ;
                       for( j = 0 ;   j < outputDimensions ;   j++ )
                               fprintf( fid, " %.10f", (outputs[ i ])[ j ] ) ;
                       fprintf( fid, "\n" ) ;
               }

               return( 0 ) ;
       }
       else fprintf( stderr, "Failed to open \"%s\" for output\n", filename ) ;

       return( 1 ) ;
}


void DataLoader::Whiten(
       int sampleCnt,
       int dim,
       double ** data,
       double  * whiteSums,
       double  * whiteSum2s,
       int recompute )
{
       int i, j ;

       if  ( recompute )
       {
       for( i = 0 ;   i < sampleCnt ;   i++ )
       {
               for( j = 0 ;   j < dim ;   j++ )
               {
//cerr << (data[ i ])[j] << "  " ;
                       whiteSums[ j ] += (data[ i ])[ j ] ;
                       whiteSum2s[ j ] += ( (data[ i ])[ j ] * (data[ i ])[ j ] ) ;
               }
//cerr << endl ;
       }

       for( j = 0 ;   j < dim ;   j++ )
       {
        whiteSum2s[ j ] = sqrt( ( whiteSum2s[ j ] - double(whiteSums[j] * whiteSums[j]) / double( sampleCnt ) ) / double( sampleCnt - 1 ) ) ;
               whiteSums[ j ] /= double( sampleCnt ) ;
//cerr << "Dim " << j << "  mean: " << whiteSums[ j ] << "   sigma: " << whiteSum2s[ j ] << endl ;
			   if  ( whiteSum2s[ j ] < 0.000001 )
               	whiteSum2s[ j ] = 1.0 ;
               else whiteSum2s[ j ] = 1.0 / whiteSum2s[ j ] ;       // so we can multiply later
       }
       }

       for( i = 0 ;   i < sampleCnt ;   i++ )
       {
               for( j = 0 ;   j < dim ;   j++ )
               {
                       (data[ i ])[ j ] -= whiteSums[ j ] ;    // set to mean  = 0
                       (data[ i ])[ j ] *= whiteSum2s[ j ] ;   // set to sigma = 1
//if  ( !i  )
//	cerr << "whitened data " << (data[i])[ j ] << ", " << (data[i])[ j ] << endl ;
               }
       }
}



void DataLoader::WhitenLogistic(
       int sampleCnt,
       int dim,
       double ** data,
       double  * whiteSums,
       double  * whiteSum2s,
       int recompute )
{
       int i, j ;

       for( i = 0 ;   i < sampleCnt ;   i++ )
       {
               for( j = 0 ;   j < dim ;   j++ )
               {
			// Grand assumption data is already in range [0,1]
			if  ( (data[ i ])[ j ] < 0.5 )
				(data[ i ])[ j ] = 0.1 ;
			else (data[ i ])[ j ] = 0.9 ;
               }
       }
}



void DataLoader::Whiten( int flags, int recompute )
{
       if  ( flags & WF_INPUTS )
       {
               if  ( recompute )
               {
                       inputSums = new double[ inputDimensions ] ;
                       inputSum2s = new double[ inputDimensions ] ;
                       memset( inputSums, 0, inputDimensions * sizeof( double ) ) ;
                       memset( inputSum2s, 0, inputDimensions * sizeof( double ) ) ;
               }

               Whiten( numSamples, inputDimensions, inputs,
                       inputSums, inputSum2s, recompute ) ;
       }

       if  ( flags & WF_OUTPUTS )
       {
               if  ( recompute )
               {
                       outputSums = new double[ outputDimensions ] ;
                       outputSum2s = new double[ outputDimensions ] ;
                       memset( outputSums, 0, outputDimensions * sizeof( double ) ) ;
                       memset( outputSum2s, 0, outputDimensions * sizeof( double ) ) ;
               }

               Whiten( numSamples, outputDimensions, outputs,
                       outputSums, outputSum2s, recompute ) ;
       }

       if  ( flags & WF_OUTPUTS_LOGISTIC )
       {
               WhitenLogistic( numSamples, outputDimensions, outputs,
                       outputSums, outputSum2s, recompute ) ;
	}

// The following nonsense is about handling uneven class distribution training.
// Because we set the system for 0 mean and 1 sigma, a higher probability class
// will have a number closer to 0, but it seems that if going with a softmax to
// determine a 1 of N encoding, we should be using the middle vs 0, so try this.
	outputMiddles = new double[ outputDimensions ] ;
	double * outputMiddles2 = new double[ outputDimensions ] ;
	memset( outputMiddles, 0, outputDimensions * sizeof( double ) ) ;
	memset( outputMiddles2, 0, outputDimensions * sizeof( double ) ) ;

	int i, j ;
	for( j = 0 ;   j < outputDimensions ;   j++ )
		outputMiddles[ j ] = outputMiddles2[ j ] = (outputs[ 0 ])[ j ] ;

	for( i = 1 ;   i < numSamples ;   i++ )
	{
		for( j = 0 ;   j < outputDimensions ;   j++ )
		{
			if  ( (outputs[ i ])[ j ] < outputMiddles[ j ] )
				outputMiddles[ j ] = (outputs[ i ])[ j ] ;
			else if  ( (outputs[ i ])[ j ] > outputMiddles2[ j ] )
				outputMiddles2[ j ] = (outputs[ i ])[ j ] ;
		}
	}

	for( j = 0 ;   j < outputDimensions ;   j++ )
		outputMiddles[ j ] = ( outputMiddles[ j ] + outputMiddles2[ j ] ) / 2.0 ;

	delete [] outputMiddles2 ;
}


void DataLoader::SaveWhite( int flags, char * fname )
{
	FILE * fid ;
	int j ;

	if  ( NULL == ( fid = fopen( fname, "w" ) ) )
		return ;

	if  ( flags & WF_INPUTS )
	{
		for( j = 0 ;   j < inputDimensions ;   j++ )
			fprintf( fid, " %0.10lf", inputSums[ j ] ) ;
		fprintf( fid, "\n" ) ;

		for( j = 0 ;   j < inputDimensions ;   j++ )
			fprintf( fid, " %0.10lf", inputSum2s[ j ] ) ;
		fprintf( fid, "\n" ) ;
	}


	if  ( flags & WF_OUTPUTS )
	{
		for( j = 0 ;   j < outputDimensions ;   j++ )
			fprintf( fid, " %0.10lf", outputSums[ j ] ) ;
		fprintf( fid, "\n" ) ;

		for( j = 0 ;   j < outputDimensions ;   j++ )
			fprintf( fid, " %0.10lf", outputSum2s[ j ] ) ;
		fprintf( fid, "\n" ) ;
	}

	fclose( fid ) ;
}


void DataLoader::LoadWhite( int flags, int reallocate, char * fname )
{
	FILE * fid ;
	int j ;

	if  ( NULL == ( fid = fopen( fname, "r" ) ) )
	{
		cerr << "Warning: failed to load white data" << endl ;
		return ;
	}

       if  ( flags & WF_INPUTS )
       {
               if  ( reallocate )
               {
                       inputSums = new double[ inputDimensions ] ;
                       inputSum2s = new double[ inputDimensions ] ;
                       memset( inputSums, 0, inputDimensions * sizeof( double ) ) ;
                       memset( inputSum2s, 0, inputDimensions * sizeof( double ) ) ;
               }

		for( j = 0 ;   j < inputDimensions ;   j++ )
			fscanf( fid, "%lf", &inputSums[ j ] ) ;

		for( j = 0 ;   j < inputDimensions ;   j++ )
			fscanf( fid, "%lf", &inputSum2s[ j ] ) ;

//for( j = 0 ;   j < inputDimensions ;   j++ )
//	cerr << "Loaded in whitened data " << inputSums[ j ] << ", " << inputSum2s[ j ] << endl ;
       }


       if  ( flags & WF_OUTPUTS )
       {
               if  ( reallocate )
               {
                       outputSums = new double[ outputDimensions ] ;
                       outputSum2s = new double[ outputDimensions ] ;
                       memset( outputSums, 0, outputDimensions * sizeof( double ) ) ;
                       memset( outputSum2s, 0, outputDimensions * sizeof( double ) ) ;
               }

		for( j = 0 ;   j < outputDimensions ;   j++ )
			fscanf( fid, "%lf", &outputSums[ j ] ) ;

		for( j = 0 ;   j < outputDimensions ;   j++ )
			fscanf( fid, "%lf", &outputSum2s[ j ] ) ;

//for( j = 0 ;   j < outputDimensions ;   j++ )
//	cerr << "Loaded out whitened data " << outputSums[ j ] << ", " << outputSum2s[ j ] << endl ;
       }
}



double DataLoader::AverageTrainingSignal(
	int      sampleCnt,
	int      numInputDimensions,
	double * trainingInputs[],
	int      numOutputDimensions,
	double * trainingOutputs[],
	int      simpleDivisor )
{
	int i, j ;
	double errorDivisor = 0.0 ;

	double * trainingOutputMean = new double[ numOutputDimensions ] ;
	memset( trainingOutputMean, 0, numOutputDimensions * sizeof( double ) ) ;

	if  ( !simpleDivisor )
	{
		// If data were whitened, this would be unnecessary
		// However, we cannot count on that, so do this
		for( i = 0 ;   i < sampleCnt ;   i++ )
		{
			for( j = 0 ;   j < numOutputDimensions ;   j++ )
				trainingOutputMean[ j ] += (trainingOutputs[ i ])[ j ] ;
		}
		for( j = 0 ;   j < numOutputDimensions ;   j++ )
			trainingOutputMean[ j ] /= double( sampleCnt ) ;

		// Now compute divisor for future error computations
		for( i = 0 ;   i < sampleCnt ;   i++ )
		{
			double dist = 0.0 ;
			for( j = 0 ;   j < numOutputDimensions ;   j++ )
			{
				double tmp = (trainingOutputs[ i ])[ j ] - trainingOutputMean[ j ] ;
				dist += tmp * tmp ;
			}
			errorDivisor += dist ;
		}
		if  ( errorDivisor < 0.0000001 )
			errorDivisor = 1.0 ;
		else errorDivisor = 1.0 / errorDivisor ;          // So we can multiply it later
	}
	else errorDivisor = 1.0 / double( sampleCnt ) ;

	return( errorDivisor ) ;
}


double DataLoader::AverageTrainingSignal( int simpleDivisor )
{
	return( AverageTrainingSignal( numSamples, inputDimensions, inputs, outputDimensions, outputs, simpleDivisor ) ) ;
}
