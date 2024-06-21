//Orden que enviamos al enclavamiento
#include <arduino.h>

class order
{
public:
	enum orderType {
		orderNone,		//Orden nula (por defecto).
		orderArrival, //Llegada de un tren a una v�a.
		orderDeparture, //Salida de un tren de una v�a.
		orderPass //Paso directo de un tren por una v�a.
	};
	order();
	orderType mvarOrder; //Orden dada.
	uint8_t mvarPlatform; //V�a a la que se da la orden.
	int8_t mvarItineraryIndex; //�ndice del array calculado al llamar a "toString"

	const char* toString(); //Texto generado con la orden.
};