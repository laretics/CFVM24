#include "storage.h"
#include "webserver.h"
    storage almacen(4);
    webserver server(0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED,192,168,1,70);
 void setup()
 {
    Serial.begin(9600);
    while (!Serial)
    {
        delay(100);
        //Espera activa hasta tener conectado el puerto serie.        
    }
    if(!almacen.init(true)) while (true); //Bucle infinito
    Serial.println(F("Storage ready!"));
    if(!server.init(true)) while (true); //Bucle infinito    
 }

 void loop()
 {
    if(server.checkRequest(true))
    {
        //Aquí procesaremos la respuesta.
    }
    delay(10);
 }