//Objeto circuito de vía de la estructura de enclavamiento
#ifndef _TRACK_CIRCUIT_
#define _TRACK_CIRCUIT_
#include "mtfItem.h"
#include "circuitLayout.h"

class circuit : public mtfItem
{
  public:
//declaracion
  enum statusType {
    error, //Fallo del enclavamiento
    disabled, //Circuito no activo
    iddle, //Sin ocupar
    locked, //Enclavado
    occupied, //Ocupado
    shunt //Maniobras
  };

    circuitLayout* mcolLayout;
    uint8_t layoutCount; //Número de configuraciones de este circuito.
    int16_t location[2]; //Coordenadas del punto
    int16_t labelLocation[2]; //Referencia a las coordenadas de la etiqueta
    int16_t boundingBox[4]; //Coordenadas de la boundingBox del circuito
    int16_t pointsCoordinatesIndex; //Colección de puntos donde pintar las agujas.
    uint8_t pointsCount; //Referencia número de puntos de agujas.

    uint8_t currentLayout;  //Configuración de agujas actual.
    statusType currentStatus; //Estado actual de este circuito.

    uint16_t statusColor(statusType rhs); //Color del circuito según el estado.
    uint16_t pointStatusColor(statusType rhs); //Color de las agujas según el estado del circuito.
    uint16_t currentStatusColor(); //Color de las partes activas del circuito.
    uint16_t currentDisableStatusColor(); //Color de las partes inactivas del circuito.
    uint16_t currentPointStatusColor(); //Color de las agujas según el estado actual.
    circuitLayout *currentPLayout(){return &mcolLayout[currentLayout];};

    circuit();

    bool contains(int16_t x, int16_t y);

  protected:

  private:

};


#endif