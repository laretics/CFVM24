#include <SPI.h>
#include <Ethernet.h>
#define HTTP_PORT 80
class webserver
{
    public:
    webserver(unsigned char mac0,unsigned char mac1,unsigned char mac2,unsigned char mac3,unsigned char mac4,unsigned char mac5,
            unsigned char ip0,unsigned char ip1,unsigned char ip2,unsigned char ip3);
    bool init(bool debug);
    bool checkRequest(bool debug); //Inicia una exploración.

    char lastRequest[1024]; //Cadena con la última petición dirigida al servidor desde un cliente.
    private:
    void sendHttpResponse(EthernetClient* client); //Enviamos una respuesta Http a la petición.

    EthernetServer mvarServer;
    const IPAddress mvarLocalAddress;
    unsigned char mvarMac[6];
};