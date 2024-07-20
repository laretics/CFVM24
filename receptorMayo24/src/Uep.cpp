#include "Uep.h"

void Uep::init()
{
    TimerOne Timer1;
    MFS.initialize(&Timer1); //Inicia el shield
    MFS.write("_uep");
    MFS.beep(50,4,4);
    rec.init();
    delay(2000);  
    mvarCurrentStatus=iddle;
}



void Uep::scan()
{
  byte btn = MFS.getButton();
  if(btn)
  {
    byte buttonNumber = btn & B00111111;
    byte buttonAction = btn & B11000000;
    switch (mvarCurrentStatus)
    {
      case recStatus::mainMenu: processMainMenu(buttonNumber,buttonAction); break;
      case recStatus::signalSelect: processSignalSelect(buttonNumber,buttonAction); break;
      case recStatus::pointSelect: processPointSelect(buttonNumber,buttonAction); break;
      case recStatus::signalDebug: processSignalDebug(buttonNumber,buttonAction); break;
      case recStatus::pointDebug: break;
    
    default: //Iddle
      if(BUTTON_LONG_PRESSED_IND==buttonAction)
      {
        //Pasamos al menú de opciones
        if(1==buttonNumber)
        {
          jumpStatus(recStatus::mainMenu,50);
          mvarCurrentOptionIndex=1;
          update();
        }
      } 
      else if(BUTTON_PRESSED_IND == buttonAction)
      {

      }
      break;
    }



  }


  delay(25);    
}

void Uep::processMainMenu(uint8_t buttonNumber, uint8_t buttonAction)
{
  if(BUTTON_PRESSED_IND!=buttonAction)return;

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
      jumpStatus(recStatus::signalSelect,40);
      break;
    case 2:
      jumpStatus(recStatus::pointSelect,40);
      break;
    default:
      jumpStatus(recStatus::iddle,60);
      break;
    }          
    update();
    consumeKey();
    mvarCurrentElementIndex=0;          
  }
}

void Uep::processSignalSelect(uint8_t buttonNumber, uint8_t buttonAction)
{
  if(BUTTON_PRESSED_IND!=buttonAction) return;

  if(3==buttonNumber)
  {
    mvarCurrentElementIndex=1;
    jumpStatus(recStatus::signalDebug,50);       
  }
  else if(2==buttonNumber)
  {
    mvarCurrentElementIndex=0;                 
    jumpStatus(recStatus::signalDebug,50);       
  } 
  else if(1==buttonNumber)
  {
    mvarCurrentStatus = recStatus::mainMenu;
    jumpStatus(recStatus::mainMenu,50);       
  }
  update();       
  consumeKey();
}

void Uep::processPointSelect(uint8_t buttonNumber, uint8_t buttonAction)
{
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
  else if(BUTTON_1_LONG_PRESSED==buttonAction)
  {
    jumpStatus(recStatus::pointDebug,20);
    update();
    consumeKey();
  }
}

void Uep::processSignalDebug(uint8_t buttonNumber, uint8_t buttonAction)
{
  signalClient* auxClient;
  if(1==mvarCurrentElementIndex)
    auxClient=&rec.mcolSignals[0];
  else
    auxClient=&rec.mcolSignals[1];

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
    jumpStatus(recStatus::signalSelect,10);
    update();
    consumeKey();
  }
}

void Uep::update()
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
        MFS.writeLeds(LED_1,ON);
        MFS.writeLeds(LED_2|LED_3|LED_4,OFF);
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
        MFS.writeLeds(LED_2,ON);
        MFS.writeLeds(LED_1|LED_3|LED_4,OFF);      
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
        MFS.writeLeds(LED_3,ON);
        MFS.writeLeds(LED_1|LED_2|LED_4,OFF);                  
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
        MFS.writeLeds(LED_2,ON);
        MFS.writeLeds(LED_1|LED_3|LED_4,OFF);            
        signalClient* auxClient;
        if(1==mvarCurrentElementIndex)
          auxClient=&rec.mcolSignals[0];
        else
          auxClient=&rec.mcolSignals[1];

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
        MFS.writeLeds(LED_4,ON);
        MFS.blinkLeds(LED_4,ON);

      break;
  }
}

void Uep::consumeKey()
{
  while (MFS.getButton())
  {
    delay(10);
  }
  
}

void Uep::jumpStatus(Uep::recStatus destination, uint8_t buzzer)
{
  mvarCurrentStatus=destination;
  if(0!=buzzer)
    MFS.beep(buzzer);
}