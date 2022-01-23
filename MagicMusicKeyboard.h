#ifndef MAGICMUSICKEYBOARD_H
#define MAGICMUSICKEYBOARD_H

#include "PS2Device.h"
#include <Arduino.h>

class MagicMusicKeyboard : public PS2Device
{
  public:
    MagicMusicKeyboard(uint8_t clockPin, uint8_t dataPin, uint8_t mainNoteOffset, uint8_t functionNoteOffset, uint8_t keypadNoteOffset);
    ~MagicMusicKeyboard();
    bool init();
    bool update();
    void sequencer();
    void MagicMusicKeyboard::sendnote(byte codebt);
    void MagicMusicKeyboard::seqclock();
    static void MagicMusicKeyboard::ClockOut16PPQN(uint32_t * tick);
    static void  MagicMusicKeyboard::ClockOut96PPQN(uint32_t * tick);
    static void MagicMusicKeyboard::onClockStart();
    static void MagicMusicKeyboard::onClockStop();
    static void MagicMusicKeyboard::mdelay(uint32_t us);
    static void noteonsend();
    static void MagicMusicKeyboard::sendmodewheel();
    static void MagicMusicKeyboard::noteoffroutine();
   static bool MagicMusicKeyboard::numLock;
   static bool MagicMusicKeyboard::chordMode;
   void MagicMusicKeyboard::seqmodeset();
    
  private:
    bool _isInitialized;
    static uint8_t MagicMusicKeyboard::noteOffset;
    uint8_t functionNoteOffset;
    uint8_t keypadNoteOffset;
    uint8_t _noteOffset;
    uint8_t _functionNoteOffset;
    uint8_t _keypadNoteOffset;
    static uint8_t MagicMusicKeyboard::octOffset;
    uint8_t cc;
    bool ccToggle;
    bool initReset();
    bool tryRead();
    bool processKey(uint8_t btCode, bool isPressed);
    bool modLCTRL;
    bool modALT;
    bool modLSHIFT;
    bool modALTGR;
    bool modRCTRL;
    bool modRSHIFT;
    bool modPGUP;
    bool modPGDN;
    bool modESC;
    bool capsLock;
    
    bool scrLock;
    void midiNoteOnMain(uint8_t note);
    void midiNoteOffMain(uint8_t note);
    void midiNoteOnFunction(uint8_t note);
    void midiNoteOffFunction(uint8_t note);
    void midiNoteOnKeypad(uint8_t note);
    void midiNoteOffKeypad(uint8_t note);
    void midiAllNotesOff();
    void midiCCToggle(uint8_t ch, uint8_t cc, bool ccToggle);
    void lightShow();
    void setLEDall();
    void setLEDoff();
    void setLEDcaps();
    void setLEDnumcaps();
    void setLEDcapsscr();
    void setLEDnumscr();
    void setLEDnum();
    void setLEDscr();
    void flashLED(int count);
    void setLED();
    void keyLayoutAdvanced();
    void keyLayoutMain();
    void holdNotes();
    void cDelay();




};

#endif //PS2KEYBOARD_H
