#include <TimerOne.h>
#include "mtfComm.h"
#include "receiver.h"
#include <Wire.h>
#include <MultiFuncShield.h>

receiver rec = receiver();

enum recStatus
{
  iddle, //Modo de reposo por defecto
  mainMenu, //Menú de selección de opciones
  signalSelect, //Selección de señal
  pointSelect, //Selección de aguja
  signalDebug, //Mantenimiento de señal
  pointDebug //Mantenimiento de aguja
};
recStatus mvarCurrentStatus; //Estado actual
int mvarCurrentOptionIndex; //Índice de la opción actual
int mvarCurrentElementIndex; //índice del elemento actual de depuración

void setup(void)
{
  Serial.begin(9600);
  Serial1.begin(9600);
  TimerOne Timer1;
  MFS.initialize(&Timer1); //Inicia el shield
  MFS.write("_uep");
  MFS.beep(50,4,4);
  delay(2000);  
  mvarCurrentStatus=iddle;

}

void loop() {
  byte btn = MFS.getButton();
  if(btn)
  {
    byte buttonNumber = btn & B00111111;
    byte buttonAction = btn & B11000000;
    switch (mvarCurrentStatus)
    {
      case recStatus::mainMenu:
      if(BUTTON_PRESSED_IND==buttonAction)
      {
        if(3==buttonNumber)
        {
          MFS.beep(20);
          mvarCurrentOptionIndex++;
          if(mvarCurrentOptionIndex>2){mvarCurrentOptionIndex=2;}
          update();
        }
        else if(2==buttonNumber)
        {
          MFS.beep(20);
          mvarCurrentOptionIndex--;
          if(mvarCurrentOptionIndex<0){mvarCurrentOptionIndex=0;}
          update();
        }
        else if(1==buttonNumber)
        {
          //Modo seleccionado... vamos a seleccionar el elemento
          switch (mvarCurrentOptionIndex)
          {
          case 1:
            mvarCurrentStatus=recStatus::signalSelect;
            MFS.beep(40);
            break;
          case 2:
            mvarCurrentStatus=recStatus::pointSelect;
            MFS.beep(40);
            break;
          default:
            mvarCurrentStatus=recStatus::iddle;
            MFS.beep(60);            
            break;
          }          
          update();
          consumeKey();
          mvarCurrentElementIndex=0;          
        }
      }
      break;
      case recStatus::signalSelect:
        if(BUTTON_PRESSED_IND==buttonAction)
        {
          if(3==buttonNumber)
          {
            mvarCurrentElementIndex=1;
            mvarCurrentStatus = recStatus::signalDebug;
          }
          else if(2==buttonNumber)
          {
            mvarCurrentElementIndex=0;                 
            mvarCurrentStatus = recStatus::signalDebug;   
          } 
          else if(1==buttonNumber)
          {
            mvarCurrentStatus = recStatus::mainMenu;
          }
          MFS.beep();  
          update();       
          consumeKey();
        }
      break;
      case recStatus::pointSelect:
        if(BUTTON_PRESSED_IND==buttonAction)
        {      
          if(3==buttonNumber)
          {
            mvarCurrentElementIndex++;
            if(mvarCurrentElementIndex>3){mvarCurrentElementIndex=3;}
            MFS.beep();  
          }
          else if(2==buttonNumber)
          {
            mvarCurrentElementIndex--;          
            if(mvarCurrentElementIndex<0){mvarCurrentElementIndex=0;}
            MFS.beep();  
          }        
          update();
        }
        else if(BUTTON_1_LONG_PRESSED==btn)
        {
          mvarCurrentStatus = recStatus::pointDebug;
          update();
          consumeKey();
        }
      break;
      case recStatus::signalDebug:
        signalClient* auxClient;
        if(1==mvarCurrentElementIndex)
          auxClient=&rec.e12;
        else
          auxClient=&rec.e11;

        if(3==buttonNumber)
        {
          switch (auxClient->getOrder())
          {
            case signalClient::typeOrder::toViaLibre:
              auxClient->setOrder(signalClient::typeOrder::toAvisoDeParada);
              break;
            case signalClient::typeOrder::toAvisoDeParada:
              auxClient->setOrder(signalClient::typeOrder::toParada);
              break;                        
            case signalClient::typeOrder::toParada:
              auxClient->setOrder(signalClient::typeOrder::toRebaseAutorizado);
              break;
            case signalClient::typeOrder::toRebaseAutorizado:
              auxClient->setOrder(signalClient::typeOrder::toPrecaucion);
              break;                                      
            case signalClient::typeOrder::toPrecaucion:
              auxClient->setOrder(signalClient::typeOrder::toViaLibre);
              break;                    
          }
          MFS.beep();  
          update();
          consumeKey();
        }
        else if(2==buttonNumber)
        {
          switch (auxClient->getOrder())
          {
            case signalClient::typeOrder::toViaLibre:
              auxClient->setOrder(signalClient::typeOrder::toPrecaucion);
              break;
            case signalClient::typeOrder::toAvisoDeParada:
              auxClient->setOrder(signalClient::typeOrder::toViaLibre);
              break;                        
            case signalClient::typeOrder::toParada:
              auxClient->setOrder(signalClient::typeOrder::toAvisoDeParada);
              break;
            case signalClient::typeOrder::toRebaseAutorizado:
              auxClient->setOrder(signalClient::typeOrder::toParada);
              break;                                      
            case signalClient::typeOrder::toPrecaucion:
              auxClient->setOrder(signalClient::typeOrder::toRebaseAutorizado);
              break;                    
          }
          MFS.beep();  
          update();
          consumeKey();
        }
        else if(1==buttonNumber)
        {
          mvarCurrentStatus=recStatus::signalSelect;
          update();
          consumeKey();
        }
      break;
      case recStatus::pointDebug:

      break;
    
    default: //Iddle
      if(BUTTON_LONG_PRESSED_IND==buttonAction)
      {
        //Pasamos al menú de opciones
        if(1==buttonNumber)
        {
          MFS.beep(50);
          mvarCurrentStatus=recStatus::mainMenu;
          mvarCurrentOptionIndex=1;
          update();
        }
      }
      break;
    }


    if(BUTTON_PRESSED_IND == buttonAction)
    {

    }
    else if(BUTTON_LONG_PRESSED_IND ==buttonAction)
    {
      

    }
  }


  delay(25);
}

void consumeKey()
{
  while (MFS.getButton())
  {
    delay(10);
  }
  
}

void update()
{
  Serial.print(F("Modo "));
  Serial.print(mvarCurrentStatus);
  Serial.print(F(" / Option "));
  Serial.println(mvarCurrentOptionIndex);
  Serial.print(F(" / Item "));
  Serial.println(mvarCurrentElementIndex);
  //Muestra la salida de datos con los elementos actuales.
  switch (mvarCurrentStatus)  
  {
      case recStatus::mainMenu:
        switch (mvarCurrentOptionIndex)
        {
        case 1: //Señales
          MFS.write("snl ");
          /* code */
          break;
        case 2: //Agujas
          MFS.write(" poi");      
          break;
        default: //Sale del menú
          MFS.write("end");
          break;
        }
      break;
      case recStatus::signalSelect:
        switch (mvarCurrentElementIndex)        
        {
        case 0:
          MFS.write("Sn 1");
          break;
        case 1:
          MFS.write("Sn 2");
          break;                  
        default:
          MFS.write("----");
          break;
        }

      break;
      case recStatus::pointSelect:
        switch (mvarCurrentElementIndex)        
        {
        case 0:
          MFS.write("po 1");
          break;
        case 1:
          MFS.write("po 2");
          break;                  
        case 2:
          MFS.write("po 3");
          break;                  
        case 3:
          MFS.write("po 4");
          break;                                      
        default:
          MFS.write("----");
          break;
        }
      break;
      case recStatus::signalDebug:
        signalClient* auxClient;
        if(1==mvarCurrentElementIndex)
          auxClient=&rec.e12;
        else
          auxClient=&rec.e11;

        switch (auxClient->getOrder())
        {
          case signalClient::typeOrder::toViaLibre:
            MFS.write("libr");
            break;
          case signalClient::typeOrder::toParada:
            MFS.write("para");
            break;
          case signalClient::typeOrder::toAvisoDeParada:
            MFS.write("avis");
            break;          
          case signalClient::typeOrder::toPrecaucion:
            MFS.write("prec");
            break;                    
          case signalClient::typeOrder::toRebaseAutorizado:
            MFS.write("reba");
            break;                                      
          default:
            MFS.write("---");
            break;
        }
      break;
      case recStatus::pointDebug:

      break;
      default:

      break;
  }
}



