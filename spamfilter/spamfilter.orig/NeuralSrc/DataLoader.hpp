#ifndef DATA_LOADER_HPP
#define DATA_LOADER_HPP


class DataLoader
{
public:

       enum WHITEN_FLAGS
       {
               WF_INPUTS  = 0x1,
               WF_OUTPUTS = 0x2,
               WF_OUTPUTS_LOGISTIC = 0x4
       } ;

       DataLoader( void ) ;
       ~DataLoader( void ) ;

       int Load( char * filename ) ;
       int Save( char * filename ) ;

       void Whiten(
               int       sampleCnt,
               int       dim,
               double ** data,
               double  * whiteSums,
               double  * whiteSum2s,
               int       recompute ) ;
       void WhitenLogistic(
               int       sampleCnt,
               int       dim,
               double ** data,
               double  * whiteSums,
               double  * whiteSum2s,
               int       recompute ) ;
       void Whiten( int flags, int recompute ) ;

       void SaveWhite( int flags, char * fname ) ;
       void LoadWhite( int flags, int reallocate, char * fname ) ;

       double AverageTrainingSignal(
		int         sampleCnt,
		int         numInputDimensions,
		double      * trainingInputs[],
		int         numOutputDimensions,
		double      * trainingOutputs[],
		int         simpleDivisor ) ;
       double AverageTrainingSignal( int simpleDivisor ) ;


       int           numSamples ;

       int           inputDimensions ;
       double ** inputs ;
       double  * inputSums ;   // whitening component
       double  * inputSum2s ;  // whitening component

       int           outputDimensions ;
       double ** outputs ;
       double  * outputSums ;  // whitening component
       double  * outputSum2s ; // whitening component

       double  * outputMiddles ;


} ;


#endif
