//////////////////////////////////
// ftpserver esp32
//////////////////////////////////
#include "ftpserveresp32.h"

void clear_display()
{
 u8g2.clearDisplay();
 u8g2.clearBuffer();
 u8g2.setFontMode(1);  // activate transparent font mode
 u8g2.setDrawColor(2);
 u8g2.setFont(SMALLFONT);
}

void init_display()
{
// impostazioni per il display
 u8g2.begin();
}

// "cancella" dal display n righe a partire dalla riga start, considerando l'altezza del font SMALLFONT
// la funzione è dichiarata due volte per gestire come default il terzo parametro
void clear_row(uint8_t start, uint8_t n)
{
 draw_box(start, n, 0);
}

void draw_box(uint8_t start, uint8_t n, uint8_t boxColor)
{
// u8g2.clearBuffer();
 u8g2.setDrawColor(boxColor);
 u8g2.drawBox(0, ((u8g2.getMaxCharHeight()*start)-u8g2.getMaxCharHeight()), u8g2.getDisplayWidth(), ((u8g2.getMaxCharHeight()*n)+1));

 u8g2.setDrawColor(2); // ripristina il colore
}

void draw_frame(uint8_t start)
{
 draw_frame(start, 1, 1);
}

void draw_frame(uint8_t start, uint8_t n, uint8_t frameColor)
{
 u8g2.setDrawColor(frameColor);

 u8g2.drawFrame(0, ((u8g2.getMaxCharHeight()*start)-u8g2.getMaxCharHeight()), u8g2.getDisplayWidth(), ((u8g2.getMaxCharHeight()*n)+1));

 u8g2.setDrawColor(2); // ripristina il colore  
}

void dispmsg(String sx, String sy, String s)
{
 static char buf[100]; // alloco un buffer

// Serial.println(s); // invio il messaggio anche sulla seriale

 u8g2_uint_t dw;
 u8g2_uint_t sw;
 u8g2_uint_t dh;
 u8g2_uint_t sh;
 u8g2_uint_t x;
 u8g2_uint_t y;

 if (s=="")
  return; // esco subito se la stringa è vuota

 s.toCharArray(buf, 100);

 sw = u8g2.getUTF8Width(buf); // permette la stampa di caratteri accentati
 dw = u8g2.getDisplayWidth();

 sh = u8g2.getMaxCharHeight();
 dh = u8g2.getDisplayHeight();

 if (sx == "left")
  x = 0;
 else if (sx == "right")
  x=(sw >= dw)?0:(dw - sw);
 else if (sx == "center")
  x=(sw >= dw)?0:((dw - sw) / 2);
 else if (sx.toInt() >= 0)
  x = sx.toInt();

 if (sy == "top")
  y = sh;
 else if (sy == "bottom")
  y = dh;
 else if (sy == "center")
  y=(sh >= dh)?dh:(((dh - sh) / 2) + sh);
 else if (sy.toInt() >= 0)
  y = sy.toInt();

 u8g2.drawUTF8(x, y, buf); // permette la stampa di caratteri accentati
}

void check_standby()
{
 volatile unsigned long now_time = millis();

 if ( !(displaystandby) ) // display acceso
 {
  if (now_time - displaystandby_time > standbyInterval)
  {
   displaystandby=true;
   displaystandby_time=0;
   displayoff();
  }
 }
}

void standby_reset(unsigned long t)
{
 displaystandby=false;
 displaystandby_time=t;
 displayon();
}

void displayon()
{
 u8g2.setPowerSave(0);
}

void displayoff()
{
 u8g2.setPowerSave(1);
}
