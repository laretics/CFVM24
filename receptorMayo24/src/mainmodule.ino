
#include "Uep.h"
Uep uep = Uep();


void setup(void)
{
  Serial.begin(9600);
  Serial1.begin(9600);
  uep.rec.mcolSignals[1].setLedPort(signalClient::typeColor::tcWhite,23); //Cable blanco
  uep.rec.mcolSignals[1].setLedPort(signalClient::typeColor::tcYellow,22); //Cable gris
  uep.rec.mcolSignals[1].setLedPort(signalClient::typeColor::tcRed,25); //Cable violeta
  uep.rec.mcolSignals[1].setLedPort(signalClient::typeColor::tcGreen,24); //Cable azul
  uep.rec.mcolSignals[1].setInverted(true,false,false,false);
  uep.rec.mcolSignals[1].setDefaultOrder(signalClient::typeOrder::toParada);
  uep.rec.mcolSignals[1].channel=4;

  uep.rec.mcolSignals[0].setLedPort(signalClient::typeColor::tcWhite,27); //Cable verde
  uep.rec.mcolSignals[0].setLedPort(signalClient::typeColor::tcYellow,26); //Cable amarillo
  uep.rec.mcolSignals[0].setLedPort(signalClient::typeColor::tcRed,29); //Cable naranja
  uep.rec.mcolSignals[0].setLedPort(signalClient::typeColor::tcGreen,28); //Cable marrón
  uep.rec.mcolSignals[0].setInverted(true,false,false,false);
  uep.rec.mcolSignals[0].setDefaultOrder(signalClient::typeOrder::toParada);
  uep.rec.mcolSignals[0].channel=3;

  uep.init();
}


void loop() 
{
  uep.scan();
}







