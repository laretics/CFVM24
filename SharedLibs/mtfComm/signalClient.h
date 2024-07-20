#include<Arduino.h>
//Objeto señal luminosa en un enclavamiento
#ifndef _SIGNAL_CLIENT_
#define _SIGNAL_CLIENT_
class signalClient
{
    public:
        enum typeColor
        {
            tcGreen,
            tcRed,
            tcYellow,
            tcWhite
        };
        enum typeOrder
        {
            toViaLibre = 0,
            toParada = 1,
            toAvisoDeParada = 2,
            toPrecaucion = 3,
            toRebaseAutorizado = 4
        };
        signalClient();

        void init(); //Ajusta los sentidos de salida y prepara las señales para la primera visualización.
        void setInverted(bool red, bool green, bool yellow, bool white); //Ajusta el mapa de inversiones.
        void setDefaultOrder(typeOrder order);
        void setLedPort(typeColor color,uint8_t port);
        uint8_t getLedPort(typeColor color);
        uint16_t channel; //Número de canal (índice) en el array de señales que corresponde a esta señal.
        void setOrder(typeOrder order); //Asigna la orden (aquí se encienden o apagan las luces)
        typeOrder getOrder(); //Devuelve la última orden asignada

    protected:
        bool mcolInverted[4]; //Colección con las inversiones de los cuatro fuegos de una señal.
        uint8_t mcolLedPort[4]; //Colección con los cuatro fuegos de una señal.         
        typeOrder mvarOrder; //Última orden recibida
        typeOrder mvarDefaultOrder; //Orden por defecto, nada más iniciar la señal.

    private:
        void auxSetOrder(); //Asigna la última orden recibida
        void auxSetLight(typeColor color, bool rhs); //Asigna la luz.
};
#endif