/*
 *
 *
 */

// ensure this library description is only included once
#ifndef button_h
#define button_h

class button
{
 public:
  bool standby = false; // serve ad indicare che deve essere eseguita la procedura corrispondente al posto delle normali action
 
  void setStandbyAction(void (*_standbyactionparam)()) // corrisponde all'azione alternativa da eseguire se ci troviamo nello stato di standby
  {
   _standbyaction = _standbyactionparam;
   _managementstandby = true;
  }
  
  
  void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)(), void (*_action_long)(), uint16_t _longinterval, uint8_t _glitchinterval);
  void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)(), void (*_action_long)(), uint16_t _longinterval); // usa il default per il glitch
  void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)(), uint8_t _glitchinterval);
  void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)()); // usa il default per il glitch

  void check(); 
  
 private:
  bool _changed = false;
  bool _pressed = false;
  uint32_t press_time = 0;
  
  void (*_standbyaction)();
  void (*_ISR)();
  void (*_action_short)();
  void (*_action_long)();
  
  bool _managementstandby = false; // indica se lo standby va gestito ed è attivato da setStandbyAction
  
  void _init_button(); // viene eseguito chiamato dalla funzione begin e serve per impostare l'accesso del PIN e attivare l'interrupt
};

#endif
