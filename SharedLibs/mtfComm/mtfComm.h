#include <Arduino.h>
#ifndef _MTF_COMM_
#define _MTF_COMM_
//Definición para depurar las comunicaciones
#define DEBUG_SERIAL 1
//Número máximo de circuitos o señales direccionados en cada uno de los buffers
#define MAX_ELEMENTS 57
//Número máximo de clientes instalados en el CTC
#define NUM_CLIENTS 16
//Tamaño de la cabecera
#define HEADER_COUNT 3
#define HEADER_ARRAY {254,255,12}
//Número de bytes de los contadores de circuitos (Uint16) y señales (Uint16)-> 2+2
#define CIRCUIT_AND_SIGNALS 4
//Cabecera de envío de información
#define COMM_HEADER 254

#define COMMAND_SEND_CIRCUIT_LAYOUT 1
#define COMMAND_RECEIVE_CHANGES 2
#define COMMAND_SEND_CIRCUIT_STATUS 3
#define COMMAND_SEND_SIGNALS 5
#define COMMAND_SEND_QUERY 7


//Tamaño del buffer que contiene el máximo de circuitos o señales
#define BUFFER_SIZE HEADER_COUNT+1+1+MAX_ELEMENTS+1
	// Tamaño de la cabecera (HEADER_COUNT) + 
	// Tamaño del comando (1) +
	// Tamaño del número de componentes (1) +
	// Máximo número de elementos (MAX_ELEMENTS) +
	// CRC (1)

//Máximo de cambios que puede reportar un módulo al enclavamiento.
#define MAX_CHANGES 8
//Tamaño del buffer de entrada (Header + comando + número de cambios + (tamaño de cada cambio * MAX_CHANGES) )+crc
//Al ser el mayor tamaño del paquete de entrada, lo usaré para procesar cualquier entrada de datos desde un dispositivo.
#define INPUT_LENGHT COMM_HEADER+1+1+(MAX_CHANGES*4)+1

#define CLIENT_RECEIVE_TRYOUTS 3
//Número de reintentos que hace el servidor para que un cliente conteste a una petición
#define CLIENT_RECEIVE_TIMEOUT 50
//Milisegundos que el servidor espera del cliente cada vez que le pide una contestación

//Si la placa es una Mega, usamos el puerto serie 1. En otro caso usamos el serie por defecto.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#define PORT Serial1
#else
	#define PORT Serial
#endif

/*
	Módulo de comunicaciones del sistema Montefaro Lock V1.0

	Implementa el protocolo interno de comunicaciones para transmitir la información y los eventos de un enclavamiento entre los módulos
	a través del puerto serie.

	Datagramas:


*/

class mtfComm
{
public:

	enum statusType {
		error, //Fallo del enclavamiento
		disabled, //Circuito no activo
		iddle, //Sin ocupar
		locked, //Enclavado
		occupied, //Ocupado
		shunt //Maniobras
	};
	const uint8_t headerArray[HEADER_COUNT] = HEADER_ARRAY;	
	mtfComm();
	//Lado del servidor
	bool serverSendStatus(); //Envía tres datagramas de estado.
	void serverRequestClients(); //Solicita cambios a los clientes.
	bool serverGetClientMessage(); //Obtiene y procesa un mensaje recibido desde el cliente	
	void (*circuitChangedCallback)(uint8_t); //Puntero a función cada vez que cambia el estado de un circuito

	//Lado del cliente

	void dumpMem(); //Escribe el contenido del buffer en el puerto serie para depuración.
	void setCommand(uint8_t rhs);
	void setCircuit(uint16_t index, uint8_t layout, uint8_t status);
	uint8_t getCircuitLayout(uint16_t index);
	uint8_t getCircuitStatus(uint16_t index);
	void setSignal(uint16_t index, uint8_t order);
	uint8_t getSignalOrder(uint16_t index);	
protected:
	uint8_t mvarNumcircuits; //Número de circuitos
	uint8_t mvarNumSignals; //Número de señales
	uint8_t mcolCircuitLayout[MAX_ELEMENTS]; //Datagrama de configuración de circuitos
	uint8_t mcolCircuitStatus[MAX_ELEMENTS]; //Datagrama de estado de circuitos
	uint8_t mcolSignals[MAX_ELEMENTS]; //Datagrama de señales
	uint8_t mvarResponseBuffer[MAX_ELEMENTS]; //Buffer de recepción de datos



	uint8_t mcolBuffer[OUTPUT_LENGHT]; //Buffer de estado
	uint8_t mcolRequest[INPUT_LENGHT]; //Buffer de entrada
	uint8_t mcolDeviceStatus[NUM_CLIENTS]; //Lista negra de dispositivos (Número de reintentos)

	

	//Lado del servidor
	bool serverRequestClient(uint8_t clientId); //Interroga a un cliente.
	

	//Lado del cliente
	bool clientReceiveStatus(uint8_t* destination); //Recibe un mensaje de estado desde el cliente y lo almacena en el array de destino.




private:
	//Variables comunes a la instancia
	uint8_t mvarCommandId; //Comando leído (o enviado)
	uint8_t mvarHeaderData; //Tamaño de los elementos que va a leer (o enviar)
	uint8_t mvarCRC; //CRC leído (o enviado)

	//Iterador de estados
	uint8_t mvarStatusIterator;

	//Funciones comunes
	bool readHeader(); //Intenta leer la cabecera de un envío.
	bool sendHeader(); //Envío la cabecera de un nuevo paquete.



	//Lado del servidor
	bool serverSendCircuitLayout();
	bool serverSendCircuitStatus();
	bool serverSendSignals();
	
	
	bool serverGetClientChanges(); //Obtiene y procesa los cambios que ha tenido el cliente
	void serverProcessClientChange(uint8_t circuitId, uint8_t received); //Da curso al cambio recibido

	void lowByteBufferReport(uint8_t expected, uint8_t awaiting);

	uint8_t calculateCRC(*uint8_t buffer, uint8_t size); //Genera un CRC para el array especificado
	bool checkCRC(*uint8_t buffer, uint8_t size); //Comprueba la recepción correcta de un paquete con integridad CRC.
};

#endif