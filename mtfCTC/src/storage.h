#include <SPI.h>
#include <SD.h>

class storage
{
    private:
        //Variables de uso interno por la librería
        Sd2Card mvarCard;
        SdVolume mvarVolume;
        SdFile mvarRoot;
        const int mvarChipSelect;

        void debugCardType();
        void debugVolumeInfo();

    public:
        storage(const int chipSelect);
        boolean init(boolean debugInfo); //Inicialización del lector SD.


};

