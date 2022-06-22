#ifndef CMD_LINE_ARGS_HPP
#define CMD_LINE_ARGS_HPP


class CmdLineArgs
{
public:

       typedef enum DEBUG_FLAGS
       {
               DEBUG_NONE          = 0x0000,

               DEBUG_ACTUAL_ARGS   = 0x0001,
               DEBUG_INITIAL_NET   = 0x0002,
               DEBUG_FINAL_STATS   = 0x0004,
               DEBUG_FINAL_NET     = 0x0008,

               DEBUG_EPOCH_STATS   = 0x0010,
               DEBUG_EPOCH_NET     = 0x0020,
               DEBUG_INTERVAL      = 0x0040,
               DEBUG_PATTERN       = 0x0080,

               DEBUG_TRAINING      = 0x0100,   // These three work in
               DEBUG_VALIDATION    = 0x0200,   // conjunction with
               DEBUG_TESTING       = 0x0400,   // the next group

               DEBUG_ASSIGN        = 0x1000,
               DEBUG_FEEDFWD       = 0x2000,
               DEBUG_CALCERR       = 0x4000,
               DEBUG_BACKPROP      = 0x8000,

               DEBUG_ALL           = 0xFFFF
       };

       int debug ;
       int seed ;

       char * loadNetworkFileName ;
       char * saveNetworkFileName ;
       char * trainFileName ;
       char * whitenFileName ;
       char * validateFileName ;
       char * testFileName ;

       char * whitenParamSaveFileName ;
       char * whitenParamLoadFileName ;

       int    whitenFlags ;


       typedef enum ERROR_STYLE_FLAGS
       {
               ESF_AVG,
               ESF_RMS,
               ESF_CROSS_ENTROPY_BB,
               ESF_CROSS_ENTROPY_BOOK
       } ;

       int    errorStyleFlag ;
       double acceptableError ;

       int    maxEpochs ;

       typedef enum LEARNING_RATE_STYLES{
       		LRS_FIXED,
       		LRS_EPOCH_ERR_A,
       		LRS_EPOCH_ERR_B,
       		LRS_DECAY_LOG_EPOCH,
       		LRS_DECAY_INV_EPOCH,
       		LRS_ANNEALING
       };

       int    learningRateStyle ;
       double initialLearningRate ;

       int    hints ;  // See NNTrainer
       double cgd;  // Enables conjugate gradient descent reporting

	   double spamConfidence ;
	   int    reportingRate ;

       int    maxRestarts ;
	   int    dataBreakdown ;

       double annealingRate ;
       double momentumRate ;

       double costMatrixItem ;

       char * tag ;


       CmdLineArgs( void ) ;
       ~CmdLineArgs( void ) ;


       void usage( FILE * fid ) ;
       void actualUsage( FILE * fid ) ;

       void readArgs( int argc, char **argv ) ;

       int atoh( char * hexStr ) ;

} ;


#endif
