//Objeto señal de la estructura.
#include "mtfItem.h"
#include "circuit.h"

#ifndef _MONTEFARO_SIGNAL_
#define _MONTEFARO_SIGNAL_

class signal : public mtfItem
{
public:
	enum tSignalOrder {
		viaLibre,
		parada,
		avisoParada,
		precaucion,		
		rebaseAutorizado		
	};
	enum tSignalType {
		normal,	//Señal de salida o intermedia (No puede dar Rebase autorizado)
		entry,  //Señal de entrada (Puede dar Rebase autorizado)
		advance, //Señal de avanzada (No puede dar parada ni rebase autorizado)
		shunt //Señal de retroceso o repetidora (Cuando no da rebase autorizado repite la indicación de la señal relacionada)
	};
	int16_t location[2]; //Coordenadas del punto
	int16_t labelLocation[2]; //Referencia a las coordenadas de la etiqueta
	tSignalType type; //Tipo de esta señal.
	signal* masterSignal; //Señal a la que obedece (sólo si es avanzada o de retroceso)
	circuit* masterCircuit; //Circuito protegido por esta señal (sólo si es de bloqueo)	
	tSignalOrder currentOrder; //Indicación actual de la señal.

	void reset(); //Establece la indicación más restrictiva.
};

#endif // !_MONTEFARO_SIGNAL_