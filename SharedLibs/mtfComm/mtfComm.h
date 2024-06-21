//Puerto por el que vamos a sacar la informaci�n.
#include <Arduino.h>
#ifndef _MTF_COMM_
#define _MTF_COMM_
#define DEBUG_SERIAL 1
#define NUM_CIRCUIT 14
#define NUM_SIGNAL 10
#define HEADER_COUNT 3
//Cabecera de envío de información
#define COMM_HEADER 254
#define BUFFER_LENGHT NUM_CIRCUIT+NUM_CIRCUIT+NUM_SIGNAL+HEADER_COUNT+2
//Número de circuitos + número de circuitos + número de señales + longitud de cabecera (3) + crc (1) + command (1).
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#define PORT Serial1
#else
	#define PORT Serial
#endif

/*
	Módulo de comunicaciones del sistema Montefaro Lock

	Implementa el protocolo interno de comunicaciones para transmitir la información y los eventos de un enclavamiento entre los módulos
	a través del puerto serie.


	Datagramas desde central a dispositivos de salida:

	Campo			Tamano (bytes)		Notas
	Command				1				Comando del datagrama.
	CircuitCount		1				Número de circuitos.
	SignalCount			1				Número de señales.
	Circuit 1 status	1				Estado del primer circuito
	Circuit 1 layout	1				Configuración del primer circuito
	Circuit 2 status	1				Estado del segundo circuito
	Circuit 2 layout	1				Configuración del segundo circuito
	...					...				...
	Circuit n status	1				Estado del último circuito
	Circuit n layout	1				Configuración del último circuito
	Signal 1 order		1				Orden de la primera señal
	Signal 2 order		1				Orden de la segunda señal
	...					...				...
	Signal n order		1				Orden de la última señal
	CRC					1				Número de comprobación de redundancia cíclica.
*/

class mtfComm
{
public:
	const uint8_t headerArray[HEADER_COUNT] = {254,255,12};
	mtfComm();
	void send();
	bool receive();
	void dumpMem(); //Escribe el contenido del buffer en el puerto serie para depuración.
	void setCommand(uint8_t rhs);
	void setCircuit(uint16_t index, uint8_t layout, uint8_t status);
	uint8_t getCircuitLayout(uint16_t index);
	uint8_t getCircuitStatus(uint16_t index);
	void setSignal(uint16_t index, uint8_t order);
	uint8_t getSignalOrder(uint16_t index);
protected:
	uint8_t mcolBuffer[BUFFER_LENGHT];

private:
	uint8_t calculateCRC(); //precálculo del CRC con los datos actuales.
	bool syncHeader(); //Consume los datos del buffer hasta encontrar la cabecera.
};

#endif