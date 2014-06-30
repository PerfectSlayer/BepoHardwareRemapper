#include <avr/pgmspace.h>
#include <Usb.h>
#include <hidboot.h>

#define DEBUG
#define modeLED 11

#define NON 0
#define CTL 1
#define MAJ 2
#define ALT 4
#define RMJ 32
#define AGR 64

// function definitions
inline void SendKeysToHost (uint8_t *buf);

uint8_t minusKeyMap[] = {
// Description:
// - Index: HID input code
// - Documentation: Related qwerty touch
// - Documentation: Related bepo touch
// - Value: Converted azerty input related qwerty input code to produce bepo input
// Exemple: A (bepo) is the A touch (qwerty layout) so index is 4 (from HID code)
//          The value is HID code of the touch A in azerty layout located in qwerty layout: Q
//          So value is 20 (Q from HID code)
// A touch (querty layout) 
    0,    1,    2,    3,   20,   14,   27,   12,   19,    8,
//                          A     B      C    D     E     F
//                          A     K      X    I     P     E
   16,    6,    7,   23,   22,   21,    4,   33,   15,   13,
//  G     H     I     J     K     L     M     N     O     P
//  ,     C     D     T     S     R     Q     '     L     J
    5,   18,   24,   36,   25,   54,   31,   28,   47,   39,
//  Q     R     S     T     U     V     W     X     Y     Z
//  B     O     U     è     V     .     é     Y     ^     à
   32,   32,   32,   34,   45,   39,   46,   35,   55,   50,
//  1     2     3     4     5     6     7     8     9     0
//  "     "     "     (     )     @     +     -     /     *
   40,   41,   42,   43,   44,   46,   52,   26,   29,   38,
//                                -     =     [     ]     \
//                                =     %     Z     W     ç
   50,   17,   51,   53,   10,   11,    9,   57,   58,   59,
//  <     ;    '            M     ,     .
//        N    M            G     H     F
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
   39,  101,  102,  103,  104,  105,  106,  107,  108,  109,
//  <
//  à 
};

uint8_t minusKeyModifierMap[] = {
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  MAJ,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  AGR,  MAJ,    0,  MAJ,    0,
    0,    0,    0,    0,    0,    0,  MAJ,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
};


uint8_t capsKeyMap[] = {
    0,    1,    2,    3,   20,   14,   27,   12,   19,    8,
//                          A     B      C    D     E     F
//                          A     K      X    I     P     E
   54,    6,    7,   23,   22,   21,    4,   16,   15,   13,
//  G     H     I     J     K     L     M     N     O     P
//  ;     C     D     T     S     R     Q     ?     L     J
    5,   18,   24,   36,   25,   55,   31,   28,   56,   39,
//  Q     R     S     T     U     V     W     X     Y     Z
//  B     O     U     è     V     :     é     Y     !     à
   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
//  1     2     3     4     5     6     7     8     9     0
//  1     2     3     4     5     6     7     8     9     0
   40,   41,   42,   43,   44,   45,   36,   26,   29,   38,
//                                -     =     [     ]     \
//                                °     %     Z     W     ç
   50,   17,   51,   53,   10,   11,    9,   57,   58,   59,
//  <     ;    '            M     ,     .
//        N    M            G     H     F
   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
   39,  101,  102,  103,  104,  105,  106,  107,  108,  109,
//  <
//à(1) 
};

uint8_t capsKeyModifierMap[] = {
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  NON,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  MAJ,  MAJ,  MAJ,  NON,  MAJ,  NON,  NON,  MAJ,  NON,  NON,
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  AGR,  MAJ,  MAJ,  NON,
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,
  NON,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ,  MAJ
};


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
  
  // Check modifiers
  if ((buf[0] & AGR) == AGR) {
    KeyBuffer[0] = buf[0];
    KeyBuffer[2] = buf[2];
  } else if ((buf[0] & MAJ) == MAJ) {
    KeyBuffer[0] = buf[0];
    KeyBuffer[2] = capsKeyMap[buf[2]];
    uint8_t modifier = capsKeyModifierMap[buf[2]];
    ForceModifier(modifier, &KeyBuffer[0], MAJ);
    ForceModifier(modifier, &KeyBuffer[0], AGR);
  } else {
    KeyBuffer[0] = buf[0];
    KeyBuffer[2] = minusKeyMap[buf[2]];
    uint8_t modifier = minusKeyModifierMap[buf[2]];
    ForceModifier(modifier, &KeyBuffer[0], MAJ);
    ForceModifier(modifier, &KeyBuffer[0], AGR);
  }
  
  for (i=3; i<8; i++) {
    KeyBuffer[i] = minusKeyMap[buf[i]];
  }

  #ifdef DEBUG
  Serial.print("Read: ");
  for (i=0; i<8; i++) {
    Serial.print(KeyBuffer[i]);
    Serial.print(" ");
  }
  Serial.println("");
  #endif
  
  // Send keys to host
  SendKeysToHost(KeyBuffer); 
};

inline void ForceModifier(uint8_t source, uint8_t *dest, uint8_t modifier) {
  if ((*dest & modifier) != (source & modifier)) {
    if ((source & modifier) == 0) {
        *dest-= modifier;
      } else {
        *dest+= modifier;
      }
    }
}

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

