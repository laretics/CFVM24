//Interface de funciones vía pantalla táctil.
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <Hardwareserial.h>
#include <TouchScreen.h>
#include "mtfLock.h"
#include "definitions.h"

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin


/******************* UI details */
#define BUTTON_X 40
#define BUTTON_Y 100
#define BUTTON_W 59
#define BUTTON_H 30
#define BUTTON_SPACING_X 20
#define BUTTON_SPACING_Y 20
#define BUTTON_TEXTSIZE 2

// text box where numbers go
#define TEXT_X 8
#define TEXT_Y 4
#define TEXT_W 300
#define TEXT_H 58
#define TEXT_TSIZE 3
#define TEXT_TCOLOR ILI9341_MAGENTA
// the data (phone #) we store in the textfield
#define TEXT_LEN 12

//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920
// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65

#define MINPRESSURE 10
#define MAXPRESSURE 1000

class mInterface
{
  private:
    Elegoo_TFTLCD* mvarTft;
    mtfLock *mvarLock; //Referencia al bloqueo para poder pintarlo.
    TouchScreen *mvarTouchScreen; 
    Elegoo_GFX_Button mainButtons[12];
    uint8_t page; //Current selected page
    /* create 15 buttons, in classic candybar phone style */
    char buttonlabels[12][5] = {"Ent", "Sal", "Pas", "I1", "1", "2","3","4","Ter", "Lay", "Clr", "Send" };
    uint16_t buttoncolors[12] = { 
                                ILI9341_DARKGREEN, ILI9341_DARKGREEN, ILI9341_DARKGREEN, ILI9341_BLUE, 
                                ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE,
                                ILI9341_NAVY,ILI9341_NAVY, ILI9341_RED, ILI9341_DARKGREEN };    
            
  public:
    enum screenMode {
      mainMenu,
      layout
    };  
    mInterface(Elegoo_TFTLCD* rhs, mtfLock* lock, TouchScreen* touch);
    void init();
    void refresh();
    void refreshOutput(); //Refresca sólo el terminal de texto.
    void pp(const char* rhs); //Visualiza un texto en pantalla.
    bool keyScan(); //Hace una exploración de teclado devolviendo true si se ha pulsado una tecla.
    void tap(); //Ejecuta una pulsación.
    screenMode currentScreen;
    int16_t tapX; //Coordenada X del teclado
    int16_t tapY; //Coordenada Y del teclado

  protected:
    int8_t keyPressed(); //Saca la tecla pulsada del teclado.

  private:
    int16_t tftWidth,tftHeight;

    uint16_t make_test();
    void createKeyboards();
    void drawKeyboard(uint8_t page);
    void drawLayout(uint8_t screen);    
};