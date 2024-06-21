#include "webserver.h"

webserver::webserver(unsigned char mac0,unsigned char mac1,unsigned char mac2,unsigned char mac3,unsigned char mac4,unsigned char mac5,
            unsigned char ip0,unsigned char ip1,unsigned char ip2,unsigned char ip3)
            :mvarLocalAddress(ip0,ip1,ip2,ip3),
            mvarServer(HTTP_PORT)
            {
                mvarMac[0] = mac0;
                mvarMac[1] = mac1;
                mvarMac[2] = mac2;
                mvarMac[3] = mac3;
                mvarMac[4] = mac4;
                mvarMac[5] = mac5;
            }

bool webserver::init(bool debug)
{
    Ethernet.begin(mvarMac,mvarLocalAddress);

    //Comprobamos que el hardware de ethernet está aquí.
    if(EthernetNoHardware== Ethernet.hardwareStatus())
    {
        Serial.println(F("Ethernet shield was not found. Can't run without hardware."));
        return false;
    }
    if(debug) {Serial.println(F("Ethernet shield present."));}
    if(LinkOFF==Ethernet.linkStatus())
    {
        Serial.println(F("Ethernet cable is not connected."));
        return false;
    }
    if(debug) {Serial.println(F("Ethernet cable connected."));}
    mvarServer.begin();
    if(debug)
    {
        Serial.print(F("Server is at "));
        Serial.println(Ethernet.localIP());
    }
    return true;
}

bool webserver::checkRequest(bool debug)
//Iniciamos una exploración para ver si algún cliente se ha conectado al servidor para
//pedir algo.
{
    int index = 0;
    EthernetClient cliente = mvarServer.available();
    if(cliente)
    {
        if(debug){Serial.println(F("New client"));}
        //Una petición http termina con una línea en blanco.
        bool currentLineIsBlank = true;
        while (cliente.connected())
        {
            if(cliente.available())
            {
                lastRequest[index] = cliente.read();
                if(debug){Serial.write(lastRequest[index]);}

                if('\n'==lastRequest[index] && currentLineIsBlank)
                {
                    sendHttpResponse(&cliente);
                    break;
                }
                if('\n'==lastRequest[index]){currentLineIsBlank=true;}
                else if('\r'!=lastRequest[index]){currentLineIsBlank=false;}
                index++;
            }         
        }
        //Damos tiempo al navegador del cliente para recibir los datos.
        delay(1);
        cliente.stop();
        if(debug){Serial.println(F("client disconnected"));}
    }
    return true;
}

void webserver::sendHttpResponse(EthernetClient* client)
{
    client->println(F("HTTP/1.1 200 OK"));
    client->println(F("Content-Type: text/html"));
    client->println(F("Connection: close")); //Cerramos la conexión al recibir la respuesta.
    client->println(F("Refresh: 2")); //El navegador debe refrescar la página cada 2 segundos.
    client->println();
    client->println(F("<!DOCTYPE HTML>"));
    client->println(F("<html>"));
    client->print(F("Testing web server: "));
    client->println(millis());
    client->println(F("<!html>"));    
}