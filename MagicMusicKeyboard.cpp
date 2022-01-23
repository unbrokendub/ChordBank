
#include "global.h"
#include "MagicMusicKeyboard.h"
#include "BTKeyCodes.h"
#include "KeyTable.h"

#include "MIDI.h"

#include "uClock.h"



#define MIDI_STRUM_CHANNEL 3
#define MIDI_BLOCK_CHORD_CHANNEL 1

#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_STOP  0xFC
//PS2 keycodes:
//http://www.computer-engineering.org/ps2keyboard/scancodes3.html

static bool MagicMusicKeyboard::numLock = false;  // chromatic scales mode

int nolen = 4;  //note lentgth in sequncer mode
////MIDI defines

int seqmode = 0;
int scounter; // internal sequncer counter for notes
int vscounter; // internal sequencer counter for velocity
int mscounter = 0; // internal modulation wheel sequencer counter
//int tick = 0 ;
int CC_STATUS_MAIN = 0xB0;

const int NOTE_VELOCITY_MAIN = 70;

bool sequencerOn = false;  // sequencer mode On/Off

int SUSTAIN = 0x40;

//unsigned long timing;  // TIMER FOR DELAY FOR COUNTING BPM
//unsigned long timing2;


const int flashDelay = 30;
const int flashLong = 30;

bool noteoffsended = true; // internal sequencer parametr to for catching when to off the note


bool chordnoteenabled = false;

int bpm = 122;  // BPM for sequencer
//uint32_t seqnotelen = 80000;
static uint8_t MagicMusicKeyboard::octOffset;  //Offset octave
static uint8_t MagicMusicKeyboard::noteOffset = 0; // Note offset

byte enabledNotes[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // For Hold Notes Mode  ( CAPS LOCK )

byte seqnotes[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // sequencer notes storage
byte seqmodwheel[64];  // sequencer modulation wheel parametr storage
byte velocityseq[64];  // sequencer velocity parametr storage
int seqwritemode = 1;  //by default we write notes into sequence. 2 - velocity. 3 -  mod wheel
int veloseqcount = 0; // velocity sequencer parametr counter
int modseqcount = 0; // modulation wheel sequencer parametr counter



int seqcount = 0;  // notes sequencer parametr counter

int enabledNotesCounter = 0; // this is for Hold Notes Mode. How many notes are enabled
int enabledNotesCounteri = 0;

bool enabledNote = false;   // Need to catch enabled note when overflow int Hold Mode

MIDI_CREATE_DEFAULT_INSTANCE();

int spreadcounter = 0; // Spread notes in chord mode

int chordbankset = 0; // select chord bank bank

int MAXIMUM_CHORD_SIZE = 5;

int scaleset = 0;  // chromatic scale set


static bool MagicMusicKeyboard::chordMode = false; // Chord Mode On/Off (SCROLL LOCK)

int chordSize = 4;




//int enabledchordnotes[5] = {0, 0, 0, 0, 0};


const byte spread[][4] = {
  {0, 0, 0, 0} ,
  {0, 0, -12, 0},
  {12, 0, 0, 0} ,
  {0, -12, 0},
  {0, 12, 0, 0},
  { -12, 0, 12, 0} ,
  { -12, 0, 0, 0} ,
  {0, 0, 12, 0} ,
  { -12, -12, 12, 0} ,
  { -12, 12, 12, 0} ,
  { -24, 0, 24, 0} ,
  { -24, 0, 12, 0} ,
  { -12, 0, 24, 0} ,
  { -24, -12, 24, 0} ,
  { -24, 12, 24, 0} ,
  { -36, 0, 36, 0} ,
  { -36, 0, 24, 0} ,
  { -24, 0, 36, 0} ,
  { -12, -36, 36, 0},
  { -12, -24, 36, 0} ,
  { -12, -12, 36, 0} ,
  { -12, 0, 36, 0} ,
  { -12, 12, 36, 0} ,
  { -12, 24, 36, 0},
  { -12, 36, 36, 0},
  { -36, -36, 36, 0},
  { -24, -36, 36, 0} ,
  { -24, -24, 36, 0} ,
  { -24, -12, 36, 0} ,
  { -24, 0, 36, 0} ,
  { -24, 12, 36, 0} ,
  { -24, 24, 36, 0} ,
  { -24, 36, 36, 0},
  { -24, -36, 36, 0},
  {0, -36, 36, 0} ,
  {0, -24, 36, 0} ,
  {0, -12, 36, 0} ,
  {0, 0, 36, 0} ,
  {0, 12, 36, 0} ,
  {0, 24, 36, 0} ,
  {0, 36, 36, 0} ,
  {0, -36, 36, 0} ,
  {12, -36, 36, 0} ,
  {12, -24, 36, 0},
  {12, -12, 36, 0} ,
  {12, 0, 36, 0} ,
  {12, 12, 36, 0} ,
  {12, 24, 36, 0} ,
  {12, 36, 36, 0} ,
  {12, -36, 36, 0},
  {24, -36, 36, 0} ,
  {24, -24, 36, 0} ,
  {24, -12, 36, 0} ,
  {24, 0, 36, 0} ,
  {24, 12, 36, 0} ,
  {24, 24, 36, 0} ,
  {24, 36, 36, 0} ,
  {24, -36, 36, 0},
  { -36, -12, 36, 0},
  { -36, -36, 36, 0} ,
  { -36, -36, 36, 0} ,
  { -36, -24, 36, 0} ,
  { -36, -12, 36, 0} ,
  { -36, 0, 36, 0} ,
  { -36, 12, 36, 0},
  { -36, 24, 36, 0},
  { -36, 36, 36, 0} ,
  {0, -36, 0, 0} ,
  {0, -24, 0, 0} ,
  {0, 24, 0, 0} ,
  {0, 36, 0, 0}

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////START PART FROM CHORDTOY {{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{












//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}CLOSE PART FROM CHORDTOY/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








MagicMusicKeyboard::MagicMusicKeyboard(uint8_t clockPin, uint8_t dataPin, uint8_t noteOffset, uint8_t functionNoteOffset, uint8_t keypadNoteOffset)
  : PS2Device(clockPin, dataPin)
{
  _isInitialized = false;
  _noteOffset = noteOffset;
  _functionNoteOffset = functionNoteOffset;
  _keypadNoteOffset = keypadNoteOffset;
}

MagicMusicKeyboard::~MagicMusicKeyboard()
{
}

bool MagicMusicKeyboard::init()
{
  MagicMusicKeyboard::mdelay(500);
  bool resetStatus = initReset();
  if (!resetStatus)
    return false;

  // MagicMusicKeyboard::mdelay(1);

  _isInitialized = true;
  bool modLCTRL = false;
  bool modLALT = false;
  bool modLSHIFT = false;
  bool modALTGR = false;
  bool modRCTRL = false;
  bool modRSHIFT = false;
  bool modPGUP = false;
  bool modPGDN = false;
  bool modESC = false;
  bool capsLock = false;

  bool scrLock = false;

  for (int i = 0; i < 64; i++) {
    velocityseq[i] = 100;
  }

  uClock.init();
  uClock.setClock16PPQNOutput(ClockOut16PPQN);
  uClock.setClock96PPQNOutput(ClockOut96PPQN);
  uClock.setOnClockStartOutput(onClockStart);
  uClock.setOnClockStopOutput(onClockStop);
  uClock.setTempo(bpm);





  lightShow();





  return true;




  MIDI.begin(1);



}

bool MagicMusicKeyboard::initReset()
{
  bool resetStatus = write(0xff, true);  // Reset signal - 0xFF

  if (!resetStatus)
  {
    return false;
  }

  read(0xFA, false);  // Acknowledge - 0xFA

  // Self-test result, 0xAA00 when OK, 0xFC00 on error
  uint8_t result = 0x00;
  read(&result, false);

  if (result != 0xAA)
  {
    return false;
  }

  write(0xF0, false);  //select scan code set
  read(NULL, false);  // Acknowledge - 0xFA
  write(0x03, false);  //scan code set 3
  read(NULL, false);  // Acknowledge - 0xFA

  write(0xf8, false);  //Set all keys Make/Break (disables typmatic repeat)
  read(NULL, false);  // Acknowledge - 0xFA

  return true;
}

bool MagicMusicKeyboard::update()
{
  switch (_isInitialized) {
    case false:
      return false;
      break;
  }

  switch (tryRead()) {
    case true:
      return true;
      break;
  }

  return false;
}

bool MagicMusicKeyboard::tryRead()
{


  //MagicMusicKeyboard::mdelay(15);

  uint8_t keyCode;
  uint8_t btCode = BTKC_NONE;
  bool pressed = false;
  read(&keyCode, true);
  switch (keyCode) {

    //Read the rest of the incoming bytes with blocking

    case 0x00:

      //do nothing
      return false;
      break;
    case 0xF0: //a standard key released

      read(&keyCode, false);
      btCode = KeyTable::getBTCode(keyCode, false);
      pressed = false;
      break;
    case 0xE0: //an extended key

      read(&keyCode, false);
      switch (keyCode) {


        case 0xF0: //an extended key released

          read(&keyCode, false);

          //The one exception to this case is Print Screen
          // Make: E0,12,E0,7C
          // Break: E0,F0,7C,E0,F0,12
          // So hardcode the case where 7C is read
          switch (keyCode) {
            case 0x7C:
              keyCode = 0x12;
              //skip over remaining 3 bytes
              read(NULL, false);
              read(NULL, false);
              read(NULL, false);
              break;
          }


          btCode = KeyTable::getBTCode(keyCode, true);
          pressed = false;
          break;

        //The one exception to this case is Print Screen
        // Make: E0,12,E0,7C
        // Break: E0,F0,7C,E0,F0,12
        case 0x12:

          //skip over remaining 2 bytes
          read(NULL, false);
          read(NULL, false);
          break;

        default: //an extended key pressed

          btCode = KeyTable::getBTCode(keyCode, true);
          pressed = true;
          break;
      }
      break;
    case 0xE1:

      //There's only 1 E1 extension in the table of scancodes (PAUSE key)
      // (there's no release code for this key either)
      btCode = BTKC_PAUSE;
      pressed = true;

      //skip over remaning 7 bytes
      read(NULL, false);
      read(NULL, false);
      read(NULL, false);
      read(NULL, false);
      read(NULL, false);
      read(NULL, false);
      read(NULL, false);
      break;
    default:
      btCode = KeyTable::getBTCode(keyCode, false);
      pressed = true;
      break;



  }
  return processKey(btCode, pressed);
  return false;
}

bool MagicMusicKeyboard::processKey(uint8_t btCode, bool isPressed)
{
  bool stateUpdated = false;



  ///////////////////////////////////////////////////////////////////////
  ////////////////OCTAVE SHIFT WITH NUMPAD + AND - //////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////
  switch (btCode) {
    case BTKC_KEYPAD_SUBTRACT:          /////////////////////
      switch (isPressed) {
        case true:
          ATOMIC( octOffset = octOffset - 12);
          MIDI.sendControlChange(123, 0, 1);
          break;
      }
      /////////////////
      break;                                                      /////////////
    /////////////
    case BTKC_KEYPAD_ADD:              ///////////////////

      switch (isPressed) {
        case true: //////////////////
          ATOMIC(  octOffset = octOffset + 12);
          MIDI.sendControlChange(123, 0, 1);
          break;
          // midiAllNotesOff();
      }////////////////////
      break;                                                   //////////////////////
    ////////////////////
    ///////////////////////////////////////////////////////////////////////
    ////////////////OCTAVE SHIFT WITH NUMPAD + AND - //////////////////////
    ///////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////
    ////////////////NOTE SHIFT WITH NUMPAD + AND - //////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////
    case BTKC_KEYPAD_DIVIDE:          /////////////////////

      switch (isPressed) {
        case true:
          ATOMIC(  MagicMusicKeyboard::noteOffset = MagicMusicKeyboard::noteOffset - 1);
          MIDI.sendControlChange(123, 0, 1);
          // midiAllNotesOff();/////////////////////
          break;
      }                                                    /////////////////
      break;                                                       /////////////
    /////////////
    case BTKC_KEYPAD_MULTIPLY:              ///////////////////

      switch (isPressed) {
        case true: //////////////////
          ATOMIC(  MagicMusicKeyboard::noteOffset = MagicMusicKeyboard::noteOffset + 1);
          MIDI.sendControlChange(123, 0, 1);
          break;
          // midiAllNotesOff();
      }////////////////////
      break;                                                   //////////////////////
    ////////////////////
    ///////////////////////////////////////////////////////////////////////
    ////////////////NOTE SHIFT WITH NUMPAD + AND - //////////////////////
    ///////////////////////////////////////////////////////////////////////

    case BTKC_KEYPAD_DECIMAL:              ///////////////////

      switch (isPressed) {
        case true: //////////////////
          if (spreadcounter < 71 ) {
            spreadcounter = spreadcounter + 1;
            MIDI.sendControlChange(123, 0, 1);
          } else {
            spreadcounter = 0;
            MIDI.sendControlChange(123, 0, 1);
          }
          // midiAllNotesOff();
          break;
      }////////////////////
      break;
    case BTKC_KEYPAD_0  :            ///////////////////

      switch (isPressed) {
        case true://////////////////
          if (spreadcounter > 0) {
            spreadcounter = spreadcounter - 1;
            MIDI.sendControlChange(123, 0, 1);
          }
          break;
          // midiAllNotesOff();
      }////////////////////
      break;

    //////////////////////////////////////////////////////////////////////////////////////


    case BTKC_PAGE_UP :              ///////////////////

      switch (isPressed) {
        case true: //////////////////
          chordSize ++;
          MIDI.sendControlChange(123, 0, 1);
          break;
          // midiAllNotesOff();
      }////////////////////
      break;
    case BTKC_PAGE_DOWN :              ///////////////////

      switch (isPressed) {
        case true://////////////////
          chordSize = chordSize - 1;
          MIDI.sendControlChange(123, 0, 1);
          break;
          // midiAllNotesOff();
      }////////////////////
      break;


    case BTKC_HOME :             ///////////////////

      switch (isPressed) {
        case true://////////////////
          if (scaleset < 63) {
            ATOMIC(   scaleset++);
          };
          MIDI.sendControlChange(123, 0, 1);
          break;
          // midiAllNotesOff();
      }////////////////////
      break;

    case BTKC_END :             ///////////////////

      switch (isPressed) {
        case true: //////////////////
          if (scaleset > 0) {
            ATOMIC( scaleset--);
          };
          MIDI.sendControlChange(123, 0, 1);
          break;
          // midiAllNotesOff();
      }////////////////////
      break;
    //////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    case BTKC_LCTRL:

      switch (isPressed) {
        case true:
          modLCTRL = true;

          return true;
          break;
        case false:
          modLCTRL = false;
          return false;
          break;
      }
      break;

    case BTKC_F1:

      switch (isPressed) {
        case true:

          switch (modLCTRL) {
            case true:
              chordbankset = 0;
              //flashLED(2);
              return true;
              break;
            case false:
              sendnote(btCode);
              break;
          }

          break;
        case false:
          midiNoteOffMain(btCode );
          return false;
          break;
      }


      break;

    case BTKC_F2:
      switch (isPressed) {
        case true:

          switch (modLCTRL) {
            case true:
              chordbankset = 1;
              //flashLED(2);
              return true;
              break;
            case false:
              sendnote(btCode);
              break;
          }

          break;
        case false:
          midiNoteOffMain(btCode );
          return false;
          break;
      }

      break;

    case BTKC_F3:
      switch (isPressed) {
        case true:

          switch (modLCTRL) {
            case true:
              chordbankset = 2;
              //flashLED(2);
              return true;
              break;
            case false:
              sendnote(btCode);
              break;
          }

          break;
        case false:
          midiNoteOffMain(btCode );
          return false;
          break;
      }

      break;

    case BTKC_F4:
      switch (isPressed) {
        case true:

          switch (modLCTRL) {
            case true:
              chordbankset = 3;
              //flashLED(2);
              return true;
              break;
            case false:
              sendnote(btCode);
              break;
          }

          break;
        case false:
          midiNoteOffMain(btCode );
          return false;
          break;
      }

      break;

    case BTKC_F5:
      switch (isPressed) {
        case true:

          switch (modLCTRL) {
            case true:
              chordbankset = 4;
              //flashLED(2);
              return true;
              break;
            case false:
              sendnote(btCode);
              break;
          }

          break;
        case false:
          midiNoteOffMain(btCode );
          return false;
          break;
      }

      break;

    case BTKC_F6:
      switch (isPressed) {
        case true:

          switch (modLCTRL) {
            case true:
              chordbankset = 5;
              //flashLED(2);
              return true;
              break;
            case false:
              sendnote(btCode);
              break;
          }

          break;
        case false:
          midiNoteOffMain(btCode );
          return false;
          break;
      }

      break;

    case BTKC_F7:
      switch (isPressed) {
        case true:

          switch (modLCTRL) {
            case true:
              chordbankset = 6;
              //flashLED(2);
              return true;
              break;
            case false:
              sendnote(btCode);
              break;
          }

          break;
        case false:
          midiNoteOffMain(btCode );
          return false;
          break;
      }

      break;

    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////


    case BTKC_ESCAPE:

      switch (isPressed) {
        case true:

          uClock.stop();
          MIDI.sendControlChange(123, 0, 1);
          PORTB = PORTB & B11011111; // led13 off
          noteOffset = 0;
          octOffset = 0;
          spreadcounter = 0;
          scaleset = 0;

          for (int i = 0; i < 128; i++) {
            seqnotes[i] = 0;
          }

          for (int i = 0; i < 64; i++) {
            velocityseq[i] = 100;
          }

          for (int i = 0; i < 64; i++) {
            seqmodwheel[i] = 0;
          }

          seqcount = 0;
          veloseqcount = 0;
          modseqcount = 0;
          mscounter = 0;
          scounter = 0;
          vscounter = 0;
          sequencerOn = false;
          //  timing = 0;

          seqwritemode = 1;
          return true;
          break;
        case false:
          modESC = false;
          return false;
          break;
      }
      break;

    case BTKC_CAPS_LOCK: /// hold notes mode

      switch (isPressed) {
        case true:
          MIDI.sendControlChange(123, 0, 1);
          capsLock = !capsLock;

          switch (scrLock) {
            case true:
              scrLock = false; // switch off Chord Mode in Hold Mode
              chordMode = false;
              break;
          }

          switch (capsLock) {
            case true:


              holdNotes();

              switch (sequencerOn) {

                case true:

                  uClock.pause();
                  setLED();
                  uClock.start();
                  break;
                case false:
                  setLED();
                  break;
              }
              break;
            case false:
              holdNotes();

              switch (sequencerOn) {
                case true:

                  uClock.pause();
                  setLED();
                  uClock.start();
                  break;
                case false:
                  setLED();
                  break;
              }
              break;
          }
          return true;
          break;
        case false:
          return false;
          break;
      }
      break;

    case BTKC_NUM_LOCK: /// chromatic scale mode

      switch (isPressed) {
        case true:
          MIDI.sendControlChange(123, 0, 1);

          switch (scrLock) {
            case true:


              ATOMIC( scrLock = false); // switch off Chord Mode in crhomatic mode
              ATOMIC(  chordMode = false);
              break;
          }

          ATOMIC(
            numLock = !numLock;
            seqmodeset();

          );
          // add numlock function here
          // midiAllNotesOff();

          // delay(5);

          //MagicMusicKeyboard::mdelay(150);

          switch (sequencerOn) {
            case true:
              uClock.pause();
              setLED();
              uClock.start();
              break;
            case false:
              setLED();
              break;
          }


          return true;
          break;
        case false:
          //MagicMusicKeyboard::mdelay(150);
          switch (sequencerOn) {

            case true:

              uClock.pause();
              setLED();
              uClock.start();
              break;
            case false:
              setLED();
              break;
          }


          return false;
          break;
      }
      break;

    case BTKC_SCROLL_LOCK:   /// chord mode

      switch (isPressed) {
        case true:
          MIDI.sendControlChange(123, 0, 1);
          scrLock = !scrLock;

          switch (numLock) {

            case true:

              ATOMIC(  numLock = false); // switch off chromatic mode in chord mode
              break;
          }

          switch (capsLock) {
            case true:
              capsLock = false; // switch off Chord Mode in Hold Mode
              break;
          }

          switch (scrLock) {

            case true:

              ATOMIC(  chordMode = true;
                       seqmodeset();
                    );
              if (sequencerOn == true) {
                uClock.pause();
                setLED();
                uClock.start();
              } else {
                setLED();
              }
              break;
            case false:
              //
              MIDI.sendControlChange(123, 0, 1);
              ATOMIC(
                chordMode = false;
                seqmodeset();
              );

              switch (sequencerOn) {
                case true:


                  uClock.pause();
                  setLED();
                  uClock.start();
                  break;
                case false:
                  setLED();
                  break;
              }
              break;
          }
          return true;
          break;
        case false:
          return false;
          break;
      }

      break;

    case BTKC_KEYPAD_ENTER:        ///////////////////////////////////////////////*****    SEQUENCER START / STOP    *****//////////////////////////////////////////////////////////////////////

      //reset key (escape)
      switch (isPressed) {
        case true:

          scounter = 0;
          vscounter = 0;

          //

          switch (sequencerOn) {
            case true:


              ATOMIC( sequencerOn = false);
              PORTB = PORTB & B11011111; // led13 off
              break;
            case false:
              ATOMIC( sequencerOn = true);
              break;
          }

          switch (sequencerOn) {
            case true:


              seqwritemode = 1;
              uClock.start();

              break;
            case false:
              uClock.stop();
              break;
          }

          return true;
          break;
        case false:
          return false;
          break;
      }
      break;

    case BTKC_PAUSE:

      //reset key (escape)
      switch (isPressed) {
        case true:
          MIDI.sendControlChange(123, 0, 1);
          // midiAllNotesOff();
          //flashLED();
          return true;
          break;
        case false:
          return false;
          break;
      }
      break;
    case BTKC_SPACE:

      // space doing empty step in sequencer mode
      switch (isPressed) {
        case true:

          switch (sequencerOn) {

            case true:

              if (seqcount < 128) {
                ATOMIC( seqnotes[seqcount] = 0;
                        seqcount++;
                      );
              }
              MIDI.sendControlChange(123, 0, 1);
              break;
            case false:
              //do nothing
              break;
          }

          return true;
          break;
        case false:
          return false;
          break;
      }
      break;



    case BTKC_RSHIFT:


      switch (isPressed) {
        case true:
          bpm++;
          uClock.setTempo(bpm);

          return true;
          break;
        case false:
          return false;
          break;
      }
      break;

    case BTKC_RCTRL:


      switch (isPressed) {
        case true:
          bpm = bpm - 1;
          uClock.setTempo(bpm);
          return true;
          break;
        case false:
          return false;
          break;
      }
      break;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////SEQUENCER WRITE MODE {{{{/////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case BTKC_KEYPAD_1:


      switch (isPressed) {
        case true:
          switch (sequencerOn) {

            case true:

              ATOMIC (  seqwritemode = 1);
              break;
          }
          return true;
          break;
        case false:
          return false;
          break;
      }
      break;

    case BTKC_KEYPAD_2:


      switch (isPressed) {
        case true:
          switch (sequencerOn) {

            case true:

              ATOMIC ( seqwritemode = 2);
              break;
          }
          return true;
          break;
        case false:
          return false;
          break;
      }
      break;

    case BTKC_KEYPAD_3:


      switch (isPressed) {
        case true:
          switch (sequencerOn) {

            case true:

              ATOMIC ( seqwritemode = 3);
              break;
          }
          return true;
          break;
        case false:
          return false;
          break;
      }
      break;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////}}}}}} SEQUENCER WRITE MODE ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    case BTKC_NONE:


      // do nothing
      break;
    //end non-dependent

    default:

      switch (isPressed) {
        case true:
          sendnote(btCode);
          break;
        case false:
          // pressedkeycounter = pressedkeycounter - 1;
          midiNoteOffMain(btCode);
          break;
      }
      return true;
      break;
  }

}

void MagicMusicKeyboard::midiNoteOnMain(uint8_t note)
{
  switch (chordMode) {
    case true:


      //    for (int k = 0; k < 12; k++) {
      //      MIDI.sendNoteOff(pgm_read_byte(&testChord[chordbankset][note - 24][k]) + octOffset + noteOffset + spread[spreadcounter][k], NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL); //clear out channel, faster than doing all notes every time which has a noticable lag
      //      //  delay(1);
      //      Serial.flush();
      //    }
      //delay(1);
      MIDI.sendControlChange(123, 0, 1);

      for (int k = 0; k < chordSize; k++) {

        //MIDI.sendNoteOn(secondChord[k], NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
        switch (pgm_read_byte(&testChord[chordbankset][note - 24][k]) ) {

          case 0:
            //do nothing
            break;
          default:

            MIDI.sendNoteOn(pgm_read_byte(&testChord[chordbankset][note - 24][k]) + octOffset + noteOffset + spread[spreadcounter][k], random(60, 80), MIDI_BLOCK_CHORD_CHANNEL);
            // delay(1);
            //enabledchordnotes[k] = pgm_read_byte(&testChord[0][note - 24][k]) + octOffset + noteOffset + spread[spreadcounter][k];
            Serial.flush();
            PORTB = PORTB | B00100000; // led13 on
            break;
        }

        //   delay(1);



      }
      //  delay(1);
      break;
    case false:

      switch (capsLock) {

        case true:
          for (int i = 0; i < 13; i++) {


            if (enabledNotes[i] == note + octOffset + noteOffset) {
              enabledNote = true;
              //                 enabledNotesCounteri = i;
              MIDI.sendNoteOff(enabledNotes[i], NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
              Serial.flush();
              enabledNotes[i] = 0;
              enabledNotesCounter = enabledNotesCounter - 1;

              break;
            }
          }


          switch (enabledNote) {
            case true:

              enabledNote = false;
              break;
            case false:


              if (enabledNotesCounter < 13) {

                switch (enabledNotes[enabledNotesCounter]) {

                  case 0:


                    MIDI.sendNoteOn(note + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
                    Serial.flush();
                    PORTB = PORTB | B00100000; // led13 on
                    enabledNotes[enabledNotesCounter] = note + octOffset + noteOffset;
                    enabledNotesCounter++;
                    break;
                  default:
                    MIDI.sendNoteOff(enabledNotes[enabledNotesCounter], NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
                    Serial.flush();
                    MIDI.sendNoteOn(note + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);

                    Serial.flush();
                    PORTB = PORTB | B00100000; // led13 on
                    enabledNotes[enabledNotesCounter] = note + octOffset + noteOffset;
                    enabledNotesCounter++;
                    break;
                }

              } else {
                MIDI.sendNoteOff(enabledNotes[0], NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
                Serial.flush();
                MIDI.sendNoteOn(note + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
                Serial.flush();
                PORTB = PORTB | B00100000; // led13 on
                enabledNotes[0] = note + octOffset + noteOffset;
                enabledNotesCounter = 1;



              }

              break;

          }

          break;
        case false:

          switch (numLock) {
            case false:

              MIDI.sendNoteOn(note + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
              Serial.flush();
              PORTB = PORTB | B00100000; // led13 on
              //delay(1);
              break;
            case true:

              MIDI.sendNoteOn(pgm_read_byte(&scalesNotes[note - 24][scaleset]) + octOffset + noteOffset , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
              Serial.flush();
              PORTB = PORTB | B00100000; // led13 on
              break;
          }
          break;
      }
      break;
  }

}

void MagicMusicKeyboard::midiNoteOffMain(uint8_t note)
{

  switch (chordMode) {
    case true:




      for (int i = 0; i < chordSize; i ++ ) {
        MIDI.sendNoteOff(pgm_read_byte(&testChord[chordbankset][note - 24][i]) + octOffset + noteOffset + spread[spreadcounter][i], NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
        Serial.flush();
        PORTB = PORTB & B11011111; // led13 off

      }

      //    MIDI.sendControlChange(123, 0, 1);
      //   PORTB = PORTB & B11011111; // led13 off
      break;
    case false:

      switch (capsLock) {

        case true:
          //do nothing
          break;
        case false:


          switch (numLock) {

            case false:

              MIDI.sendNoteOff(note + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
              Serial.flush();
              PORTB = PORTB & B11011111; // led13 off
              // delay(1);
              break;

            case true:

              MIDI.sendNoteOff(pgm_read_byte(&scalesNotes[note - 24][scaleset]) + octOffset + noteOffset , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
              Serial.flush();
              PORTB = PORTB & B11011111; // led13 off
              break;
          }
          break;
      }
      break;
  }
}








void MagicMusicKeyboard::midiAllNotesOff()
{
  MIDI.sendControlChange(123, 0, 1);
  Serial.flush();
  PORTB = PORTB & B11011111; // led13 off

  //
  //delay(2);
}


void MagicMusicKeyboard::cDelay()
{
  MagicMusicKeyboard::mdelay(30);
}

void MagicMusicKeyboard::setLEDall()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  MagicMusicKeyboard::mdelay(5);
  write(0x07, false);  //set capslock on

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard::setLEDoff()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  MagicMusicKeyboard::mdelay(5);
  write(0x00, false);  //set capslock on

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard::setLEDcaps()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  MagicMusicKeyboard::mdelay(5);
  write(0x04, false);  //set capslock on

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard:: setLEDnumcaps()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  cDelay();
  write(0x06, false);  //set capslock on

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard:: setLEDcapsscr()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  MagicMusicKeyboard::mdelay(5);
  write(0x05, false);  //set capslock on

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard:: setLEDnumscr()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  MagicMusicKeyboard::mdelay(5);
  write(0x03, false);  //set capslock off

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard:: setLEDscr()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  MagicMusicKeyboard::mdelay(5);
  write(0x01, false);  //set capslock off

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard:: setLEDnum()
{
  write(0xED, false);  //initiate LED set state

  read(NULL, false);  // Acknowledge - 0xFA
  MagicMusicKeyboard::mdelay(5);
  write(0x02, false);  //set capslock off

  read(NULL, false);  // Acknowledge - 0xFA
}

void MagicMusicKeyboard::setLED()
{
  if (capsLock == true && numLock == false && scrLock == false)
  {
    write(0xED, true);  //initiate LED set state

    read(0xFA,  true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x04, true);  //set capslock on

    read(0xFA,  true);  // Acknowledge - 0xFA
  }

  else if (capsLock == true && numLock == true && scrLock == false)
  {
    write(0xED, true);  //initiate LED set state

    read(NULL, true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x06, true);  //set capslock on

    read(NULL, true);  // Acknowledge - 0xFA
  }

  else if (capsLock == true && numLock == true && scrLock == true)
  {
    write(0xED, true);  //initiate LED set state

    read(NULL, true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x07, true);  //set capslock on

    read(NULL, true);  // Acknowledge - 0xFA
  }

  else if (capsLock == true && numLock == false && scrLock == true)
  {
    write(0xED, true);  //initiate LED set state

    read(NULL, true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x05, true);  //set capslock on

    read(NULL, true);  // Acknowledge - 0xFA
  }

  else if (capsLock == false && numLock == false && scrLock == false)
  {
    write(0xED, true);  //initiate LED set state

    read(NULL, true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x00, true);  //set capslock off

    read(NULL, true);  // Acknowledge - 0xFA
  }

  else if (capsLock == false && numLock == true && scrLock == false)
  {
    write(0xED, true);  //initiate LED set state

    read(NULL,  true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x02, true);  //set capslock off

    read(NULL,  true);  // Acknowledge - 0xFA
  }

  else if (capsLock == false && numLock == true && scrLock == true)
  {
    write(0xED, true);  //initiate LED set state

    read(0xFA,  true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x03, true);  //set capslock off
    //
    read(0xFA,  true);  // Acknowledge - 0xFA
  }

  else // capsLock == false && numLock == false && scrLock == true
  {
    write(0xED, true);  //initiate LED set state

    read(NULL,  true);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(7);
    write(0x01, true);  //set capslock off

    read(NULL,  true);  // Acknowledge - 0xFA
  }
}

void MagicMusicKeyboard::flashLED(int count)
{
  for (int x = count; x > 0; x = x - 1)
  {
    write(0xED, false);  //initiate LED set state
    read(NULL, false);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(15);
    write(0x07, false);  //set capslock on
    read(NULL, false);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(flashDelay);
    write(0xED, false);  //initiate LED set state
    read(NULL, false);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(15);
    write(0x00, false);  //set capslock on
    read(NULL, false);  // Acknowledge - 0xFA
    MagicMusicKeyboard::mdelay(flashDelay);
  }
  setLED();
}

void MagicMusicKeyboard::lightShow()
{
  setLEDall();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDoff();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDnum();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDnumcaps();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDall();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDcapsscr();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDnumscr();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDnumcaps();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDall();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDcapsscr();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDscr();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDoff();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDall();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDoff();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDall();
  MagicMusicKeyboard::mdelay(flashLong);
  setLEDoff();
  MagicMusicKeyboard::mdelay(flashLong);

}





void MagicMusicKeyboard::holdNotes() {

  switch (capsLock) {

    case false:



      //    for (int i = 0; i < 15; i++) {
      //      MIDI.sendNoteOff(enabledNotes[i] , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
      //      enabledNotes[i] = 0;
      //    }
      enabledNotesCounter = 0;
      enabledNote = false;
      MIDI.sendControlChange(123, 0, 1);
      PORTB = PORTB & B11011111; // led13 off
      break;
  }
}



static void MagicMusicKeyboard::ClockOut16PPQN(uint32_t * tick) {

  switch (seqcount) {
    case 0:
      //do nothing
      break;
    default:
      switch (seqmode) {
        case 0:

          MagicMusicKeyboard::sendmodewheel();
          switch (noteoffsended) {
            case true:
              switch (seqnotes[scounter]) {
                case 0:
                  //do nothing;
                  MIDI.sendNoteOff(seqnotes[scounter] + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset , velocityseq[vscounter], 1);
                  Serial.flush();
                  PORTB = PORTB | B00000100;
                  noteoffsended = false;
                  nolen = 3;
                  break;
                default:

                  MIDI.sendNoteOn(seqnotes[scounter] + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset , velocityseq[vscounter], 1);
                  Serial.flush();
                  PORTB = PORTB | B00100000;
                  noteoffsended = false;
                  nolen = 3;
                  break;
              }
              break;

          }

          break;

        case 1:
          MagicMusicKeyboard::sendmodewheel();

          switch (noteoffsended) {
            case true:
              switch (seqnotes[scounter]) {
                case 0:
                  //do nothing
                  MIDI.sendNoteOff(pgm_read_byte(&scalesNotes[seqnotes[scounter] - 24][scaleset]) + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset, velocityseq[vscounter], 1);
                  Serial.flush();
                  PORTB = PORTB | B00000100;
                  noteoffsended = false;
                  nolen = 3;
                  break;
                default:

                  MIDI.sendNoteOn(pgm_read_byte(&scalesNotes[seqnotes[scounter] - 24][scaleset]) + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset, velocityseq[vscounter], 1);
                  Serial.flush();
                  PORTB = PORTB | B00100000;
                  noteoffsended = false;
                  nolen = 3;
                  break;
              }
              break;

          }

          break;
        case 2:
          MagicMusicKeyboard::sendmodewheel();
          MIDI.sendControlChange(123, 0, 1);

          for (int k = 0; k < chordSize; k++) {

            switch (pgm_read_byte(&testChord[chordbankset][seqnotes[scounter] - 24][k])) {
              case 0:
                //do nothing;
                MIDI.sendNoteOff(pgm_read_byte(&testChord[chordbankset][(seqnotes[scounter]) - 24][k]) + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset + spread[spreadcounter][k], velocityseq[vscounter], MIDI_BLOCK_CHORD_CHANNEL);
                Serial.flush();
                PORTB = PORTB | B00000100; // led13 on
                break;
              default:
                MIDI.sendNoteOn(pgm_read_byte(&testChord[chordbankset][(seqnotes[scounter]) - 24][k]) + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset + spread[spreadcounter][k], velocityseq[vscounter], MIDI_BLOCK_CHORD_CHANNEL);
                Serial.flush();
                PORTB = PORTB | B00100000; // led13 on
                break;
            }

          }

          break;
      }

      break;
  }
}



static void MagicMusicKeyboard::ClockOut96PPQN(uint32_t * tick) {
  Serial.write(MIDI_CLOCK);
  switch (nolen) {
    case 0:
      switch (seqcount) {
        case 0:
          //do nothing
          break;
        default:
          switch (seqmode) {
            case 0:
              switch (noteoffsended) {

                case false:
                  switch (seqnotes[scounter]) {
                    case 0:
                      //do nothing
                      MIDI.sendNoteOff(seqnotes[scounter] + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset, 0, 16);
                      Serial.flush();
                      PORTB = PORTB & B11011111;
                      noteoffsended = true;
                      break;
                    default:
                      MIDI.sendNoteOff(seqnotes[scounter] + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset, 0, 1);
                      Serial.flush();
                      PORTB = PORTB & B11011111;
                      noteoffsended = true;

                      break;
                  }
                  MagicMusicKeyboard::noteoffroutine();
                  break;
                case true:
                  MagicMusicKeyboard::noteoffroutine();

                  break;
              }
              break;
            case 1:
              switch (noteoffsended) {

                case false:
                  switch (seqnotes[scounter]) {
                    case 0:
                      //do nothing
                      MIDI.sendNoteOff(pgm_read_byte(&scalesNotes[seqnotes[scounter] - 24][scaleset]) + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset, 0, 1);
                      Serial.flush();
                      PORTB = PORTB & B11011111;
                      noteoffsended = true;
                      break;
                    default:
                      MIDI.sendNoteOff(pgm_read_byte(&scalesNotes[seqnotes[scounter] - 24][scaleset]) + MagicMusicKeyboard::octOffset + MagicMusicKeyboard::noteOffset, 0, 1);
                      Serial.flush();
                      PORTB = PORTB & B11011111;
                      noteoffsended = true;

                      break;
                  }
                  MagicMusicKeyboard::noteoffroutine();
                  break;
                case true:
                  MagicMusicKeyboard::noteoffroutine();

                  break;
              }
              break;
            case 2:
              noteoffsended = true;
              MagicMusicKeyboard::noteoffroutine();
              break;
          }

          break;
      }
      break;
    default:
      nolen = nolen - 1;
      break;
  }
}


static void MagicMusicKeyboard::onClockStart() {
  Serial.write(MIDI_START);
}



static void MagicMusicKeyboard::onClockStop() {
  Serial.write(MIDI_STOP);
}


static void MagicMusicKeyboard::mdelay(uint32_t us) {
  uint32_t tmr = millis();
  while (millis() - tmr < us);
}


void MagicMusicKeyboard::sendnote(byte codebt) {

  switch (sequencerOn) {

    case false:

      // pressedkeycounter++;
      MagicMusicKeyboard::midiNoteOnMain(codebt );
      // chordnoteenabled == true;
      break;
    case true:   ////////////////////////////////////////////////////////////////// SEQUENCER ////////////////////////////////////////////////////

      switch (seqwritemode) {
        case 1:



          if (seqcount < 64) {
            ATOMIC( seqnotes[seqcount] = codebt;
                    seqcount++;
                  );
          }
          break;
        case 2:


          if (veloseqcount < 64) {
            ATOMIC (  velocityseq[veloseqcount] = (codebt / 2) * (codebt / 23);
                      veloseqcount++;
                   );
          }

          break;
        case 3:


          if (modseqcount < 64) {
            ATOMIC (  seqmodwheel[modseqcount] = (codebt / 2) * (codebt / 23);
                      modseqcount++;
                   );
          }
          break;
      }
      break;
  }
}

void MagicMusicKeyboard::seqmodeset() {
  if (numLock == false && scrLock == false) {
    seqmode = 0;
  };
  if (numLock == true && scrLock == false) {
    seqmode = 1;
  };
  if (numLock == false && scrLock == true) {
    seqmode = 2;
  };
}



static void MagicMusicKeyboard::sendmodewheel() {
  switch (modseqcount) {
    case 0:
      //do nothing
      break;
    default:
      MIDI.sendControlChange(1, seqmodwheel[mscounter], 1);
      mscounter++;
      if (mscounter == modseqcount)
      {
        mscounter = 0;
      }
      break;
  }
}


static void MagicMusicKeyboard::noteoffroutine() {
  scounter++;
  if (scounter >= seqcount)
  {
    scounter = 0;
  }

  switch (veloseqcount) {
    case 0:
      //do nothing
      break;
    default:
      vscounter++;
      if (vscounter >= veloseqcount)
      {
        vscounter = 0;
      }
      break;
  }



  nolen = 3;
}
//schawrzonator presets
/*
  , reset_1_2_3_4_5_6_7_8_9_10_11_12;
  2, Leave My Head Alone Brain_1-4-8-13_1-4-11-13_3-8-11-15_4-8-11-16_1-8-11-13_4-11-13-16_3-8-13-15_8-11-15-20_8-13-16-20_8-11-16-20_11-16-20-23_11-15-20-23;
  3, Leave My Black Keys Alone_1-5-10-13_3-6-10-15_3-6-13-15_5-10-13-17_6-10-13-18_3-10-13-15_6-13-15-18_5-10-15-17_10-13-17-22_10-15-18-22_10-13-18-22_13-18-22-25;
  4, All Black_2-7-9_11-2-7_4-9-7_11-4-7_14-4-7_11-14-7_16-11-7_9-14-16_9-14-16_19-11-14_16-19-11_21-16-11;
  5, Hermitonic Pentatonic_1-4-8_2-9-4_13-9-4_13-9-4_1-8-4_9-13-16_13-4-9_16-8-14_9-13-16_16-8-14_9-13-16_9-13-16;
  6, Full Half_1-4-10-13_1-4-9-13_3-7-10-15_4-7-10-16_4-6-10-16_ 6-9-13-18_7-10-13-19_7-9-12-19_9-13-16-21_10-13-16-22_9-12-16-21_12-16-19-24;
  7, Half Full_1-4-8-13_2-5-9-14_2-5-8-14_4-7-11-16_5-10-13-17_5-11-13-17_7-11-14-19_8-11-17-20_8-13-16-20_8-13-17-20_11-14-17-23_11-13-16-23;
  8, You Are Sunshine_1-5-9-13_1-6-9-13_3-6-11-15_3-7-11-15_5-9-13-17_6-9-13-18_7-11-15-19_6-11-15-18_9-13-17-21_9-13-18-21_11-15-18-23_11-15-19-23;
  9, Sakamoto_1-5-8-12_3-5-8-12_9-3-5-12_10-13-8-5_15-12-8-5_6-17-10-13_8-17-10-13_11-18-15-8_9-12-17-15_10-13-18-15_22-11-18-15_20-12-17-15;
  10, Strings Of D_2-9-13-4_2-6-13-9_4-7-11-13_4-7-12-14_4-7-11-14_6-9-13-16_7-11-18-13_7-11-14-18_9-13-16-2_7-11-16-19_11-16-19-23_12-16-19-23;
  11, Anime_1-15-8-13_1-17-8-13_3-22-17-15_3-10-6-15_5-13-8-17_11-18-15-23_6-15-11-18_8-18-13-20_11-18-13-23_10-24-17-22_10-26-17-22_11-18-15-23;
  12, Michael_2-7-10-14_2-5-9-14_4-9-12-16_4-7-12-16_5-7-12-17_7-12-16-19_7-10-14-19_9-14-19-21_9-12-16-21_10-14-17-22_12-14-19-24_12-16-19-24;
  13, Âme Rej_1-9-6-13_3-10-6-15_3-10-6-11_3-10-8-11_5-12-8-17_6-11-8-17_6-14-10-17_8-14-11-17_8-14-11-20_8-15-11-18_6-15-10-18_5-15-11-20;
  14, Bugge Wesseltoft 1_1-3-6-10_1-3-6-10_3-5-8-11_3-5-8-11_6-8-11-15_6-8-11-15_6-8-11-15_8-10-13-17_8-10-13-17_8-10-13-17_11-13-17-20_11-13-17-20;
  15, FranG_1-4-7-11_3-6-11-13_4-6-9-13_6-9-11-13_6-11-14-16_6-9-11-15_8-11-13-16_9-13-16-18_11-13-15-18_11-13-16-18_11-16-18-21;
  16, Ligeti_1-9-4-11_2-7-11-16_4-9-13-20_4-11-7-14_5-10-14-17_7-11-14-16_7-11-16-13_7-10-14-17_8-13-17-22_10-17-13-20_10-14-19-17_11-16-13-20_1-9-4-11;
  17, Pop_2-6-9-13_2-9-13-14_4-7-13-16_6-11-13-18_6-11-14-18_6-9-14-18_6-9-11-18_7-11-16-18_7-11-16-21_9-14-18-21_7-9-14-19_9-14-18-21_2-6-9-13;
  18, C Major 2_1-5-8-12__3-6-10-13__5-8-12-15_6-10-13-17__8-12-15-18__10-13-17-20__12-15-18-22;
  19, Does It Work_1-4-6-9_1-4-6-11__4-6-9-13_4-6-11-16_6-9-11-16_6-11-13-16__9-11-13-16-18_11-13-16-21_11-13-18-23_11-13-16-23;
  20, Back To Me __2-5-10-14__4-9-12-16_5-7-12-17_5-9-12-17_7-10-14-17_7-12-16-21_9-12-16-21_10-14-17-19_10-14-17-22_12-14-17-22;
*/







/////////old sequencer {{{



//void MagicMusicKeyboard::sequencer() {
//
//  //sequencerOn
//  while (sequencerOn == true) {
//
//    for (int i = 0; i < seqcount; i++) {
//      update();
//      if (seqnotes[i] == 0) {
//
//        delay(125);
//      } else {
//
//
//
//
//
//
//        MIDI.sendNoteOn(seqnotes[i] , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//        delay(70);
//        MIDI.sendNoteOff(seqnotes[i] , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//        delay(55);
//
//      }
//
//    }
//    update();
//
//  }
//}

//void MagicMusicKeyboard::seqclock() {
//
//  while (sequencerOn == true) {
//    if (micros() - timing2 > bpm / 6.0) {
//
//      timing2 = micros();
//
//      MIDI.sendRealTime(midi::Clock);
//      Serial.flush();
//
//
//
//      if (tick == 0 || tick == 6 || tick == 12 || tick == 18) {
//        if (seqnotes[scounter] != 0) {
//          MIDI.sendNoteOn(seqnotes[scounter] + octOffset + noteOffset , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//          MIDI.sendNoteOff(seqnotes[scounter] + octOffset + noteOffset , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//          Serial.flush();
//        }
//      }
//
//
//
//      if (tick < 24) {
//        tick++;
//      } else {
//        tick = 0;
//      }
//      if (scounter == seqcount)
//      {
//        scounter = 0;
//      }
//
//
//    }
//     tryRead();
//
//  }
//
//}



//void MagicMusicKeyboard::sequencer() {
//
//  //sequencerOn
//  while (sequencerOn == true) {
//
//    if (seqcount > 0) {
//
//      if (micros() - timing > bpm) {
//        // update();
//        timing = micros();
//
//        if (seqnotes[scounter] != 0) {
//
//          if (noteoffsended == true) {
//
//
//            if (numLock == false) {
//              MIDI.sendNoteOn(seqnotes[scounter] + octOffset + noteOffset , NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//
//              Serial.flush();
//              noteoffsended = false;
//
//
//            }
//
//            else if (numLock == true) {
//
//              MIDI.sendNoteOn(pgm_read_byte(&scalesNotes[seqnotes[scounter] - 24][scaleset]) + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//              Serial.flush();
//              noteoffsended = false;
//              //  update();
//            }
//
//          }
//
//        } else {
//          scounter++;
//
//
//        }
//
//      }
//
//
//      if (micros() - timing > seqnotelen) {
//
//        if (noteoffsended == false) {
//          //  update();
//
//          if (numLock == false) {
//            MIDI.sendNoteOff(seqnotes[scounter] + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//            Serial.flush();
//            noteoffsended = true;
//            // delay(1);
//            // update();
//
//          }
//
//          else if (numLock == true) {
//
//            MIDI.sendNoteOff(pgm_read_byte(&scalesNotes[seqnotes[scounter] - 24][scaleset]) + octOffset + noteOffset, NOTE_VELOCITY_MAIN, MIDI_BLOCK_CHORD_CHANNEL);
//            Serial.flush();
//            noteoffsended = true;
//
//          }
//
//          scounter++;
//
//        }
//
//      }
//
//
//    }
//    update();
//
//    if (scounter == seqcount)
//    {
//      scounter = 0;
//    }
//  }
//
//
//}
