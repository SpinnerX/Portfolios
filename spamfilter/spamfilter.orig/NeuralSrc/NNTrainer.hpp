#include <iostream>
#include <sys/time.h>


#include "NeuralNet.hpp"
using namespace std;

class IndexGroup
{
public:

       IndexGroup::IndexGroup( void ) ;
       IndexGroup::~IndexGroup( void ) ;

       void Allocate( int qty, int assign = 1 ) ;
       void Shuffle( void ) ;

       int NextIndex( void ) ;
       int AvailableIndexes( void ) ;


       // Should really make accessors to these, but will do it later

       int numAllocated ;
       int * indexes ;
       int numIndexes ;
       int indexPosn ;

} ;




class NNTrainer
{
public:

       typedef enum NN_DESC_HINTS
       {
               NN_HINT_NONE       = 0x0,
               NN_HINT_CLASSIFIER = 0x1,

               NN_HINT_NUM_HINTS
       } ;

       NNTrainer( void ) ;
       virtual ~NNTrainer( void ) ;

       void AllocateIndexes( int numIndexes ) ;
       void AllocateIndexes(
               int       numSamples,
               int       numDimensions,
               double ** samples,
               int       numClassifications ) ;
       void ReleaseOldIndexes( void ) ;

       void Shuffle( void ) ;

       int  NextIndex( void ) ;

       int Epoch( void ) ;
       void StartEpoch( void ) ;
       // void EndEpoch( void ) ;
       double EpochError( void ) ;


       void TrainStochasticGroup(
               NeuralNet & network,
               int         numTrainingPatterns,
               int         numInputDimensions,
               double      * trainingInputs[],
               int         numOutputDimensions,
               double      * trainingOutputs[],
               double      trainingOutputMiddles[],
               double      acceptableAvgError,
               int         maxTrainingEpochs,
               int         hints,
			   double      avgTrainSignalFactor ) ;

/*
       void TrainBatchGroup(
               NeuralNet & network,
               int         numTrainingPatterns,
               int         numInputDimensions,
               double      * trainingInputs[],
               int         numOutputDimensions,
               double      * trainingOutputs[],
               double      acceptableAvgError,
               int         maxTrainingEpochs,
               int         hints ) ;
*/

protected:

       // int          hints ;

       int          numGroups ;        // Number of classifier groups
       IndexGroup * groups ;           // Input indexes by group

       int          groupPosn ;        // Next group for index selection
       int          groupAmount ;      // Count for next index purposes

       double       groupRatio ;       // ratio for binary classifiers
       int          groupLarger ;      // index of larger group


       int          epoch ;
       double       epochError ;

		double tp, fp, fn, tn ;
		double qtp, qfp, qfn, qtn ;
		struct timeval epochStartTime ;


                               int errorsUp; //WK
                               double lastError; //WK
       // NeuralNet  * network ;

} ;


ostream & operator<<( ostream & os, NNTrainer & nnTrainer ) ;



