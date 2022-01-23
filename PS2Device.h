#ifndef PS2DEVICE_H
#define PS2DEVICE_H

#include <Arduino.h>

#define TIMEOUT_WRITE_WAIT 5000
#define TIMEOUT_READ_WAIT 5000

class PS2Device
{
  public:
    PS2Device(uint8_t clockPin, uint8_t dataPin);
    virtual ~PS2Device();

  protected:
    bool write(uint8_t data, bool enableTimeout);
    bool read(uint8_t * data, bool enableTimeout);
    void golo(uint8_t pin);
    void gohi(uint8_t pin);
    uint8_t _clockPin;
    uint8_t _dataPin;

  private:
    void startTimeoutCtr();
    bool hasTimedOut(uint64_t usecsToTimeOut);
    uint64_t _timeoutStartCtr;
    bool _timedOut;
    void PS2Device::mdelayMicroseconds(uint32_t us);
};


#endif //PS2DEVICE_H
