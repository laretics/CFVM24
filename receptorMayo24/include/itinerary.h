#include "mtfItem.h"
#include "circuit.h"
#include "signal.h"
/*
	Un itinerario contiene una lista de �rdenes a cursar al enclavamiento.
	Antes de ejecutar estas �rdenes, habr� que verificar que todos los circuitos
	implicados en la orden est�n libres.
*/

class itinerary: public mtfItem
{
public:
	circuit** path; //Colecci�n de circuitos del camino.
	uint8_t* layout; //Colecci�n de configuraciones para cada circuito.
	uint8_t count; //N�mero de circuitos del camino.
	signal** signals; //Colecci�n de se�ales a cambiar del camino.
	signal::tSignalOrder* orders; //Orden de cada se�al del camino.
	uint8_t signalCount; //N�mero de se�ales a cambiar.

	itinerary(circuit* auxPath, uint8_t* auxLayout);
	itinerary();
};