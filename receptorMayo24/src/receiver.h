#include "signalClient.h"

//Receptor Marratxinet

class receiver
{
    public:
        signalClient e11 = signalClient(3,22,23,24,25,signalClient::typeOrder::toParada);
        signalClient e12 = signalClient(4,26,27,28,29,signalClient::typeOrder::toParada);
 
};


