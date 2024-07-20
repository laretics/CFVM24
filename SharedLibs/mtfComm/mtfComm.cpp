#include "mtfComm.h"
mtfComm::mtfComm()
{
	for(uint16_t i = 0; i<HEADER_COUNT;i++) mcolBuffer[i]=headerArray[i];
	mcolBuffer[HEADER_COUNT]=1; //Comando de envío de estado.
}
bool mtfComm::serverSendStatus() 
{ 
	//Cada vez que llamamos a esta función vamos iterando
	//por cada tipo de información. En tres llamadas hemos enviado todo.
	switch (mvarStatusIterator)
	{
	case 0:
		mvarStatusIterator++;
		return serverSendCircuitLayout();
		break;
	case 1:
		mvarStatusIterator++;
		return serverSendCircuitStatus();
	
	default:
		mvarStatusIterator=0;
		return serverSendSignals();
		break;
	}
}

void mtfComm::serverRequestClients()
{
	for (uint8_t i=0;i<NUM_CLIENTS;i++)
	{
		if(mcolDeviceStatus[i]>0)
		{
			if(!serverRequestClient(i)) 
			{
				mcolDeviceStatus[i]-=1; //Este dispositivo no ha contestado.
			}
		}
	}
}

bool mtfComm::serverGetClientMessage()
{
	if(!readHeader()) //Error si no hay una cabecera válida
	{
		#ifdef DEBUG_SERIAL
			Serial.println(F("Invalid header sent from client"));
		#endif
		return false;
	}
	//Comprobamos que el comando sea el correcto
	switch (mvarCommandId)
	{
	case COMMAND_RECEIVE_CHANGES: //Es un mensaje de cambios
		return serverGetClientChanges();
		break;
	
	default:
		#ifdef DEBUG_SERIAL
			Serial.println(F("Wrong response from client."));
			Serial.print(F("Unmanaged message command "));
			Serial.print(mvarCommandId);
			Serial.println(F(" received."));
		#endif
		return false;	
		break;
	}
}





void mtfComm::dumpMem()
{
	Serial.println(F("Serial buffer content:"));
	Serial.println();
	Serial.print(F("\tCommand:"));Serial.print(mcolBuffer[HEADER_COUNT+1]);
	Serial.print(F(" Num of circuits "));Serial.print(NUM_CIRCUIT);
	Serial.print(F(" Num of signals "));Serial.println(NUM_SIGNAL);
	Serial.println();
	Serial.println(F("\tCircuits"));
	Serial.println(F("\tId\tStat\tLayout"));
	uint16_t auxIndex;
	for(uint16_t i=0;i<NUM_CIRCUIT;i++)
	{
		Serial.print(F("\t"));
		Serial.print(i);Serial.print(F("\t"));
		auxIndex = HEADER_COUNT+(i*2)+1;
		Serial.print(mcolBuffer[auxIndex]);Serial.print(F("\t"));
		Serial.print(mcolBuffer[auxIndex+1]);Serial.println();
	}
	Serial.println(F("\tSignals"));
	Serial.println(F("\tId\torder"));	
	for(uint16_t i=0;i<NUM_SIGNAL;i++)
	{
		Serial.print(F("\t"));
		Serial.print(i);Serial.print(F("\t"));
		auxIndex = HEADER_COUNT+(NUM_CIRCUIT*2)+i+1;
		Serial.print(mcolBuffer[auxIndex]);Serial.println();
	}
	Serial.println();
}
void mtfComm::setCommand(uint8_t rhs)
{
	mcolBuffer[HEADER_COUNT]=rhs;
}
void mtfComm::setCircuit(uint16_t index, uint8_t layout, uint8_t status)
{
	uint16_t auxIndex = HEADER_COUNT+(index*2)+1;
	mcolBuffer[auxIndex]=status;
	mcolBuffer[auxIndex+1]=layout;
}
uint8_t mtfComm::getCircuitLayout(uint16_t index)
{
	uint16_t auxIndex = HEADER_COUNT+(index*2)+2;
	return mcolBuffer[auxIndex];
}
uint8_t mtfComm::getCircuitStatus(uint16_t index)
{
	uint16_t auxIndex = HEADER_COUNT+(index*2)+1;
	return mcolBuffer[auxIndex];
}
void mtfComm::setSignal(uint16_t index, uint8_t order)
{
	uint16_t auxIndex = HEADER_COUNT+(NUM_CIRCUIT*2)+index+1;
	mcolBuffer[auxIndex] = order;
}
uint8_t mtfComm::getSignalOrder(uint16_t index)
{
	uint16_t auxIndex = HEADER_COUNT+(NUM_CIRCUIT*2)+index+1;
	return mcolBuffer[auxIndex];
}
bool mtfComm::serverRequestClient(uint8_t clientId)
{
	uint8_t tryouts = CLIENT_RECEIVE_TRYOUTS;
	//Envía una petición de información al cliente
	mvarCommandId = COMMAND_SEND_QUERY; //Petición de información
	mvarHeaderData = clientId; //...dirigida al cliente [clientId]
	if(!sendHeader()) return false; //Lo ha intentado y no ha podido.
	PORT.write(mvarCRC); //Enviamos código de autocomprobación. En este tipo de datagrama no hay más info.
	delay(100); //Damos tiempo a que el cliente conteste.

	while (0<tryouts)
	{
		if(PORT.available()>5) //Al menos tiene que tener header, command y CRC... 5 bytes.
		{
			if(serverGetClientChanges()) //Devuelve True en una recepción correcta, aunque sea sin cambios que reportar
			{
				return true;
			}
		}
		delay(CLIENT_RECEIVE_TIMEOUT); //Esperamos antes de hacer el próximo reintento.
		tryouts-=1;
	}
	return false; //No ha podido recibir nada del cliente
}

bool mtfComm::readHeader()
{	
	if(PORT.available>4)
	{
		//Comprobamos la cabecera
		mvarCRC = 0; //Iniciamos CRC.
		for(uint8_t i =0;i<HEADER_COUNT;i++)
		{
			if(headerArray[i]!=PORT.read())			
			{
				PORT.flush(); //Vacío lo que haya en el buffer para la próxima
				#ifdef DEBUG_SERIAL
					Serial.print(F("Error reading "));
					Serial.print(i);
					Serial.println(F(" header value on datagram sent."));
				#endif
				return false;
			}
			mvarCRC^=headerArray[i];
		}
		mvarCommandId = PORT.read();		
		mvarHeaderData = PORT.read();
		#ifdef DEBUG_SERIAL
			Serial.println("Header received.")
			Serial.print(F("Command Id: "));
			Serial.println(mvarCommandId);
			Serial.print(F("Header data: "));
			Serial.println(mvarHeaderData);
		#endif
		mvarCRC^=mvarCommandId;
		mvarCRC^=mvarHeaderData;
		return true;
	}
	return false; //No puedo empezar a leer porque no hay info suficiente.
}
bool mtfComm::sendHeader()
{
	if(PORT.availableForWrite()>4)
	{
		mvarCRC = 0; //Iniciamos CRC.
		PORT.write(headerArray,HEADER_COUNT);
		for(uint8_t i =0;i<HEADER_COUNT;i++) {mvarCRC^=headerArray[i];}
		PORT.write(mvarCommandId);
		mvarCRC^=mvarCommandId;
		PORT.write(mvarHeaderData);
		mvarCRC^=mvarHeaderData;
		#ifdef DEBUG_SERIAL
			Serial.println("Header sent.")
			Serial.print(F("Command Id: "));
			Serial.println(mvarCommandId);
			Serial.print(F("Header data: "));
			Serial.println(mvarHeaderData);
		#endif		
		return true;
	}
	return false;
}


bool mtfComm::serverSendCircuitLayout()
{
	mvarCommandId=COMMAND_SEND_CIRCUIT_LAYOUT;
	mvarHeaderData=mvarNumCircuits;
	//Enviamos cabecera
	if(sendHeader())
	{
		calculateCRC(&mcolCircuitLayout[0],mvarNumCircuits);
		//Enviamos configuración de los circuitos
		PORT.write(mcolCircuitLayout,mvarNumCircuits);
		PORT.write(mvarCRC);
		return true;
	}
	return false;
}
bool mtfComm::serverSendCircuitStatus()
{
	mvarCommandId=COMMAND_SEND_CIRCUIT_STATUS;
	mvarHeaderData=mvarNumCircuits;
	//Enviamos cabecera
	if(sendHeader())		
	{
		calculateCRC(&mcolCircuitStatus[0],mvarNumCircuits);
		PORT.write(mcolCircuitStatus,mvarNumCircuits);
		PORT.write(mvarCRC);
		return true;
	}		
	return false;
}
bool mtfComm::serverSendSignals()
{
	mvarCommandId=COMMAND_SEND_SIGNALS;
	mvarElementsSet=mvarNumSignals;
	//Enviamos cabecera
	if(sendHeader())
	{
		calculateCRC(&mcolSignals[0],mvarNumSignals);
		PORT.write(mcolSignals,mvarNumSignals);
		PORT.write(mvarCRC);
		return true;
	}				
	return false;
}



bool mtfComm::serverGetClientChanges()
{	
	//Hemos recibido la cabecera válida.
	#ifdef DEBUG_SERIAL
		Serial.print(F("Receiving "));
		Serial.print(mvarHeaderData);
		Serial.println(F(" circuit changes from client."));
	#endif
	uint8_t amount = PORT.available();
	//El número de bytes tiene que ser mvarHeaderData*2 + 1
	uint8_t minimum = (mvarHeaderData*2)+1;
	if(amount < minimum)
	{
		#ifdef DEBUG_SERIAL
			lowByteBufferReport(minimum,PORT.available());	
		#endif
		return false; //Nos salimos por no poder continuar.
	}
	//Leemos los datos en espera
	for(uint8_t i=0; i< amount; i++) {mcolRequest[i] = PORT.read();}

	//Comprobamos que el CRC es correcto
	for (uint8_t i=0;i<amount-1;i++) {mvarCRC^=mcolRequest[i];}

	if(mcolRequest[amount-1]!=mvarCRC)
	{
		//Error de comprobación de datos. No podemos procesar el paquete
		#ifdef DEBUG_SERIAL
			Serial.println(F("CRC error receiving changes from client."));
		#endif		
		return false;
	}

	//Procesamos la información
	uint8_t auxIndex=0;
	for(uint8_t i=0; i<mvarHeaderData;i++)
	{				
		serverProcessClientChange(mcolRequest[auxIndex++],mcolRequest[auxIndex++]);
	}
	return true; //Todo ha salido bien.
}
void mtfComm::serverProcessClientChange(uint8_t circuitId, uint8_t received)
{
	bool occupied = (received&128)!=0;
	uint8_t newLayout = received&127;
	bool changed = false;
	//Actualizando layout
	if(mcolCircuitLayout[circuitId]!=newLayout)
	{
		changed = true;
		mcolCircuitLayout[circuitId]=newLayout;
	}
	//Actualizando estado
	switch (mcolCircuitStatus[circuitId])
	{
	case /* constant-expression */:
		/* code */
		break;
	
	default:
		break;
	}

}

void mtfComm::lowByteBufferReport(uint8_t expected, uint8_t awaiting)
{
	Serial.println(F("Not enough data from sender."));
	Serial.print(F("Expecting at least "));
	Serial.print(expected);
	Serial.print(F(" bytes and awaiting only "));
	Serial.print(awaiting);
	Serial.println(F(" bytes at serial port."));	
}

void mtfComm::calculateCRC(*uint8_t buffer, uint8_t size)
{
	for(uint16_t i=0; i<(size-1);i++) mvarCRC^=buffer[i];
}

bool mtfComm::checkCRC(*uint8_t buffer, uint8_t size)
{
	//Comprobaremos la recepción correcta de un mensaje con el CRC.
	uint8_t auxCrc = calculateCRC(buffer, size);
	if(auxCrc!=buffer[size-1])
	{
		Serial.print(F("Bad CRC received: Expected "));
		Serial.print(auxCrc);
		Serial.print(F(" and received "));
		Serial.println(buffer[size-1]);
	}
	return auxCrc==buffer[size-1];
}
