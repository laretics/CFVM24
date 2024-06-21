//Configuración de un circuito de vía
#include <Arduino.h>
class circuitLayout
{
  public:
    circuitLayout();
    void *getNext(bool even);
    void setNext( void* oddPtr, void*evenPtr);
    int16_t mcolTraces[2]; //Array de punteros a los trazos.
    uint8_t mcolTracesCount[2]; //Número de trazos activos e inactivos.

  protected:
    void *nextOdd; //Siguiente circuito por la izquierda
    void *nextEven; //Siguiente circuito por la derecha
  private:

};