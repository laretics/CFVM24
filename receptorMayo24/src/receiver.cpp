#include "receiver.h"
void receiver::init()
{
    for(uint8_t i=0;i<SIGNALS_COUNT;i++)
    {
        if(0!=mcolSignals[i].getLedPort(signalClient::typeColor::tcRed))
        {
            mcolSignals[i].init();
        }
    }
}