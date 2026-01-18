#include "button.h"

/////////////////////////
// setStandbyAction()
/////////////////////////
void button::setStandbyAction(void (*_standbyaction)())
{
}

/////////////////////////
// check()
/////////////////////////
void button::check()
{
}

/////////////////////////
// begin()
/////////////////////////
void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)(), void (*_action_long)(), uint16_t _longinterval, uint8_t _glitchinterval);
{
}

void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)(), void (*_action_long)(), uint16_t _longinterval); // usa il default per il glitch
{
}

void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)(), uint8_t _glitchinterval);
{
}

void begin(uint_8 _pin, void (*_ISR)(), void (*_action_short)()); // usa il default per il glitch
{
}

/////////////////////////
// check()
/////////////////////////
void button::check()
{
}

/////////////////////////
// _init_button()
/////////////////////////
void button::_init_button()
{
}