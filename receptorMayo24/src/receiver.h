#include "signalClient.h"
#ifndef RECEIVER_MODULE
#define RECEIVER_MODULE 0
#define SIGNALS_COUNT 4


//Receptor universal de señales y agujas
class receiver
{
    public:
        signalClient mcolSignals[SIGNALS_COUNT];
        void init();
 
};

#endif


