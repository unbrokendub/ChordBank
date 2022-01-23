 
#include "MagicMusicKeyboard.h"
#include "MIDI.h"


//MIDI baud rate
#define serialRate 31250//live
//#define serialRate 38400//diagnostic


//PS2 pins
#define kbdClock 3
#define kbdData 2

//First note offsets
int kbdOffset = 0; //added to MIDI values
int fxOffset = 48; //subtracted from MIDI values
int kpdOffset = 0;



MagicMusicKeyboard magicMusicKeyboard(kbdClock, kbdData, kbdOffset, fxOffset, kpdOffset);



void setup()
{

pinMode(13, OUTPUT);

  
  Serial.begin(serialRate);

  magicMusicKeyboard.init();
  //  Serial.println("qwertyphon ready");//diagnostic
  //  Serial.println();//diagnostic


  // Starts the clock, tick-tac-tick-tac...
  
  //  MIDI.begin(MIDI_STRUM_CHANNEL); //bypass only works with this one channel. TODO: research MIDI Library more

}

void loop()
{
  magicMusicKeyboard.update();
 // magicMusicKeyboard.sequencer();
 //magicMusicKeyboard.seqclock();
}

/*

   v1.5:
   Determine remaining CC items
   Find hex code for Sequencer start/stop
   use arrows to set incremental controls
   pgup pgdn volume in tenths?
   Shifted root to reference keys F and Jis there

   v1.4:
   ARP toggle assigned to Scroll Lock, Sustain pedal toggle assigned to Caps Lock SUCCESS!
   LED controls shifted to flashLED(int count) and setLED() (disabled most due to crashes)
   Created midiCCToggle(uint8_t ch, uint8_t cc, bool ccToggle) for Control Code functions.
   Initial MIDI channel for all sections set to 0

   v1.3:
   ESC+[LSHIFT,LCTRL,LALT]+[0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f] sets MIDI channel for main, Fx, keypad sections
   Fx keys cover one octave routed to a separate channel, static in bass range starting at E2
   Keypad keys are set for a drum kit, but needed codes are unknown as of yet. Use offset variable to set appropriate range?
   LED Controls enabled (verified)
     LED Toggles
     hex status  LR
     00  off     000
     01  scr     001
     02  num     100
     03  num+scr 101
     04  cap     010
     05  cap+scr 011
     06  num+cap 110
     07  all     111

   v1.2 notes:
   Enable CTRL, ALT, & SHIFT as modifiers to consolidate octave & pitch changes to Fx keys.
   Shifting LED controls to functions led

   v1.1 notes:
   Fx keys can now set the OCTAVE!
   NumPad 1-9/*- can set the ROOT NOTE!

*/
