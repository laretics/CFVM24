#include "storage.h"

storage::storage(const int chipSelect):mvarChipSelect(chipSelect){}
boolean storage::init(boolean debugInfo)
{
    if(debugInfo)
    {
        Serial.print(F("Initializing SD card... "));
    }    
    if(!mvarCard.init(SPI_HALF_SPEED,mvarChipSelect))
    {
        Serial.println(F("Initialization FAILED."));
        Serial.println(F("Check if is a card inserted, if your wiring is correct or if chipSelect value matches your shield or module."));        
        return false;
        
            /* code */
    }
    if(debugInfo)
    {    
        Serial.println(F("Wiring is correct and card is present."));
        debugCardType();
    }    
    if(!mvarVolume.init(mvarCard))
    {
        Serial.println(F("Could not find FAT16/FAT32 partition. \nMake sure you've formatted the card"));
        return false;
    }
    if(debugInfo)
    {
        debugVolumeInfo();
    }
    return true;
}

void storage::debugCardType()
{
    Serial.print(F("Card type: "));
    switch (mvarCard.type())
    {
    case SD_CARD_TYPE_SD1:
        Serial.println(F("SD1"));
        break;
    case SD_CARD_TYPE_SD2:
        Serial.println(F("SD2"));
        break;
    case SD_CARD_TYPE_SDHC:
        Serial.println(F("SDHC"));
        break;            
    default:
        Serial.println(F("Unknown"));
        break;
    }
}

void storage::debugVolumeInfo()
{
    Serial.print(F("Clusters:           "));
    Serial.println(mvarVolume.clusterCount());
    Serial.print(F("Blocks per Cluster: "));
    Serial.println(mvarVolume.blocksPerCluster());
    Serial.print(F("Total Blocks        "));
    Serial.println(mvarVolume.blocksPerCluster()*mvarVolume.clusterCount());
    Serial.println();

    uint32_t volumeSize;
    Serial.print(F("Volume type is:  FAT"));
    Serial.println(mvarVolume.fatType(),DEC);

    volumeSize = mvarVolume.blocksPerCluster(); //Un cluster es una colección de bloques
    volumeSize *= mvarVolume.clusterCount();
    volumeSize /=2; //En las tarjetas SD, los bloques son siempre de 512 bytes (cada dos bloques son 1Kb)

    Serial.print(F("Volume size (KB):  "));
    Serial.println(volumeSize);
    Serial.print(F("Volume size (MB):  "));
    volumeSize/=1024;
    Serial.println(volumeSize);
    Serial.print(F("Volume size (GB):  "));    
    Serial.println((float)volumeSize/1024.0);

    Serial.println(F("\nFiles found on the card (name, date and size in bytes): "));
    mvarRoot.openRoot(mvarVolume);

    mvarRoot.ls(LS_R|LS_DATE|LS_SIZE);
    mvarRoot.close();
}