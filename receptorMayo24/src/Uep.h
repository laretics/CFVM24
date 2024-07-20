//Módulo Interface para dispositivo se señalización dotado de consola básica UEP.
#include <TimerOne.h>
#include "mtfComm.h"
#include "receiver.h"
#include <Wire.h>
#include <MultiFuncShield.h>

class Uep
{
  
  enum recStatus
  {
    iddle, //Modo de reposo por defecto
    mainMenu, //Menú de selección de opciones
    signalSelect, //Selección de señal
    pointSelect, //Selección de aguja
    signalDebug, //Mantenimiento de señal
    pointDebug //Mantenimiento de aguja
  };
  recStatus mvarCurrentStatus; //Estado actual

  int mvarCurrentOptionIndex; //Índice de la opción actual
  int mvarCurrentElementIndex; //índice del elemento actual de depuración

  public:
      receiver rec = receiver();
      void init();
      void scan();

  private:
      void jumpStatus(recStatus destination, uint8_t buzzer);
      void consumeKey();
      void update();

      void processMainMenu(uint8_t buttonNumber, uint8_t buttonAction);
      void processSignalSelect(uint8_t buttonNumber, uint8_t buttonAction);
      void processPointSelect(uint8_t buttonNumber, uint8_t buttonAction);
      void processSignalDebug(uint8_t buttonNumber, uint8_t buttonAction);


};
