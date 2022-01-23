#ifndef KEYTABLE_H
#define KEYTABLE_H

#include <Arduino.h>


extern  uint8_t g_standardKeyTable[];
extern uint8_t g_extendedKeyTable[];

class KeyTable
{
  public:
    static uint8_t getBTCode(uint8_t ps2ScanCode, bool isExtendedKey);
};

#endif  //KEYTABLE_H
