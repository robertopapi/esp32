//////////////////////////////////
// ftpserver esp32
//////////////////////////////////
#include "ftpserveresp32.h"

///////////////////////////////////////////
// definizione dei timer
///////////////////////////////////////////

#define LONGINTERVAL 1000 // intervallo che definisce il long click
#define GLITCHINTERVAL 100 // intervallo che definisce la durata minima del click per eliminare i glitch

// creo la struttura con i parametri e necessari a ciascun bottone
struct Button
{
  const uint8_t PIN;
  const functionPtr ISR;
  bool changed;
  bool pressed;
  unsigned long press_time;
  const functionPtr action_short;
  const functionPtr action_long;
};

// dichiaro che le funzioni a seguire vanno allocate in RAM
//void ICACHE_RAM_ATTR è per ESP8266 mentre per ESP32 serve IRAM_ATTR
void IRAM_ATTR isr_b_down();
void IRAM_ATTR isr_b_up();
void IRAM_ATTR isr_b_select();

// creo i singoli insiemi di parametri per ciascun bottone. short e long puntano alla medesima procedura
Button b_down = { 32, isr_b_down, false, false, 0, action_b_down, action_b_down};
Button b_up = { 33, isr_b_up, false, false, 0, action_b_up, action_b_up};
Button b_select = { 27, isr_b_select, false, false, 0, action_b_select, action_b_select};

////////////////////////////////
// isr_b_down
////////////////////////////////
void isr_b_down()
{
 b_down.changed = true;
}

////////////////////////////////
// isr_b_up
////////////////////////////////
void isr_b_up()
{
 b_up.changed = true;
}

////////////////////////////////
// isr_b_select
////////////////////////////////
void isr_b_select()
{
 b_select.changed = true;
}

////////////////////////////////
// init_button
////////////////////////////////
void init_button()
{
 pinMode(b_down.PIN, INPUT);
 attachInterrupt(b_down.PIN, b_down.ISR, CHANGE);
 
 pinMode(b_up.PIN, INPUT);
 attachInterrupt(b_up.PIN, b_up.ISR, CHANGE);
 
 pinMode(b_select.PIN, INPUT);
 attachInterrupt(b_select.PIN, b_select.ISR, CHANGE);

 defLogFile();
 writeLog("init_button() done");
 fsCurrentLog.close();
}

////////////////////////////////
// manage_button
////////////////////////////////
void manage_button()
{
 check_button(& b_down);
 check_button(& b_up);
 check_button(& b_select);
}

////////////////////////////////
// check_button
////////////////////////////////
void check_button(struct Button * b)
{
 unsigned long now_time = millis();
 functionPtr action;
 
 if (b->changed)
 {
  b->changed = false; // resetto prima di tutto il flag
#ifdef LOGDEBUG  
  msg = "check_button() button:";
  switch (b->PIN)
  {
   case 32:
    msg += "DOWN ";
    break;
   case 33:
    msg += "UP ";
    break;
   case 27:
    msg += "SELECT ";
    break;
  }
  Serial.println(msg + "(" + String(b->PIN) + ") status:" + String(digitalRead(b->PIN)));
#endif  
  if (displaystandby)
  {
   standby_reset(millis());
#ifdef LOGDEBUG 
   Serial.println(msg + "(" + String(b->PIN) + ") standby_reset");
#endif
   return; // esco con l'indicazione che la variazione non va gestita
  }
  
  if ((!(b->pressed)) && (!digitalRead(b->PIN))) // bottone premuto ma ancora non rilasciato
  {
   b->pressed = true;
   b->press_time = now_time;
#ifdef LOGDEBUG
   Serial.println(msg + "(" + String(b->PIN) + ") pressed=true");
#endif
   return;
  }
  
  if (b->pressed && b->press_time > 0 && (digitalRead(b->PIN))) // bottone premuto e rilasciato
  {
   if ((now_time - b->press_time) < GLITCHINTERVAL)
   {
#ifdef LOGDEBUG
    Serial.println(msg + "(" + String(b->PIN) + ") < GLITCHINTERVAL");
#endif    
    return;
   }
   
   if ((now_time - b->press_time) < LONGINTERVAL)
   {
#ifdef LOGDEBUG    
    Serial.println(msg + "(" + String(b->PIN) + ") RELEASED status:" + String(digitalRead(b->PIN)) + " isshort");
#endif
    action = b->action_short;
   }
   else
   {
#ifdef LOGDEBUG    
    Serial.println(msg + "(" + String(b->PIN) + ") RELEASED status:" + String(digitalRead(b->PIN)) + " islong");
#endif
    action = b->action_long;
   }
   
   // resetto tutti i flag ed eseguola funzione action corrispondente
   b->pressed = false;
   b->press_time = 0;

   standby_reset(now_time); // resetto il timer dello standby
   
   action();
  }
 }
}

////////////////////////////////
// action_b_up
////////////////////////////////
void action_b_up()
{
 if (currentRowPosition > 0)
 {
  if (currentRowPosition > 1)
  {
   currentRowPosition--;
   currentIdxItemSelected--;
  }
  show_menu();
 }
}

////////////////////////////////
// action_b_down
////////////////////////////////
void action_b_down()
{
 if (currentRowPosition > 0)
 {
  if (currentRowPosition < totalItem)
  {
   currentRowPosition++;
   currentIdxItemSelected++;
  }
  show_menu();
 }
}

////////////////////////////////
// action_b_select
////////////////////////////////
void action_b_select()
{
 functionPtr ptrMenu;
 
 ptrMenu = displayItem[currentIdxItemSelected].manager;
 ptrMenu();
}

///////////////////////////////////
// LED UPLOAD DATA
///////////////////////////////////
unsigned long ledFTPoperation_time=0;

void ledFTPoperation() // accende o spegne all'opposto dello stato della porta
{
 unsigned long now_time = millis();

 if (ledFTPoperation_time==0)
 {
  ledFTPoperation_time = now_time;
  if (digitalRead(LED_FTPOPERATION_PIN))
   ledFTPoperationON();
  else
  {
   digitalWrite(LED_FTPOPERATION_PIN,HIGH); // faccio direttamente il digitalwrite per evitare di resettare ledFTPoperation_time
//   Serial.println("LED OFF");
  }
 }
 
 if ((now_time - ledFTPoperation_time) >= LED_FTPOPERATION_FLASH)
  ledFTPoperation_time = 0;
}
void ledFTPoperationON() // forza l'accenzione del LED
{
 digitalWrite(LED_FTPOPERATION_PIN,LOW);
// Serial.println("LED ON");
}
void ledFTPoperationOFF() // forza lo spegnimento del LED
{
 digitalWrite(LED_FTPOPERATION_PIN,HIGH);
 ledFTPoperation_time=0;
// Serial.println("LED OFF");
}
