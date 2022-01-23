#include "PS2Device.h"
#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t _tmpSREG;
#define ATOMIC(X) _tmpSREG = SREG; cli(); X; SREG = _tmpSREG;










#ifdef __cplusplus
extern "C" {
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

uint32_t countPulseASM(volatile uint8_t *port, uint8_t bit, uint8_t stateMask, unsigned long maxloops);

#define EXTERNAL_INT_0 0
#define EXTERNAL_INT_1 1
#define EXTERNAL_INT_2 2
#define EXTERNAL_INT_3 3
#define EXTERNAL_INT_4 4
#define EXTERNAL_INT_5 5
#define EXTERNAL_INT_6 6
#define EXTERNAL_INT_7 7

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega128RFA1__) || defined(__AVR_ATmega256RFR2__) || \
    defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__)
#define EXTERNAL_NUM_INTERRUPTS 8
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define EXTERNAL_NUM_INTERRUPTS 3
#elif defined(__AVR_ATmega32U4__)
#define EXTERNAL_NUM_INTERRUPTS 5
#else
#define EXTERNAL_NUM_INTERRUPTS 2
#endif

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif


















PS2Device::PS2Device(uint8_t clockPin, uint8_t dataPin)
{
  _clockPin = clockPin;
  _dataPin = dataPin;
  gohi(_clockPin);
  gohi(_dataPin);
}

PS2Device::~PS2Device()
{
}



void PS2Device::mdelayMicroseconds(uint32_t us) {
  uint32_t tmr = micros();
  while (micros() - tmr < us);
}



bool PS2Device::write(uint8_t data, bool enableTimeout)
{
  TIMSK1  &= ~(1 << OCIE1A);
  bool parity = true;


  gohi(_dataPin);



  gohi(_clockPin);


  PS2Device::mdelayMicroseconds(148);


  golo(_clockPin);


  PS2Device::mdelayMicroseconds(149);


  golo(_dataPin);


  PS2Device::mdelayMicroseconds(5);

  gohi(_clockPin);

  //wait for clock lo
  switch (enableTimeout) {
    case true:


      startTimeoutCtr();
      // while (digitalRead(_clockPin) == HIGH && !hasTimedOut(TIMEOUT_WRITE_WAIT)); //busy wait

      cbi(TCCR2A, COM2B1);
      while (((PIND & B00001000) == 8) && !hasTimedOut(TIMEOUT_WRITE_WAIT));
      sbi(TCCR2A, COM2B1);


      if (_timedOut)
        return false;
      break;
    case false:
      // while (digitalRead(_clockPin) == HIGH); //busy wait

      cbi(TCCR2A, COM2B1);
      while (((PIND & B00001000) == 8));
      sbi(TCCR2A, COM2B1);
      break;
  }

  //data bits
  for (uint8_t i = 0; i < 8; i++)
  {

    //    switch (data & (1 << i)) {
    //      case true:
    //        parity = !parity;
    //
    //        gohi(_dataPin);
    //        break;
    //      case false:
    //        golo(_dataPin);
    //        break;
    //
    //    }


    if (data & (1 << i))
    {
      parity = !parity;

      gohi(_dataPin);

    }
    else
    {

      golo(_dataPin);

    }

    //clock cycle

    cbi(TCCR2A, COM2B1);
    while (((PIND & B00001000) != 8));
    sbi(TCCR2A, COM2B1);


    //while (digitalRead(_clockPin) == HIGH);
    cbi(TCCR2A, COM2B1);
    while (((PIND & B00001000) == 8));
    sbi(TCCR2A, COM2B1);

  }

  //parity bit
  switch (parity) {
    case true:
      gohi(_dataPin);
      break;
    case false:
      golo(_dataPin);
      break;
  }
  //  if (parity) {
  //
  //    gohi(_dataPin);
  //  } else {
  //
  //
  //    golo(_dataPin);
  //  }

  //clock cycle

  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) != 8));
  sbi(TCCR2A, COM2B1);



  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) == 8));
  sbi(TCCR2A, COM2B1);


  //stop bit
  PS2Device::mdelayMicroseconds(31);

  gohi(_dataPin);


  //ack

  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) == 8));
  sbi(TCCR2A, COM2B1);

  //mode switch

  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) != 8) || ((PIND & B00000100) != 4));
  sbi(TCCR2A, COM2B1);


  //hold up incoming data

  golo(_clockPin);

  TIMSK1 |= (1 << OCIE1A);
  // PS2Device::mdelayMicroseconds(60);
  return true;

}

bool PS2Device::read(uint8_t * data, bool enableTimeout)
{
  //TIMSK1  &= ~(1 << OCIE1A);
  //clear data
  *data = 0;

  gohi(_dataPin);


  switch (enableTimeout) {
    case true:
      startTimeoutCtr();
      gohi(_clockPin);

      //test
      //delayMicroseconds(50);
      cbi(TCCR2A, COM2B1);
      while (((PIND & B00001000) == 8)  && !hasTimedOut(TIMEOUT_READ_WAIT)); //busy wait
      sbi(TCCR2A, COM2B1);

      switch (_timedOut) {
        case true:
          //hold up incoming data
          golo(_clockPin);
          return false;
          break;
      }
   
      break;

    case false:
      gohi(_clockPin);
      cbi(TCCR2A, COM2B1);
      while (((PIND & B00001000) == 8))
        sbi(TCCR2A, COM2B1);
      ; //busy wait
      break;
  }


  //start bit
  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) != 8));
  sbi(TCCR2A, COM2B1);

  //data bits
  for (uint8_t i = 0; i < 8; i++)
  {
    //wait for clock lo
    cbi(TCCR2A, COM2B1);
    while (((PIND & B00001000) == 8));
    sbi(TCCR2A, COM2B1);

    if (((PIND & B00000100) == 4) && data != NULL)
    {
      *data |= (1 << i);
    }

    //wait for clock hi
    cbi(TCCR2A, COM2B1);
    while (((PIND & B00001000) != 8));
    sbi(TCCR2A, COM2B1);
  }

  //parity bit (ignore it)
  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) == 8));
  sbi(TCCR2A, COM2B1);
  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) != 8));
  sbi(TCCR2A, COM2B1);

  //stop bit
  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) == 8));
  sbi(TCCR2A, COM2B1);

  cbi(TCCR2A, COM2B1);
  while (((PIND & B00001000) != 8));
  sbi(TCCR2A, COM2B1);


  //hold incoming data
  golo(_clockPin);
  //TIMSK1 |= (1 << OCIE1A);
  return true;

}

void PS2Device::golo(uint8_t pin)
{

  DDRD |=  (1 << pin);
  PORTD &= ~(1 << pin);



  // pinMode(pin, OUTPUT);


  //digitalWrite(pin, LOW);





  //if (pin == 2){
  //  // pinMode(pin, OUTPUT);
  //   DDRD =  B00000100;
  //  //digitalWrite(pin, LOW);
  //  PORTD = B00000000;
  //  }
  //  if (pin == 3){
  //    DDRD =  B00001000;
  // PORTD = B00000000;
  //  }



}

void PS2Device::gohi(uint8_t pin)
{

  DDRD &= ~(1 << pin);
  PORTD |= 1 << pin;


  // pinMode(pin, INPUT);


  //digitalWrite(pin, HIGH);

  //  if (pin == 2){
  //  // pinMode(pin, OUTPUT);
  //   DDRD =  B00000000;
  //  //digitalWrite(pin, LOW);
  //  PORTD = B00000100;
  //
  //  }
  //  if (pin == 3){
  //    DDRD =  B00000000;
  // PORTD = B00001000;
  //  }
}

void PS2Device::startTimeoutCtr()
{
  _timeoutStartCtr = micros();
  _timedOut = false;
}

bool PS2Device::hasTimedOut(uint64_t usecsToTimeOut)
{
  uint64_t deltaTime = micros() - _timeoutStartCtr;
  if (deltaTime > usecsToTimeOut)
  {
    _timedOut = true;
    return true;
  }

  return false;
}
