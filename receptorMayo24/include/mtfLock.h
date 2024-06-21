//Clase base de enclavamiento para la arquitectura
#ifndef _MTF_LOCK_
#define _MTF_LOCK_
#define SCALE_X 0.3
#define SCALE_Y 1
#include "signal.h"
#include "order.h"
#include "itinerary.h"
#include "platform.h"
#include "mtfComm.h" //Módulo de comunicaciones
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

class mtfLock
{
  public:
    void draw(int16_t offsetX, int16_t offsetY);
    circuit* getCircuit(int16_t x, int16_t y); //Devuelve el circuito en las coordenadas del punto
    mtfLock(Elegoo_TFTLCD* tft);
    mtfComm mainComm = mtfComm();
    void initialize_structure(); //Inicia la estructura
    void create_routes(); //Enlaza los itinerarios
    void tapCircuit(circuit* rhs); //Modifica el estado del circuito para depuración.
    void dai(); //Pone todos los circuitos en estado "Iddle".
    bool processOrder(); //Procesa la orden actual (True si ha podido hacerlo)
    order currentOrder; //Orden a procesar.

  protected:
    Elegoo_TFTLCD* mvarTft;
    circuit* mcolCircuit;
    signal* mcolSignal;
    itinerary* mcolItinerary;
    platform* mcolPlatform;
    uint8_t circuitCount;
    uint8_t signalCount;  
    uint8_t platformCount;
    int16_t *rawData;    

  //Funciones de dibujo en display TFT
  circuit* mvarCurrentCircuit; //Circuito seleccionado.
  signal* mvarCurrentSignal; //Señal seleccionada.
  platform* mvarCurrentPlatform; //Andén seleccionado.
  uint8_t mvarScreenId; //Pantalla a dibujar.
  int16_t mvarOffsetX; //Coordenadas del offset.
  int16_t mvarOffsetY;
  int16_t mvarLocalOffsetX; //Coordenadas locales de offset.
  int16_t mvarLocalOffsetY; 

  private:
  void drawCircuit();
  void drawSignal();
  void drawPlatform();
  void parseTraces(uint16_t color, uint16_t pointer, uint8_t count); //Ejecuta un trazo con valores del array
  void updateSignals(circuit* rhs); //Actualiza las señales ante un cambio de estado en un circuito de vía.
  void copyToComm(); //Traslada el estado actual del enclavamiento al buffer de comunicaciones.

};

#endif