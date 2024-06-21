#include "signalClient.h"

signalClient::signalClient(uint16_t chanel_id,uint8_t greenPort,uint8_t redPort, uint8_t yellowPort, uint8_t whitePort,typeOrder defaultOrder)
{
    channel = chanel_id;
    setLedPort(typeColor::tcGreen,greenPort);
    setLedPort(typeColor::tcRed,redPort);
    setLedPort(typeColor::tcYellow,yellowPort);
    setLedPort(typeColor::tcWhite,whitePort);
    uint8_t auxPort;
    for(uint8_t i=0;i<4;i++)
    {
        auxPort = getLedPort((typeColor)i);
        if(auxPort<255) pinMode(auxPort,OUTPUT);
    }
    setOrder(defaultOrder);
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
        auxSetLight(mcolLedPort[typeColor::tcGreen],true);
        auxSetLight(mcolLedPort[typeColor::tcRed],false);
        auxSetLight(mcolLedPort[typeColor::tcYellow],false);
        auxSetLight(mcolLedPort[typeColor::tcWhite],false);
        break;
    case typeOrder::toParada:
        auxSetLight(mcolLedPort[typeColor::tcGreen],false);
        auxSetLight(mcolLedPort[typeColor::tcRed],true);
        auxSetLight(mcolLedPort[typeColor::tcYellow],false);
        auxSetLight(mcolLedPort[typeColor::tcWhite],false);
        break;    
    case typeOrder::toAvisoDeParada:
        auxSetLight(mcolLedPort[typeColor::tcGreen],false);
        auxSetLight(mcolLedPort[typeColor::tcRed],false);
        auxSetLight(mcolLedPort[typeColor::tcYellow],true);
        auxSetLight(mcolLedPort[typeColor::tcWhite],false);
        break;        
    case typeOrder::toPrecaucion:
        auxSetLight(mcolLedPort[typeColor::tcGreen],true);
        auxSetLight(mcolLedPort[typeColor::tcRed],false);
        auxSetLight(mcolLedPort[typeColor::tcYellow],true);
        auxSetLight(mcolLedPort[typeColor::tcWhite],false);
        break;            
    case typeOrder::toRebaseAutorizado:
        auxSetLight(mcolLedPort[typeColor::tcGreen],false);
        auxSetLight(mcolLedPort[typeColor::tcRed],true);
        auxSetLight(mcolLedPort[typeColor::tcYellow],false);
        auxSetLight(mcolLedPort[typeColor::tcWhite],true);
        break;                
    default:
        auxSetLight(mcolLedPort[typeColor::tcGreen],false);
        auxSetLight(mcolLedPort[typeColor::tcRed],false);
        auxSetLight(mcolLedPort[typeColor::tcYellow],false);
        auxSetLight(mcolLedPort[typeColor::tcWhite],false);    
        break;
    }
}

void signalClient::setInverted(bool red, bool green, bool yellow, bool white)
{
    mcolInverted[0]=red;
    mcolInverted[1]=green;
    mcolInverted[2]=yellow;
    mcolInverted[3]=white;
    auxSetOrder();    
}
void signalClient::auxSetLight(uint8_t port, bool rhs)
{
    if(mcolInverted[port])
        digitalWrite(port,rhs);
    else
        digitalWrite(port,!rhs);
}