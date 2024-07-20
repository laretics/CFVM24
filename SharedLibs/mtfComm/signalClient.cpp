#include "signalClient.h"

signalClient::signalClient()
{
    /*
    channel = chanel_id;
    setLedPort(typeColor::tcGreen,greenPort);
    setLedPort(typeColor::tcRed,redPort);
    setLedPort(typeColor::tcYellow,yellowPort);
    setLedPort(typeColor::tcWhite,whitePort);
    

    setOrder(defaultOrder);
    */
}

void signalClient::init()
{
    uint8_t auxPort;
    for(uint8_t i=0;i<4;i++)
    {
        auxPort = getLedPort((typeColor)i);
        if(auxPort<255) pinMode(auxPort,OUTPUT);
    }
    setOrder(mvarDefaultOrder); //Muestra la orden por defecto.
}

void signalClient::setLedPort(typeColor color, uint8_t port)
{
    mcolLedPort[color]=port;
}
uint8_t signalClient::getLedPort(typeColor color)
{
    return mcolLedPort[color];
}
void signalClient::setOrder(typeOrder order)
{
    mvarOrder=order;
    auxSetOrder();
}
signalClient::typeOrder signalClient::getOrder()
{
    return mvarOrder;
}

void signalClient::auxSetOrder()
{
    switch (mvarOrder)
    {
    case typeOrder::toViaLibre:
        auxSetLight(typeColor::tcGreen,true);
        auxSetLight(typeColor::tcRed,false);
        auxSetLight(typeColor::tcYellow,false);
        auxSetLight(typeColor::tcWhite,false);    
        break;
    case typeOrder::toParada:
        auxSetLight(typeColor::tcGreen,false);
        auxSetLight(typeColor::tcRed,true);
        auxSetLight(typeColor::tcYellow,false);
        auxSetLight(typeColor::tcWhite,false);    
        break;    
    case typeOrder::toAvisoDeParada:
        auxSetLight(typeColor::tcGreen,false);
        auxSetLight(typeColor::tcRed,false);
        auxSetLight(typeColor::tcYellow,true);
        auxSetLight(typeColor::tcWhite,false);    
        break;        
    case typeOrder::toPrecaucion:
        auxSetLight(typeColor::tcGreen,true);
        auxSetLight(typeColor::tcRed,false);
        auxSetLight(typeColor::tcYellow,true);
        auxSetLight(typeColor::tcWhite,false);    
        break;            
    case typeOrder::toRebaseAutorizado:
        auxSetLight(typeColor::tcGreen,false);
        auxSetLight(typeColor::tcRed,true);
        auxSetLight(typeColor::tcYellow,false);
        auxSetLight(typeColor::tcWhite,true);    
        break;                
    default:
        auxSetLight(typeColor::tcGreen,false);
        auxSetLight(typeColor::tcRed,false);
        auxSetLight(typeColor::tcYellow,false);
        auxSetLight(typeColor::tcWhite,false);    
        break;
    }
}

void signalClient::setInverted(bool red, bool green, bool yellow, bool white)
{
    mcolInverted[typeColor::tcGreen]=green;
    mcolInverted[typeColor::tcRed]=red;
    mcolInverted[typeColor::tcYellow]=yellow;
    mcolInverted[typeColor::tcWhite]=white;
}
void signalClient::setDefaultOrder(typeOrder order)
{
    mvarDefaultOrder=order;
}
void signalClient::auxSetLight(typeColor color, bool rhs)
{
    uint8_t auxPort = mcolLedPort[color];
    if(mcolInverted[color])
        digitalWrite(auxPort,rhs);
    else
        digitalWrite(auxPort,!rhs);
}