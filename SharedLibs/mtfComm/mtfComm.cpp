#include "mtfComm.h"
mtfComm::mtfComm()
{
	for(uint16_t i = 0; i<HEADER_COUNT;i++) mcolBuffer[i]=headerArray[i];
}
void mtfComm::send()
{
	uint8_t auxCrc = calculateCRC();
	mcolBuffer[BUFFER_LENGHT-1]=auxCrc;
	PORT.write(mcolBuffer,BUFFER_LENGHT);
}
bool mtfComm::receive()
{
	if(PORT.available()<BUFFER_LENGHT) return false; //No hay datos suficientes.	
	PORT.readBytes(mcolBuffer,BUFFER_LENGHT);
	if(!syncHeader())
	{
		return false; //Error de cabecera
	} 
	uint8_t auxCrc = calculateCRC();
	#ifdef DEBUG_SERIAL
	if(auxCrc!=mcolBuffer[BUFFER_LENGHT])
	{
		Serial.print(F("Error: Bad CRC. (Expected "));
		Serial.print(auxCrc);
		Serial.print(F(" and read "));
		Serial.print(mcolBuffer[BUFFER_LENGHT-1]);
		Serial.println(F(" )."));
	}
	#endif	
	return auxCrc==mcolBuffer[BUFFER_LENGHT-1];
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
uint8_t mtfComm::calculateCRC()
{
	uint8_t crc = 0;
	for(uint16_t i=0; i<BUFFER_LENGHT-1;i++) crc^=mcolBuffer[i];
	return crc;
}
bool mtfComm::syncHeader()
{
	for(uint16_t i=0; i< HEADER_COUNT;i++)
	{
		if(headerArray[i]!=mcolBuffer[i])
		{
			#ifdef DEBUG_SERIAL
			Serial.print(F("Error: Bad header read. (Expected "));
			for(int16_t i=0;i<HEADER_COUNT;i++)
			{
				Serial.print(headerArray[i]);
				Serial.print(F(" "));
			}
			Serial.print(F("and read "));
			for(int16_t i=0;i<HEADER_COUNT;i++)
			{
				Serial.print(mcolBuffer[i]);
				Serial.print(F(" "));
			}
			Serial.println(F(")."));
			Serial.println();
			#endif			
			return false;
		}
	}
	return true;
}
