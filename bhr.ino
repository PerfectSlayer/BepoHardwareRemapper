#include <avr/pgmspace.h>
#include <Usb.h>
#include <hidboot.h>

#define DEBUG
#define modeLED 11


// function definitions
inline void SendKeysToHost (uint8_t *buf);

prog_uint8_t keyMap[] = {
// Description:
// - Index: HID input code
// - Documentation: Related qwerty touch
// - Documentation: Related bepo touch
// - Value: Converted azerty input related qwerty input code to produce bepo input
// Exemple: A (bepo) is the A touch (qwerty layout) so index is 4 (from HID code)
//          The value is HID code of the touch A in azerty layout located in qwerty layout: Q
//          So value is 20 (Q from HID code)
// A touch (querty layout) 
    0,    1,    2,    3,   20,   33,   54,   12,   19,    8,
//                          A     B      C    D     E     F
//                          A  '(2)   .(2)    I     P     E
   16,    6,    7,   23,   22,   21,   10,    4,   15,   19,
//  G     H     I     J     K     L     M     N     O     P
//  ,     C     D     T     S     R     G     A     L    13
    5,   18,   24,   36,   25,   14,   31,   27,   47,   28,
//  Q     R     S     T     U     V      W    X     Y     Z
//  B     O     U   è(1)    V     K   é(1)    X    ^(2)   Y
   32,   32,   32,   34,   45,   39,   46,   35,   55,   50,
//  1     2     3     4     5     6     7     8     9     0
//  "     "     "     (     )     @     +     -     /     *
   40,   41,   42,   43,   44,   46,   52,   26,   29,   49,
//                                -     =     [     ]
//                                =     %     Z     W
   50,   17,   51,   53,   11,    9,   38,   57,   58,   59,
//  <     ;    '            ,     .     /
//        N    M            H     F   ç(2)
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
   39,  101,  102,  103,  104,  105,  106,  107,  108,  109,
//  <
//à(1) 
};

// (1) Force no modifier
// (2) Force maj modifier


// global variables
//uint32_t ledBlinkTime = millis();
//uint16_t ledBlinkDelay = 500;

// Declare the key buffer to send
uint8_t KeyBuffer[8] = {0,0,0,0,0,0,0,0};

// Declare child class of parser: bepo remapper
class KeyboardBepoRemapper : public KeyboardReportParser {
protected:
  virtual void Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};


// Declare parse method of the remapper
void KeyboardBepoRemapper::Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  // Check error
  if (buf[2] == 1) {
    // Return on error
    return;
  }
  
  uint8_t i;
  


  #ifdef DEBUG
  // Print buffer
  for (i=0; i<8; i++) {
    Serial.print(buf[i], DEC);
    Serial.print(" ");
  }
  Serial.println("");
  #endif

  KeyBuffer[0] = buf[0];
   
//  if (buf[2] != 0) {
//    KeyBuffer[2] = 0x20;
//    KeyBuffer[0] = 0x10;
//  } else {
//    for (i=2; i<8; i++) {
//      KeyBuffer[i] = buf[i];
//    }
//  }

  Serial.print("Read: ");
  for (i=2; i<8; i++) {
    KeyBuffer[i] = keyMap[buf[i]];
    Serial.print(KeyBuffer[i]);
    Serial.print(" ");
  }
  Serial.println("");
   
      
//      // remap all keys according to the existing keymap
//      for (i=2; i<1; i++)
//      {
//        // handle special case of Shift-CAPSLOCK to be ignored by the remapper
//        if (buf[i] == KEY_CAPS_LOCK && buf[0] & 0x22)
//        {
//          KeyBuffer[i] = KEY_CAPS_LOCK;
//        }
//        else
//        {
//          // print the key based on the current layout
//          if (buf[i]>=4 && buf[i] <= 57)     	// transpose of 4 becoz our array starts from 0 but A is 4
//                                              // limit check to 57, which is the last mappable key (CAPSLOCK)
//          {
//            // if it was a special key of shift-CAPS, then only allow mapping if the key has been released at least once
//            
//              KeyBuffer[i] = pgm_read_byte(buf[i]-4);
//           
//          }
//          else
//            KeyBuffer[i] = buf[i];
//        }
//      }
      
      // send out key press
      SendKeysToHost (KeyBuffer); 

    // for (uint8_t i=0; i<8; i++)
    // {
        // PrintHex(KeyBuffer[i]);
        // Serial.print(" ");
    // }
    // Serial.println("");
    // Serial.println("");
        

};


inline void SendKeysToHost (uint8_t *buf) {
  Keyboard.set_modifier(buf[0]);
  Keyboard.set_key1(buf[2]);
  Keyboard.set_key2(buf[3]);
  Keyboard.set_key3(buf[4]);
  Keyboard.set_key4(buf[5]);
  Keyboard.set_key5(buf[6]);
  Keyboard.set_key6(buf[7]);
  Keyboard.send_now();
}


  
USB Usb;
//USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD> ExtKeyboard(&Usb);

// Keyboard report parset
KeyboardBepoRemapper remapper;

// Main program setup
void setup() {
    randomSeed(analogRead(0));

    // initialize the digital pin as an output.
    pinMode(modeLED, OUTPUT);  
    Keyboard.begin();

#ifdef DEBUG
    Serial.begin(115200);
    while(!Serial);
    Serial.println("Start v1");
#endif

    if (Usb.Init() == -1) {
#ifdef DEBUG
        Serial.println("OSC did not start.");
#else
        delay(1);
#endif
    }

    delay(200);
    ExtKeyboard.SetReportParser(0, (HIDReportParser*)&remapper);
}

// Main program loop
void loop() {
    Usb.Task();
}

